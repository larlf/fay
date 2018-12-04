"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ejs = require("ejs");
const fs = require("fs");
const larlf = require("larlf");
const path = require("path");
const xlsx = require("xlsx");
const SourceMapSupport = require("source-map-support");
SourceMapSupport.install();
let log = larlf.log;
let Cmds = {};
let RootPath = path.resolve(__dirname, "..");
let TypeMap = {
    "int": "int",
    "byte": "byte",
    "long": "int64",
    "float": "float",
    "bool": "char",
    "double": "double"
};
let LastAction = null; //用于保存最后一条Action
class FayCfg {
    constructor() {
        this.dataType = new Map();
        let file = xlsx.readFile(path.resolve(__dirname, "../doc/FayLang.xlsx"));
        let json = xlsx.utils.sheet_to_json(file.Sheets['ValueType']);
        for (let key in json) {
            let it = json[key];
            this.dataType.set(it.Name.toString().toLowerCase(), it);
        }
    }
    getType(name) {
        let it = this.dataType.get(name);
        if (!it)
            log.error("Cannot find type : " + name);
        return it;
    }
}
//指令中的字段
class FayInstField {
    constructor(index, text) {
        this.index = index;
        if (text.indexOf(":") > 0) {
            let textArray = text.split(":");
            this.name = textArray[0];
            this.type = textArray[1];
        }
        else {
            this.name = "p" + this.index;
            this.type = text;
        }
    }
    //生成字段字符串
    fieldDefStr() {
        return this.getCType() + " " + this.name + ";";
    }
    getCType() {
        return CFG.getType(this.type).CType;
    }
}
//Fay语言指令定义
class FayInst {
    constructor(data) {
        this.props = [];
        this.values = [];
        this.actionVars = [];
        this.name = (data.Code1 ? data.Code1 : "") + (data.Code2 ? data.Code2 : "");
        this.action = data.Action ? data.Action.toString() : "";
        //记录一下最后一条指令
        if (this.action.trim() == "same" && LastAction)
            this.action = LastAction;
        else if (this.action && this.action.trim() != "nothing")
            LastAction = this.action;
        if (this.action.trim() == "nothing")
            this.action = "//DoNothing";
        if (data.Params) {
            let list = data.Params.trim().split("\n");
            for (let i = 0; i <= list.length; ++i) {
                if (list[i]) {
                    let str = list[i].trim();
                    this.props.push(new FayInstField(i + 1, str));
                }
            }
        }
        if (data.Values) {
            let list = data.Values.trim().split("\n");
            for (let i = 0; i <= list.length; ++i) {
                if (list[i]) {
                    let str = list[i].trim();
                    this.values.push(new FayInstField(i + 1, str));
                }
            }
        }
        if (data.ActionVars) {
            let list = data.ActionVars.trim().split("\n");
            for (let i = 0; i <= list.length; ++i) {
                if (list[i]) {
                    let str = list[i].trim();
                    this.actionVars.push(str);
                }
                else {
                    this.actionVars.push("");
                }
            }
        }
    }
    makeHeadCode() {
        let tpl = fs.readFileSync(path.resolve(RootPath, "tools/tpl/inst_h.ejs")).toString();
        let text = ejs.render(tpl, {
            "data": this
        });
        //去除空行
        text = text.replace(/\n\s*\n/g, "\n");
        text = text.replace(/\&amp\;/g, "&");
        return text;
    }
    makeCppCode() {
        let tpl = fs.readFileSync(path.resolve(RootPath, "tools/tpl/inst_c.ejs")).toString();
        let text = ejs.render(tpl, {
            "data": this
        });
        //去除空行
        text = text.replace(/\n\s*\n/g, "\n");
        text = text.replace(/\&amp\;/g, "&");
        return text;
    }
    makeParamStr() {
        let str = "";
        this.props.forEach(it => {
            if (str.length)
                str += ",";
            if (it.type == "string")
                str += "const std::string &" + it.name;
            else
                str += it.getCType() + " " + it.name;
        });
        return str;
    }
    makeInitParamStr() {
        let str = "";
        this.props.forEach(it => {
            str += ",";
            str += it.name + "(" + it.name + ")";
        });
        return str;
    }
    makeCaseCode() {
        //只生成有代码的
        if (!this.action)
            return "";
        let str = this.action;
        //替换属性
        for (let i = 0; i < this.props.length; ++i) {
            let it = this.props[i];
            if (it) {
                let reg1 = new RegExp("\\#p" + it.index, "g");
                str = str.replace(reg1, "((inst::" + this.name + "*)inst)->" + it.name);
            }
        }
        //替换值
        for (let i = 0; i < this.values.length; ++i) {
            let it = this.values[i];
            if (it) {
                let reg1 = new RegExp("\\#v" + it.index, "g");
                str = str.replace(reg1, "((inst::" + this.name + "*)inst)->" + it.name);
            }
        }
        //替换变量
        for (let i = 0; i < this.actionVars.length; ++i) {
            let it = this.actionVars[i].trim();
            if (it) {
                let reg1 = new RegExp("\\#a" + (i + 1), "g");
                str = str.replace(reg1, it);
            }
        }
        str = str.replace(/\#s/g, "this->stack");
        str = str.replace(/\#val/g, "FayValue");
        str = str.replace(/\#new/g, "FayValue");
        str = str.replace(/\n/g, "\n\t"); //缩进
        str = "case InstType::" + this.name + ":\n{" + (str ? "\n\t" + str : str) + "\n\tbreak;\n}";
        return str;
    }
    makeNameCode() {
        return larlf.text.format("TypeDict::InstTypeName[InstType::{0}] = \"{0}\";", this.name);
    }
}
function replaceFileBody(filename, keyword, str, indent) {
    filename = path.resolve(RootPath, filename);
    let text = fs.readFileSync(filename).toString();
    text = larlf.text.replaceBlock(text, new RegExp(keyword + "Start", "g"), new RegExp(keyword + "End", "g"), str, indent);
    log.debug("Write : " + filename);
    fs.writeFileSync(filename, text);
}
////////////////////////////////////////////////////////////////////////////
let CFG = new FayCfg();
function main() {
    let cmd = process.argv[2];
    if (cmd) {
        cmd = cmd.replace(/\./g, "_");
        if (Cmds[cmd])
            Cmds[cmd]();
        else
            log.error("Cannot find cmd : " + cmd);
    }
}
Cmds._help = "帮助信息";
Cmds.help = function () {
    for (let cmd in Cmds) {
        if (!cmd.startsWith('_')) {
            console.log((cmd + "").yellow.bold);
            let comment = Cmds['_' + cmd];
            if (comment)
                console.log("\t" + comment);
        }
    }
};
Cmds._run = "运行的所有";
Cmds.run = function () {
    Cmds.token_type();
    Cmds.value_type();
    Cmds.inst();
    Cmds.convert_inst();
};
Cmds._token_type = "生成Token类型的数据";
Cmds.token_type = function () {
    let file = xlsx.readFile(path.resolve(__dirname, "../doc/FayLang.xlsx"));
    let json = xlsx.utils.sheet_to_json(file.Sheets['TokenType']);
    console.log(json);
    let str1 = "";
    let str2 = "";
    for (let i = 0; i < json.length; ++i) {
        let it = json[i];
        if (it.Code) {
            if (str1.length > 0)
                str1 += "\n";
            str1 += it.Code + ",";
            if (it.Comment)
                str1 += "  //" + it.Comment;
            if (str2.length > 0)
                str2 += "\n";
            str2 += "TypeDict::TokenTypeName[TokenType::" + it.Code + "] = \"" + it.Code + "\";";
        }
    }
    replaceFileBody("src/fay_const.h", "TokenType", str1, "\t\t");
    replaceFileBody("src/fay_const.cpp", "TokenTypeName", str2, "\t");
};
Cmds.value_type = function () {
    let file = xlsx.readFile(path.resolve(__dirname, "../doc/FayLang.xlsx"));
    let json = xlsx.utils.sheet_to_json(file.Sheets['ValueType']);
    log.info("Prase Value Type");
    let str1 = "", str2 = "", str3 = "";
    let index = 1;
    for (let i = 0; i < json.length; ++i) {
        let it = json[i];
        log.debug("Value Type : " + it['Name']);
        if (it['Name']) {
            if (str1.length > 0)
                str1 += "\n";
            str1 += it.Name + " = " + it.Value + ",";
            if (it.Comment)
                str1 += "  //" + it.Comment;
            str2 += str2.length ? "\n" : "";
            str2 += larlf.text.format("TypeDict::ValueTypeName[ValueType::{0}] = \"{1}\";", it['Name'], (it['Name'] + "").toLowerCase());
            str3 += str3.length ? "\n" : "";
            str3 += larlf.text.format("TypeDict::ValueTypeMap[\"{0}\"] = ValueType::{1};", (it.Name + "").toLowerCase(), it.Name);
        }
    }
    replaceFileBody("src/fay_const.h", "ValueType", str1, "\t\t");
    replaceFileBody("src/fay_const.cpp", "ValueTypeName", str2, "\t");
    replaceFileBody("src/fay_const.cpp", "ValueTypeMap", str3, "\t");
};
Cmds.inst = function () {
    let file = xlsx.readFile(path.resolve(__dirname, "../doc/FayLang.xlsx"));
    let json = xlsx.utils.sheet_to_json(file.Sheets['Inst']);
    let valueFilename = path.resolve(__dirname, "../data/inst.json");
    let values = JSON.parse(fs.readFileSync(valueFilename).toString());
    let maxValue = 0;
    for (let key in values) {
        if (values[key] > maxValue)
            maxValue = values[key];
    }
    log.debug("Max inst : " + maxValue);
    //用于处理Code到Value的转换
    let Code1Value = new Map();
    let hText = "";
    let cppText = "";
    let typeText = "";
    let caseText = "";
    let nameText = "";
    for (let i = 0; i < json.length; ++i) {
        let it = json[i];
        //只处理需要处理的语句
        if ((it.Code1 || it.Code2) && !it.Disabled) {
            let inst = new FayInst(it);
            if (values[inst.name] === undefined) {
                maxValue++;
                inst.code = maxValue;
                values[it.name] = maxValue;
            }
            else {
                inst.code = values[it.Name];
            }
            if (it.Code1)
                Code1Value.set(it.Code1, true);
            hText += inst.makeHeadCode();
            cppText += inst.makeCppCode();
            typeText += (typeText ? "\n" : "") + inst.name + " = " + inst.code + ",";
            nameText += (nameText ? "\n" : "") + inst.makeNameCode();
            //生成运行代码
            let code = inst.makeCaseCode();
            caseText += (caseText && code ? "\n" : "") + code;
        }
    }
    //生成指类分类的常量
    let groupText = "";
    Code1Value.forEach((value, key) => {
        if (key) {
            if (groupText)
                groupText += "\n";
            groupText += key + ",";
        }
    });
    //保存已生成的代码值
    log.debug("Write : " + valueFilename);
    fs.writeFileSync(valueFilename, JSON.stringify(values, null, "\t"));
    replaceFileBody("src/fay_inst.h", "Inst", hText, "\t\t");
    replaceFileBody("src/fay_inst.cpp", "Inst", cppText, "");
    replaceFileBody("src/fay_const.h", "InstType", typeText, "\t\t");
    replaceFileBody("src/fay_const.h", "InstGroupType", groupText, "\t\t");
    replaceFileBody("src/fay_const.cpp", "InstTypeName", nameText, "\t");
    replaceFileBody("src/fay_vm.cpp", "InstCode", caseText, "\t\t\t");
};
Cmds._convert_inst = "生成类型转换的代码";
Cmds.convert_inst = function () {
    let file = xlsx.readFile(path.resolve(__dirname, "../doc/FayLang.xlsx"));
    let typeData = xlsx.utils.sheet_to_json(file.Sheets['ValueType']);
    let typeMap = new Map();
    let instData = xlsx.utils.sheet_to_json(file.Sheets['Inst']);
    let instMap = new Map(); //用于记录当前有效的语句
    typeData.forEach((it) => {
        typeMap.set(it.Name, true);
    });
    //记录所有有效的指令
    instData.forEach((it) => {
        if (it.Action && it.Action.trim().length > 0)
            instMap.set(it.Name, true);
    });
    //生成类型转换的语句
    let convertText = "switch(src)\n{";
    typeData.forEach((it1) => {
        convertText += "\ncase ValueType::" + it1.Name + ":";
        convertText += "\n\tswitch(dest)\n\t{";
        typeData.forEach((it2) => {
            let instName = it1.Name + "To" + it2.Name;
            if (!instMap.get(instName))
                return;
            convertText += "\n\tcase ValueType::" + it2.Name + ":";
            convertText += "\n\t\treturn new inst::" + instName + "();";
        });
        convertText += "\n\t}\n\tbreak;";
    });
    convertText += "\n}";
    replaceFileBody("src/fay_lang.cpp", "ConvertInst", convertText, "\t");
    //生成按操作类型处理的语句
    let opText = "switch(op)\n{";
    let nowCode1 = "";
    instData.forEach((it) => {
        //如果第二部分是一个类型
        if (typeMap.get(it.Code2) && instMap.get(it.Name) && (!it.Params || it.Params.trim().length <= 0)) {
            if (nowCode1 != it.Code1) {
                //如果之前有值，需要结束
                if (nowCode1)
                    opText += "\n\t}\n\tbreak;";
                nowCode1 = it.Code1;
                opText += "\ncase InstGroupType::" + it.Code1 + ":";
                opText += "\n\tswitch(type)\n\t{";
            }
            opText += "\n\tcase ValueType::" + it.Code2 + ":";
            opText += "\n\t\treturn new inst::" + it.Name + "();";
            //opText += larlf.text.format("if (op == InstGroupType::{0} && type == ValueType::{1})", it.Code1, it.Code2);
            //opText += larlf.text.format("\n\treturn new inst::{0}();", it.Name);
        }
    });
    if (nowCode1)
        opText += "\n\t}";
    opText += "\n}";
    // let ops = ["Minus", "Add", "Sub", "Mul", "Div", "Equal", "Greater", "Less", "Complement"];
    // ops.forEach(op =>
    // {
    // 	typeData.forEach((it: any) =>
    // 	{
    // 		if (opText)
    // 			opText += "\nelse ";
    // 		opText += larlf.text.format("if (op == InstGroupType::{0} && type == ValueType::{1})", op, it.Name);
    // 		opText += larlf.text.format("\n\treturn new inst::{0}{1}();", op, it.Name);
    // 	});
    // });
    replaceFileBody("src/fay_lang.cpp", "OPInst", opText, "\t");
};
Cmds._deps = "处理依赖关系";
Cmds.deps = function () {
    //检查是不是有mirage项目
    let mirageDir = path.resolve(__dirname, "../../mirage/cpp/");
    if (!fs.existsSync(mirageDir)) {
        log.error("Cannot find mirage path : " + mirageDir);
        return;
    }
    larlf.file.copyFiles(path.resolve(mirageDir, "src/mirage"), "*.h", path.resolve(__dirname, "../deps/win64/mirage/inc/mirage"));
    larlf.file.copyFiles(path.resolve(mirageDir, "build/bin/Debug"), "mirage.lib", path.resolve(__dirname, "../deps/win64/mirage/lib/"));
};
main();
//# sourceMappingURL=index.js.map