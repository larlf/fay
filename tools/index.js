"use strict";
/// <reference path="typings/index.d.ts" />
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
    constructor(code, data) {
        this.props = [];
        this.values = [];
        this.code = code;
        this.name = data.Code1 + (data.Code2 ? data.Code2 : "");
        this.action = data.Action ? data.Action.toString() : "";
        for (let i = 1; i <= 3; ++i) {
            let propName = "P" + i;
            if (data[propName]) {
                this.props.push(new FayInstField(i, data[propName]));
            }
        }
        for (let i = 1; i <= 3; ++i) {
            let propName = "V" + i;
            if (data[propName]) {
                this.values.push(new FayInstField(i, data[propName]));
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
        if (this.props[0])
            str = str.replace(/\#p1/g, "((inst::" + this.name + "*)inst)->" + this.props[0].name);
        if (this.props[1])
            str = str.replace(/\#p2/g, "((inst::" + this.name + "*)inst)->" + this.props[1].name);
        if (this.props[2])
            str = str.replace(/\#p3/g, "((inst::" + this.name + "*)inst)->" + this.props[2].name);
        if (this.values[0])
            str = str.replace(/\#v1/g, "((inst::" + this.name + "*)inst)->" + this.values[0].name);
        if (this.values[1])
            str = str.replace(/\#v2/g, "((inst::" + this.name + "*)inst)->" + this.values[1].name);
        if (this.values[2])
            str = str.replace(/\#v2/g, "((inst::" + this.name + "*)inst)->" + this.values[2].name);
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
//取指令的编码
function getInstCode(codeStr, valueStr, Code1Value) {
    if (codeStr) {
        if (valueStr) {
            let value = parseInt(valueStr);
            if (Code1Value.has(codeStr) && Code1Value.get(codeStr) != value)
                log.debug("Change '" + codeStr + "' code value from " + Code1Value.get(codeStr) + " to " + value);
            Code1Value.set(codeStr, value);
            return value;
        }
        else {
            if (!Code1Value.has(codeStr))
                log.error("Cannot find code value : " + codeStr);
            else
                return Code1Value.get(codeStr);
        }
    }
    return 0;
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
            str2 += larlf.text.format("TypeDict::ValueTypeName[ValueType::{0}] = \"{0}\";", it['Name']);
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
    //用于处理Code到Value的转换
    let Code1Value = new Map();
    let Code2Value = new Map();
    let hText = "";
    let cppText = "";
    let typeText = "";
    let caseText = "";
    let nameText = "";
    for (let i = 0; i < json.length; ++i) {
        let it = json[i];
        if (!it.Code1)
            it.Code1 = "";
        if (!it.Code2)
            it.Code2 = "";
        //log.dump(it);
        let value1 = getInstCode(it.Code1, it.Value1, Code1Value);
        let value2 = getInstCode(it.Code2, it.Value2, Code2Value);
        //检查限制，需要在4个字节以内
        if (value2 < 0 || value2 >= 16)
            log.error("Bad value2 code : " + value2);
        //只处理需要处理的语句
        if ((it.Code1 || it.Code2) && !it.Disabled) {
            let inst = new FayInst((value1 << 4) + value2, it);
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
            groupText += key + " = " + value + ",";
        }
    });
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
    let json = xlsx.utils.sheet_to_json(file.Sheets['ValueType']);
    let text = "";
    json.forEach((it1) => {
        json.forEach((it2) => {
            if (text)
                text += "\nelse ";
            text += "if (src == ValueType::" + it1.Name + " && dest == ValueType::" + it2.Name + ")\n";
            text += "\treturn new inst::" + it1.Name + "To" + it2.Name + "();";
        });
    });
    replaceFileBody("src/fay_lang.cpp", "ConvertInst", text, "\t");
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