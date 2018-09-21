#include "fay_lang_const.h"
#include <string>
#include <map>

using namespace fay::lang;

string fay::lang::VarValue::toString()
{
	switch (this->type)
	{
	case VAL_Int:
		return to_string(this->value.intValue);
	case VAL_Long:
		return to_string(this->value.longValue);
	}

	return "null";
}

MAP<fay::lang::ASTType, string> fay::lang::FayConst::ASTTypeName;

MAP<fay::lang::InstructType, string> fay::lang::FayConst::InstructTypeName;

MAP<fay::lang::ValueType, string> fay::lang::FayConst::ValueTypeName;

MAP<string, ValueType> fay::lang::FayConst::ValueTypeValue;

MAP<fay::lang::PSTType, std::string> fay::lang::FayConst::PSTTypeName;

fay::lang::FayConst::FayConstInit fay::lang::FayConst::__init;

fay::lang::FayConst::FayConstInit::FayConstInit()
{
	//ValueTypeName_Init_Start
	FayConst::ValueTypeName[VAL_Void] = "Void";
	FayConst::ValueTypeName[VAL_Byte] = "Byte";
	FayConst::ValueTypeName[VAL_Int] = "Int";
	FayConst::ValueTypeName[VAL_Long] = "Long";
	FayConst::ValueTypeName[VAL_Float] = "Float";
	FayConst::ValueTypeName[VAL_Double] = "Double";
	FayConst::ValueTypeName[VAL_Bool] = "Bool";
	FayConst::ValueTypeName[VAL_String] = "String";
	FayConst::ValueTypeName[VAL_Object] = "Object";
	FayConst::ValueTypeName[VAL_Function] = "Function";
	//ValueTypeName_Init_End

	//ValueTypeValue_Init_Start
	FayConst::ValueTypeValue["Void"] = VAL_Void;
	FayConst::ValueTypeValue["void"] = VAL_Void;
	FayConst::ValueTypeValue["Byte"] = VAL_Byte;
	FayConst::ValueTypeValue["byte"] = VAL_Byte;
	FayConst::ValueTypeValue["Int"] = VAL_Int;
	FayConst::ValueTypeValue["int"] = VAL_Int;
	FayConst::ValueTypeValue["Long"] = VAL_Long;
	FayConst::ValueTypeValue["long"] = VAL_Long;
	FayConst::ValueTypeValue["Float"] = VAL_Float;
	FayConst::ValueTypeValue["float"] = VAL_Float;
	FayConst::ValueTypeValue["Double"] = VAL_Double;
	FayConst::ValueTypeValue["double"] = VAL_Double;
	FayConst::ValueTypeValue["Bool"] = VAL_Bool;
	FayConst::ValueTypeValue["bool"] = VAL_Bool;
	FayConst::ValueTypeValue["String"] = VAL_String;
	FayConst::ValueTypeValue["string"] = VAL_String;
	FayConst::ValueTypeValue["Object"] = VAL_Object;
	FayConst::ValueTypeValue["object"] = VAL_Object;
	FayConst::ValueTypeValue["Function"] = VAL_Function;
	FayConst::ValueTypeValue["function"] = VAL_Function;
	//ValueTypeValue_Init_End

	//ASTTypeName_Init_Start
	FayConst::ASTTypeName[AST_Text] = "Text";
	FayConst::ASTTypeName[AST_File] = "File";
	FayConst::ASTTypeName[AST_ID] = "ID";
	FayConst::ASTTypeName[AST_IntValue] = "IntValue";
	FayConst::ASTTypeName[AST_LongValue] = "LongValue";
	FayConst::ASTTypeName[AST_FloatValue] = "FloatValue";
	FayConst::ASTTypeName[AST_DoubleValue] = "DoubleValue";
	FayConst::ASTTypeName[AST_StringValue] = "StringValue";
	FayConst::ASTTypeName[AST_Expr1Left] = "Expr1Left";
	FayConst::ASTTypeName[AST_Expr1Right] = "Expr1Right";
	FayConst::ASTTypeName[AST_Expr2] = "Expr2";
	FayConst::ASTTypeName[AST_Expr3] = "Expr3";
	FayConst::ASTTypeName[AST_Call] = "Call";
	FayConst::ASTTypeName[AST_Params] = "Params";
	FayConst::ASTTypeName[AST_VarStmt] = "VarStmt";
	FayConst::ASTTypeName[AST_CallStmt] = "CallStmt";
	FayConst::ASTTypeName[AST_AssignStmt] = "AssignStmt";
	//ASTTypeName_Init_End

	//PSTTypeName_Init_Start
	FayConst::PSTTypeName[PST_Empty] = "Empty";
	FayConst::PSTTypeName[PST_ID] = "ID";
	FayConst::PSTTypeName[PST_FixedValue] = "FixedValue";
	FayConst::PSTTypeName[PST_Expr] = "Expr";
	FayConst::PSTTypeName[PST_Expr1Left] = "Expr1Left";
	FayConst::PSTTypeName[PST_Expr1Right] = "Expr1Right";
	FayConst::PSTTypeName[PST_Expr2] = "Expr2";
	FayConst::PSTTypeName[PST_Expr3] = "Expr3";
	FayConst::PSTTypeName[PST_Call] = "Call";
	FayConst::PSTTypeName[PST_Params] = "Params";
	FayConst::PSTTypeName[PST_Assign] = "Assign";
	//PSTTypeName_Init_End

	//InstructTypeName_Init_Start
	FayConst::InstructTypeName[INS_PushByte] = "PushByte";
	FayConst::InstructTypeName[INS_PushInt] = "PushInt";
	FayConst::InstructTypeName[INS_PushLong] = "PushLong";
	FayConst::InstructTypeName[INS_PushFloat] = "PushFloat";
	FayConst::InstructTypeName[INS_PushDouble] = "PushDouble";
	FayConst::InstructTypeName[INS_PushBool] = "PushBool";
	FayConst::InstructTypeName[INS_PushString] = "PushString";
	FayConst::InstructTypeName[INS_Pop] = "Pop";
	FayConst::InstructTypeName[INS_PopTo] = "PopTo";
	FayConst::InstructTypeName[INS_IntToByte] = "IntToByte";
	FayConst::InstructTypeName[INS_IntToInt] = "IntToInt";
	FayConst::InstructTypeName[INS_IntToLong] = "IntToLong";
	FayConst::InstructTypeName[INS_IntToFloat] = "IntToFloat";
	FayConst::InstructTypeName[INS_IntToDouble] = "IntToDouble";
	FayConst::InstructTypeName[INS_IntToBool] = "IntToBool";
	FayConst::InstructTypeName[INS_IntToString] = "IntToString";
	FayConst::InstructTypeName[INS_AddByte] = "AddByte";
	FayConst::InstructTypeName[INS_AddInt] = "AddInt";
	FayConst::InstructTypeName[INS_AddLong] = "AddLong";
	FayConst::InstructTypeName[INS_AddFloat] = "AddFloat";
	FayConst::InstructTypeName[INS_AddDouble] = "AddDouble";
	FayConst::InstructTypeName[INS_AddBool] = "AddBool";
	FayConst::InstructTypeName[INS_AddString] = "AddString";
	FayConst::InstructTypeName[INS_SubByte] = "SubByte";
	FayConst::InstructTypeName[INS_SubInt] = "SubInt";
	FayConst::InstructTypeName[INS_SubLong] = "SubLong";
	FayConst::InstructTypeName[INS_SubFloat] = "SubFloat";
	FayConst::InstructTypeName[INS_SubDouble] = "SubDouble";
	FayConst::InstructTypeName[INS_SubBool] = "SubBool";
	FayConst::InstructTypeName[INS_SubString] = "SubString";
	//InstructTypeName_Init_End
}

ValueType fay::lang::FayUtils::StringToValueType(const string &str)
{
	auto it=FayConst::ValueTypeValue.find(str);
	if (it != FayConst::ValueTypeValue.end())
		return it->second;

	return VAL_Object;
}

std::string fay::lang::FayUtils::ValueTypeToString(ValueType type)
{
	auto it = FayConst::ValueTypeName.find(type);
	if (it != FayConst::ValueTypeName.end())
		return it->second;

	return "Bad ValueType";
}

string fay::lang::FayUtils::ASTTypeToString(ASTType type)
{
	auto it = FayConst::ASTTypeName.find(type);
	if (it != FayConst::ASTTypeName.end())
		return it->second;

	return "Bad ASTType";
}
