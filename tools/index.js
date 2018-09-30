"use strict";
/// <reference path="typings/index.d.ts" />
Object.defineProperty(exports, "__esModule", { value: true });
const os = require("os");
const fs = require("fs");
const path = require("path");
const xlsx = require("xlsx");
const larlf = require("larlf");
const fs_extra = require("fs-extra");
const ejs = require("ejs");
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
            if (str2.length > 0)
                str2 += "\n";
            str2 += "TypeDict::TokenTypeName[TokenType::" + it.Code + "] = \"" + it.Code + "\";";
        }
    }
    replaceFileBody("cpp/src/fay_const.h", "TokenType", str1, "\t\t");
    replaceFileBody("cpp/src/fay_const.cpp", "TokenTypeName", str2, "\t");
};
function replaceFileBody(filename, keyword, str, indent) {
    filename = path.resolve(RootPath, filename);
    let text = fs.readFileSync(filename).toString();
    text = larlf.text.replaceBlock(text, new RegExp(keyword + "Start", "g"), new RegExp(keyword + "End", "g"), str, indent);
    log.debug("Write : " + filename);
    fs.writeFileSync(filename, text);
}
Cmds._ast_type = "生成AST的类型数据";
Cmds.ast_type = function () {
    let file = xlsx.readFile(path.resolve(__dirname, "../doc/FayLang.xlsx"));
    let json = xlsx.utils.sheet_to_json(file.Sheets['ASTType']);
    log.info("Parse AST Type");
    let str1 = "", str2 = "";
    let index = 1;
    for (let i = 0; i < json.length; ++i) {
        let it = json[i];
        let code = it["Code"];
        let comment = it["Comment"];
        if (code) {
            log.debug("AST Code : " + code);
            str1 += str1.length > 0 ? ",\n" : "";
            str1 += "AST_" + code + " = " + index++;
            str2 += str2.length > 0 ? "\n" : "";
            str2 += larlf.text.format("FayConst::ASTTypeName[AST_{0}] = \"{0}\";", code);
        }
    }
    //生成头文件中的定义
    let filename = path.resolve(RootPath, "cpp\\src\\faylib_const.h");
    let text = fs.readFileSync(filename).toString();
    text = larlf.text.replaceBlock(text, /ASTType_Start/g, /ASTType_End/g, str1, "\t\t\t");
    log.debug("Write : " + filename);
    fs.writeFileSync(filename, text);
    //生成初始化的定义
    filename = path.resolve(RootPath, "cpp\\src\\faylib_const.cpp");
    text = fs.readFileSync(filename).toString();
    text = larlf.text.replaceBlock(text, /ASTTypeName_Init_Start/, /ASTTypeName_Init_End/, str2, "\t");
    log.debug("Write : " + filename);
    fs.writeFileSync(filename, text);
};
Cmds._pst_type = "生成PST的类型数据";
Cmds.pst_type = function () {
    let file = xlsx.readFile(path.resolve(__dirname, "../doc/FayLang.xlsx"));
    let json = xlsx.utils.sheet_to_json(file.Sheets['PSTType']);
    log.info("Parse PST Type");
    let str1 = "", str2 = "";
    let index = 1;
    for (let i = 0; i < json.length; ++i) {
        let code = json[i]['Code'];
        if (code) {
            log.debug("PST : " + code);
            str1 += str1.length ? ",\n" : "";
            str1 += "PST_" + code + " = " + index;
            index *= 2;
            str2 += str2.length ? "\n" : "";
            str2 += larlf.text.format("FayConst::PSTTypeName[PST_{0}] = \"{0}\";", code);
        }
    }
    {
        let filename = path.resolve(RootPath, "src\\fay_lang_const.h");
        let text = fs.readFileSync(filename).toString();
        text = larlf.text.replaceBlock(text, /PSTType_Start/g, /PSTType_End/g, str1, "\t\t\t");
        log.debug("Write : " + filename);
        fs.writeFileSync(filename, text);
    }
    {
        let filename = path.resolve(RootPath, "src\\fay_lang_const.cpp");
        let text = fs.readFileSync(filename).toString();
        text = larlf.text.replaceBlock(text, /PSTTypeName_Init_Start/g, /PSTTypeName_Init_End/g, str2, "\t");
        log.debug("Write : " + filename);
        fs.writeFileSync(filename, text);
    }
};
Cmds.value_type = function () {
    let file = xlsx.readFile(path.resolve(__dirname, "../doc/FayLang.xlsx"));
    let json = xlsx.utils.sheet_to_json(file.Sheets['Type']);
    log.info("Prase Value Type");
    let str1 = "", str2 = "", str3 = "";
    let index = 1;
    for (let i = 0; i < json.length; ++i) {
        let it = json[i];
        log.debug("Value Type : " + it['Name']);
        if (it['Name']) {
            if (str1.length > 0)
                str1 += ",\n";
            str1 += "VAL_" + it['Name'] + " = " + index++;
            //FayConst::ValueTypeName
            str2 += str2.length ? "\n" : "";
            str2 += larlf.text.format("FayConst::ValueTypeName[VAL_{0}] = \"{0}\";", it['Name']);
            str3 += str3.length ? "\n" : "";
            str3 += larlf.text.format("FayConst::ValueTypeValue[\"{0}\"] = VAL_{0};\n", it['Name']);
            str3 += larlf.text.format("FayConst::ValueTypeValue[\"{0}\"] = VAL_{1};", ("" + it['Name']).toLowerCase(), it['Name']);
        }
    }
    {
        let filename = path.resolve(RootPath, "src\\fay_lang_const.h");
        let text = fs.readFileSync(filename).toString();
        text = larlf.text.replaceBlock(text, /ValueType_Start/, /ValueType_End/, str1, "\t\t\t");
        log.debug("Write : " + filename);
        fs.writeFileSync(filename, text);
    }
    {
        let filename = path.resolve(RootPath, "src\\fay_lang_const.cpp");
        let text = fs.readFileSync(filename).toString();
        text = larlf.text.replaceBlock(text, /ValueTypeName_Init_Start/, /ValueTypeName_Init_End/, str2, "\t");
        text = larlf.text.replaceBlock(text, /ValueTypeValue_Init_Start/, /ValueTypeValue_Init_End/, str3, "\t");
        log.debug("Write : " + filename);
        fs.writeFileSync(filename, text);
    }
};
Cmds.instruct_type = function () {
    let file = xlsx.readFile(path.resolve(__dirname, "../doc/FayLang.xlsx"));
    let json = xlsx.utils.sheet_to_json(file.Sheets['Instruct']);
    log.info("Parse Instruction");
    let str1 = "";
    let str2 = ""; //FayConst::InstructTypeName[instruct::AddByte] = "AddByte";
    let str3 = ""; //用于生成Class
    let tpl = fs.readFileSync(path.resolve(RootPath, "tools/tpl/ins_h.ejs")).toString();
    let index = 1;
    for (let i = 0; i < json.length; ++i) {
        let it = json[i];
        if (it["Code1"]) {
            let code = it["Code1"] + (it["Code2"] ? (it["Code2"]) : "");
            log.debug("Instruction : " + code);
            str1 += str1.length > 0 ? ",\n" : "";
            str1 += "INS_" + code + " = " + index++;
            str2 += str2.length > 0 ? "\n" : "";
            str2 += larlf.text.format("FayConst::InstructTypeName[INS_{0}] = \"{0}\";", code);
            let props = [];
            for (let j = 1; j <= 5; ++j) {
                let pType = json[i]['P' + j];
                if (!pType)
                    break;
                props.push({
                    name: 'p' + j,
                    type: TypeMap[pType]
                });
            }
            str3 += ejs.render(tpl, {
                name: code,
                props: props
            });
        }
    }
    //保存到文件
    {
        let filename = path.resolve(RootPath, "src/fay_lang_const.h");
        let text = fs.readFileSync(filename).toString();
        text = larlf.text.replaceBlock(text, /InstructType_Start/, /InstructType_End/, str1, "\t\t\t");
        log.debug("Write : " + filename);
        fs.writeFileSync(filename, text);
    }
    {
        let filename = path.resolve(RootPath, "src/fay_lang_const.cpp");
        let text = fs.readFileSync(filename).toString();
        text = larlf.text.replaceBlock(text, /InstructTypeName_Init_Start/, /InstructTypeName_Init_End/, str2, "\t");
        log.debug("Write : " + filename);
        fs.writeFileSync(filename, text);
    }
    {
        let filename = path.resolve(RootPath, "src/fay_lang_ins.h");
        let text = fs.readFileSync(filename).toString();
        text = larlf.text.replaceBlock(text, /InstructionDefine_Start/, /InstructionDefine_End/, str3, "\t\t\t");
        log.debug("Write : " + filename);
        fs.writeFileSync(filename, text);
    }
    {
        // let tpl=fs.readFileSync(path.resolve(RootPath, "tools/tpl/ins_h.ejs")).toString();
        // let text=ejs.render(tpl, {
        // 	"name":"Larlf"
        // });
        // log.debug(text);
    }
};
Cmds.parse = function () {
    if (os.platform() == "win32") {
        let cmd = "..\\deps\\win32\\bin\\win_bison -v -d -l -o fay_compiler_parser.cpp fay.y";
        larlf.project.execCmd(cmd, path.resolve(RootPath, "src"));
        cmd = "..\\deps\\win32\\bin\\win_flex -o fay_compiler_tokens.cpp fay.l";
        larlf.project.execCmd(cmd, path.resolve(RootPath, "src"));
        let dir = path.resolve(RootPath, "build/bin/Debug");
        if (!fs.existsSync(dir))
            fs_extra.mkdirsSync(dir);
        larlf.project.execCmd("cmake -G \"Visual Studio 15 Win64\" ..", path.resolve(RootPath, "build"));
        larlf.file.copyFiles(path.resolve(RootPath, "deps/win32/lib"), "*.dll", dir);
        //"C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\Common7\IDE\devenv.exe" ../build/fay.sln /build Debug /project fay
        //rem ..\build\bin\Debug\fay.exe
    }
    else {
        // bison -d -o fay_parser.cpp fay.y
        let cmd = "bison -d -o fay_parser.cpp fay.y";
        larlf.project.execCmd(cmd, path.resolve(RootPath, "src"));
        // flex -o fay_tokens.cpp fay.l
        cmd = "flex -o fay_tokens.cpp fay.l";
        larlf.project.execCmd(cmd, path.resolve(RootPath, "src"));
    }
};
Cmds.build = function () {
    //生成代码
    Cmds.value_type();
    Cmds.ast_type();
    Cmds.instruct_type();
    Cmds.parse();
};
Cmds.build_vs = function () {
    let task = new larlf.project.VSTask(path.resolve(RootPath, "build/fay.sln"));
    task.projectName = "fay";
    task.configName = "Debug";
    task.build();
};
Cmds.run = function () {
    larlf.project.execCmd("bin\\Debug\\fay.exe", path.resolve(RootPath, "build"));
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