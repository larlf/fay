/// <reference path="typings/index.d.ts" />

import * as ejs from 'ejs';
import * as fs from "fs";
import * as fs_extra from 'fs-extra';
import * as larlf from 'larlf';
import * as os from "os";
import * as path from 'path';
import * as xlsx from 'xlsx';

let log = larlf.log;

let Cmds: any = {};
let RootPath = path.resolve(__dirname, "..");
let TypeMap: { [key: string]: string } = {
	"int": "int",
	"byte": "byte",
	"long": "int64",
	"float": "float",
	"bool": "char",
	"double": "double"
};

class FayCfg
{
	public dataType: Map<string, any> = new Map();

	constructor()
	{
		let file = xlsx.readFile(path.resolve(__dirname, "../doc/FayLang.xlsx"));
		let json = xlsx.utils.sheet_to_json(file.Sheets['ValueType']);

		for (let key in json)
		{
			let it: any = json[key];
			this.dataType.set(it.Name.toString().toLowerCase(), it);
		}
	}

	public getType(name: string): any
	{
		let it = this.dataType.get(name);
		if (!it)
			log.error("Cannot find type : " + name);

		return it;
	}
}



//指令中的字段
class FayInstField
{
	public index: number;
	public name: string;
	public type: string;

	constructor(index: number, text: string)
	{
		this.index = index;

		if (text.indexOf(":") > 0)
		{
			let textArray = text.split(":");
			this.name = textArray[0];
			this.type = textArray[1];
		}
		else
		{
			this.name = "p" + this.index;
			this.type = text;
		}
	}

	//生成字段字符串
	public fieldDefStr(): string
	{
		return this.getCType() + " " + this.name + ";";
	}

	public getCType(): string
	{
		return CFG.getType(this.type).CType;
	}
}

//Fay语言指令定义
class FayInst
{
	code: number;
	name: string;
	props: FayInstField[] = [];
	values: FayInstField[] = [];

	constructor(code: number, data: any)
	{
		this.code = code;
		this.name = data.Code1 + (data.Code2 ? data.Code2 : "");

		for (let i = 1; i <= 3; ++i)
		{
			let propName = "P" + i;
			if (data[propName])
			{
				this.props.push(new FayInstField(i, data[propName]));
			}
		}

		for (let i = 1; i <= 3; ++i)
		{
			let propName = "V" + i;
			if (data[propName])
			{
				this.values.push(new FayInstField(i, data[propName]));
			}
		}
	}

	public makeHeadCode(): string
	{
		let tpl = fs.readFileSync(path.resolve(RootPath, "tools/tpl/inst_h.ejs")).toString();
		let text = ejs.render(tpl, {
			"data": this
		});

		//去除空行
		text = text.replace(/\n\s*\n/g, "\n");
		return text;
	}

	public makeCppCode(): string
	{
		let tpl = fs.readFileSync(path.resolve(RootPath, "tools/tpl/inst_c.ejs")).toString();
		let text = ejs.render(tpl, {
			"data": this
		});

		//去除空行
		text = text.replace(/\n\s*\n/g, "\n");
		return text;
	}

	public makeParamStr(): string
	{
		let str = "";
		this.props.forEach(it =>
		{
			if (str.length)
				str += ",";

			str += it.getCType() + " " + it.name;
		});

		return str;
	}

	public makeInitParamStr(): string
	{
		let str = "";

		this.props.forEach(it =>
		{
			str += ",";
			str += it.name + "(" + it.name + ")";
		});


		return str;
	}
}

////////////////////////////////////////////////////////////////////////////

let CFG = new FayCfg();

function main()
{
	let cmd = process.argv[2];
	if (cmd)
	{
		cmd = cmd.replace(/\./g, "_");

		if (Cmds[cmd])
			Cmds[cmd]();
		else
			log.error("Cannot find cmd : " + cmd);
	}
}

Cmds._help = "帮助信息";
Cmds.help = function()
{
	for (let cmd in Cmds)
	{
		if (!cmd.startsWith('_'))
		{
			console.log((cmd + "").yellow.bold);
			let comment = Cmds['_' + cmd];
			if (comment)
				console.log("\t" + comment);
		}
	}
}

Cmds._run = "运行的所有";
Cmds.run = function()
{
	Cmds.token_type();
	Cmds.value_type();
	Cmds.inst();
}

