/// <reference path="typings/index.d.ts" />

import * as ejs from 'ejs';
import * as fs from "fs";
import * as fs_extra from 'fs-extra';
import * as larlf from 'larlf';
import * as os from "os";
import * as path from 'path';
import * as xlsx from 'xlsx';
import * as SourceMapSupport from 'source-map-support';

SourceMapSupport.install();

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
	actionVars: string[] = [];
	action: string;

	constructor(code: number, data: any)
	{
		this.code = code;
		this.name = data.Code1 + (data.Code2 ? data.Code2 : "");
		this.action = data.Action ? data.Action.toString() : "";

		if (data.Params)
		{
			let list = data.Params.trim().split("\n");
			for (let i = 0; i <= list.length; ++i)
			{
				if (list[i])
				{
					let str = list[i].trim();
					this.props.push(new FayInstField(i + 1, str));
				}
			}
		}

		if (data.Values)
		{
			let list = data.Values.trim().split("\n");
			for (let i = 0; i <= list.length; ++i)
			{
				if (list[i])
				{
					let str = list[i].trim();
					this.values.push(new FayInstField(i + 1, str));
				}
			}
		}

		if (data.ActionVars)
		{
			let list = data.ActionVars.trim().split("\n");
			for (let i = 0; i <= list.length; ++i)
			{
				if (list[i])
				{
					let str = list[i].trim();
					this.actionVars.push(str);
				}
				else
				{
					this.actionVars.push("");
				}
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
		text = text.replace(/\&amp\;/g, "&");
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
		text = text.replace(/\&amp\;/g, "&");
		return text;
	}

	public makeParamStr(): string
	{
		let str = "";
		this.props.forEach(it =>
		{
			if (str.length)
				str += ",";

			if (it.type == "string")
				str += "const std::string &" + it.name;
			else
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

	public makeCaseCode(): string
	{
		//只生成有代码的
		if (!this.action)
			return "";

		let str = this.action;

		//替换属性
		for (let i = 0; i < this.props.length; ++i)
		{
			let it = this.props[i];
			if (it)
			{
				let reg1 = new RegExp("\\#p" + it.index, "g");
				str = str.replace(reg1, "((inst::" + this.name + "*)inst)->" + it.name);
			}
		}

		//替换值
		for (let i = 0; i < this.values.length; ++i)
		{
			let it = this.values[i];
			if (it)
			{
				let reg1 = new RegExp("\\#v" + it.index, "g");
				str = str.replace(reg1, "((inst::" + this.name + "*)inst)->" + it.name);
			}
		}

		//替换变量
		for (let i = 0; i < this.actionVars.length; ++i)
		{
			let it = this.actionVars[i].trim();
			if (it)
			{
				let reg1 = new RegExp("\\#a" + (i + 1), "g");
				str = str.replace(reg1, it);
			}
		}

		str = str.replace(/\#s/g, "this->stack");
		str = str.replace(/\#val/g, "FayValue");
		str = str.replace(/\#new/g, "FayValue");
		str = str.replace(/\n/g, "\n\t");  //缩进
		str = "case InstType::" + this.name + ":\n{" + (str ? "\n\t" + str : str) + "\n\tbreak;\n}";
		return str;
	}

	public makeNameCode(): string
	{
		return larlf.text.format("TypeDict::InstTypeName[InstType::{0}] = \"{0}\";", this.name);
	}
}

function replaceFileBody(filename: string, keyword: string, str: string, indent: string)
{
	filename = path.resolve(RootPath, filename);
	let text = fs.readFileSync(filename).toString();
	text = larlf.text.replaceBlock(text, new RegExp(keyword + "Start", "g"), new RegExp(keyword + "End", "g"), str, indent);
	log.debug("Write : " + filename);
	fs.writeFileSync(filename, text);
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
				log.debug("Change '" + codeStr + "' code value from " + Code1Value.get(codeStr) + " to " + value);
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
	Cmds.convert_inst();
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

	replaceFileBody("src/fay_const.h", "TokenType", str1, "\t\t");
	replaceFileBody("src/fay_const.cpp", "TokenTypeName", str2, "\t");
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
			str2 += larlf.text.format("TypeDict::ValueTypeName[ValueType::{0}] = \"{1}\";", it['Name'], (it['Name']+"").toLowerCase());

			str3 += str3.length ? "\n" : "";
			str3 += larlf.text.format("TypeDict::ValueTypeMap[\"{0}\"] = ValueType::{1};", (it.Name + "").toLowerCase(), it.Name);
		}
	}

	replaceFileBody("src/fay_const.h", "ValueType", str1, "\t\t");
	replaceFileBody("src/fay_const.cpp", "ValueTypeName", str2, "\t");
	replaceFileBody("src/fay_const.cpp", "ValueTypeMap", str3, "\t");
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
	let caseText = "";
	let nameText = "";
	for (let i = 0; i < json.length; ++i)
	{
		let it: any = json[i];
		if (!it.Code1) it.Code1 = "";
		if (!it.Code2) it.Code2 = "";
		//log.dump(it);

		let value1 = getInstCode(it.Code1, it.Value1, Code1Value);
		let value2 = getInstCode(it.Code2, it.Value2, Code2Value);

		//检查限制，需要在4个字节以内
		if (value2 < 0 || value2 >= 16)
			log.error("Bad value2 code : " + value2);

		//只处理需要处理的语句
		if ((it.Code1 || it.Code2) && !it.Disabled)
		{
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
	Code1Value.forEach((value, key) =>
	{
		if (key)
		{
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
}

Cmds._convert_inst = "生成类型转换的代码";
Cmds.convert_inst = function()
{
	let file = xlsx.readFile(path.resolve(__dirname, "../doc/FayLang.xlsx"));
	let json = xlsx.utils.sheet_to_json(file.Sheets['ValueType']);

	let convertText = "";
	json.forEach((it1: any) =>
	{
		json.forEach((it2: any) =>
		{
			if (convertText)
				convertText += "\nelse ";

			convertText += "if (src == ValueType::" + it1.Name + " && dest == ValueType::" + it2.Name + ")\n";
			convertText += "\treturn new inst::" + it1.Name + "To" + it2.Name + "();"
		});
	});

	replaceFileBody("src/fay_lang.cpp", "ConvertInst", convertText, "\t");

	let ops = ["Add", "Sub", "Mul", "Div", "Equal", "Greater", "Less"];
	let opText = "";
	ops.forEach(op =>
	{
		json.forEach((it: any) =>
		{
			if (opText)
				opText += "\nelse ";

			opText += larlf.text.format("if (op == InstGroupType::{0} && type == ValueType::{1})", op, it.Name);
			opText += larlf.text.format("\n\treturn new inst::{0}{1}();", op, it.Name);
		});
	});

	replaceFileBody("src/fay_lang.cpp", "OPInst", opText, "\t");
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