Cmds._token_type = "生成Token类型的数据";
Cmds.token_type = function()
{
	let file = xlsx.readFile(path.resolve(__dirname, "../doc/FayLang.xlsx"));
	let json = xlsx.utils.sheet_to_json(file.Sheets['TokenType']);
	console.log(json);

	let str1 = "";
	let str2 = "";
	for (let i = 0; i < json.length; ++i)
	{
		let it: any = json[i];
		if (it.Code)
		{
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

	replaceFileBody("cpp/src/fay_const.h", "TokenType", str1, "\t\t");
	replaceFileBody("cpp/src/fay_const.cpp", "TokenTypeName", str2, "\t");
}

function replaceFileBody(filename: string, keyword: string, str: string, indent: string)
{
	filename = path.resolve(RootPath, filename);
	let text = fs.readFileSync(filename).toString();
	text = larlf.text.replaceBlock(text, new RegExp(keyword + "Start", "g"), new RegExp(keyword + "End", "g"), str, indent);
	log.debug("Write : " + filename);
	fs.writeFileSync(filename, text);
}

Cmds._ast_type = "生成AST的类型数据";
Cmds.ast_type = function()
{
	let file = xlsx.readFile(path.resolve(__dirname, "../doc/FayLang.xlsx"));
	let json = xlsx.utils.sheet_to_json(file.Sheets['ASTType']);
	log.info("Parse AST Type");

	let str1 = "", str2 = "";
	let index = 1;
	for (let i = 0; i < json.length; ++i)
	{
		let it = json[i];
		let code = it["Code"];
		let comment = it["Comment"];
		if (code)
		{
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
}

Cmds.value_type = function()
{
	let file = xlsx.readFile(path.resolve(__dirname, "../doc/FayLang.xlsx"));
	let json = xlsx.utils.sheet_to_json(file.Sheets['ValueType']);
	log.info("Prase Value Type");

	let str1 = "", str2 = "", str3 = "";
	let index = 1;
	for (let i = 0; i < json.length; ++i)
	{
		let it: any = json[i];
		log.debug("Value Type : " + it['Name']);

		if (it['Name'])
		{
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

	replaceFileBody("cpp/src/fay_const.h", "ValueType", str1, "\t\t");
	replaceFileBody("cpp/src/fay_const.cpp", "ValueTypeName", str2, "\t");
	replaceFileBody("cpp/src/fay_const.cpp", "ValueTypeMap", str3, "\t");
}


//取指令的编码
function getInstCode(codeStr: string, valueStr: string, Code1Value: Map<string, number>): number
{
	if (codeStr)
	{
		if (valueStr)
		{
			let value = parseInt(valueStr);
			if (Code1Value.has(codeStr) && Code1Value.get(codeStr) != value)
				log.error("Diff code value : " + codeStr + " = " + Code1Value.get(codeStr) + " or " + value);
			Code1Value.set(codeStr, value);
			return value;
		}
		else
		{
			if (!Code1Value.has(codeStr))
				log.error("Cannot find code value : " + codeStr);
			else
				return Code1Value.get(codeStr);
		}
	}

	return 0;
}

Cmds.inst = function()
{
	let file = xlsx.readFile(path.resolve(__dirname, "../doc/FayLang.xlsx"));
	let json = xlsx.utils.sheet_to_json(file.Sheets['Inst']);

	//用于处理Code到Value的转换
	let Code1Value: Map<string, number> = new Map();
	let Code2Value: Map<string, number> = new Map();

	let hText = "";
	let cppText = "";
	let typeText = "";

	for (let i = 0; i < json.length; ++i)
	{
		let it: any = json[i];
		//log.dump(it);

		let value1 = getInstCode(it.Code1, it.Value1, Code1Value);
		let value2 = getInstCode(it.Code2, it.Value2, Code2Value);

		//检查限制，需要在4个字节以内
		if (value2 < 0 || value2 >= 16)
			log.error("Bad value2 code : " + value2);

		if (it.Code1 && !it.Disabled)
		{
			let inst = new FayInst((value1 << 4) + value2, it);
			hText += inst.makeHeadCode();
			cppText += inst.makeCppCode();
			typeText += (typeText ? "\n" : "") + inst.name + " = " + inst.code + ",";
		}
	}

	replaceFileBody("cpp/src/fay_inst.h", "Inst", hText, "\t\t");
	replaceFileBody("cpp/src/fay_inst.cpp", "Inst", cppText, "");
	replaceFileBody("cpp/src/fay_const.h", "InstType", typeText, "\t\t");
}

Cmds.parse = function()
{
	if (os.platform() == "win32")
	{
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
	else
	{
		// bison -d -o fay_parser.cpp fay.y
		let cmd = "bison -d -o fay_parser.cpp fay.y";
		larlf.project.execCmd(cmd, path.resolve(RootPath, "src"));
		// flex -o fay_tokens.cpp fay.l
		cmd = "flex -o fay_tokens.cpp fay.l";
		larlf.project.execCmd(cmd, path.resolve(RootPath, "src"));
	}
};

Cmds.build_vs = function()
{
	let task = new larlf.project.VSTask(path.resolve(RootPath, "build/fay.sln"));
	task.projectName = "fay";
	task.configName = "Debug";
	task.build();
}

Cmds._deps = "处理依赖关系";
Cmds.deps = function()
{
	//检查是不是有mirage项目
	let mirageDir = path.resolve(__dirname, "../../mirage/cpp/");
	if (!fs.existsSync(mirageDir))
	{
		log.error("Cannot find mirage path : " + mirageDir);
		return;
	}

	larlf.file.copyFiles(path.resolve(mirageDir, "src/mirage"), "*.h", path.resolve(__dirname, "../deps/win64/mirage/inc/mirage"));
	larlf.file.copyFiles(path.resolve(mirageDir, "build/bin/Debug"), "mirage.lib", path.resolve(__dirname, "../deps/win64/mirage/lib/"));
}

main();

