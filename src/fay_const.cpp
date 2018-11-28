﻿#include <fay_const.h>

using namespace fay;

MAP<fay::TokenType, std::string> fay::TypeDict::TokenTypeName;
MAP<fay::ValueType, std::string> fay::TypeDict::ValueTypeName;
MAP<fay::InstType, std::string> fay::TypeDict::InstTypeName;
MAP<std::string, fay::ValueType> fay::TypeDict::ValueTypeMap;

void fay::TypeDict::Init()
{
	//TokenTypeNameStart
	TypeDict::TokenTypeName[TokenType::None] = "None";
	TypeDict::TokenTypeName[TokenType::Colon] = "Colon";
	TypeDict::TokenTypeName[TokenType::Semicolon] = "Semicolon";
	TypeDict::TokenTypeName[TokenType::Comma] = "Comma";
	TypeDict::TokenTypeName[TokenType::LeftBrace] = "LeftBrace";
	TypeDict::TokenTypeName[TokenType::RightBrace] = "RightBrace";
	TypeDict::TokenTypeName[TokenType::OP] = "OP";
	TypeDict::TokenTypeName[TokenType::DoubleSlash] = "DoubleSlash";
	TypeDict::TokenTypeName[TokenType::Comment] = "Comment";
	TypeDict::TokenTypeName[TokenType::Using] = "Using";
	TypeDict::TokenTypeName[TokenType::Package] = "Package";
	TypeDict::TokenTypeName[TokenType::Var] = "Var";
	TypeDict::TokenTypeName[TokenType::Fun] = "Fun";
	TypeDict::TokenTypeName[TokenType::Class] = "Class";
	TypeDict::TokenTypeName[TokenType::Interface] = "Interface";
	TypeDict::TokenTypeName[TokenType::DescSymbol] = "DescSymbol";
	TypeDict::TokenTypeName[TokenType::SystemName] = "SystemName";
	TypeDict::TokenTypeName[TokenType::Goto] = "Goto";
	TypeDict::TokenTypeName[TokenType::Label] = "Label";
	TypeDict::TokenTypeName[TokenType::If] = "If";
	TypeDict::TokenTypeName[TokenType::Else] = "Else";
	TypeDict::TokenTypeName[TokenType::ElseIf] = "ElseIf";
	TypeDict::TokenTypeName[TokenType::For] = "For";
	TypeDict::TokenTypeName[TokenType::Foreach] = "Foreach";
	TypeDict::TokenTypeName[TokenType::While] = "While";
	TypeDict::TokenTypeName[TokenType::Do] = "Do";
	TypeDict::TokenTypeName[TokenType::Return] = "Return";
	TypeDict::TokenTypeName[TokenType::ID] = "ID";
	TypeDict::TokenTypeName[TokenType::BasicType] = "BasicType";
	TypeDict::TokenTypeName[TokenType::Number] = "Number";
	TypeDict::TokenTypeName[TokenType::String] = "String";
	TypeDict::TokenTypeName[TokenType::Char] = "Char";
	TypeDict::TokenTypeName[TokenType::Bool] = "Bool";
	//TokenTypeNameEnd

	//ValueTypeNameStart
	TypeDict::ValueTypeName[ValueType::Void] = "void";
	TypeDict::ValueTypeName[ValueType::Bool] = "bool";
	TypeDict::ValueTypeName[ValueType::Byte] = "byte";
	TypeDict::ValueTypeName[ValueType::Int] = "int";
	TypeDict::ValueTypeName[ValueType::Long] = "long";
	TypeDict::ValueTypeName[ValueType::Float] = "float";
	TypeDict::ValueTypeName[ValueType::Double] = "double";
	TypeDict::ValueTypeName[ValueType::String] = "string";
	TypeDict::ValueTypeName[ValueType::Object] = "object";
	TypeDict::ValueTypeName[ValueType::Function] = "function";
	//ValueTypeNameEnd

	//ValueTypeMapStart
	TypeDict::ValueTypeMap["void"] = ValueType::Void;
	TypeDict::ValueTypeMap["bool"] = ValueType::Bool;
	TypeDict::ValueTypeMap["byte"] = ValueType::Byte;
	TypeDict::ValueTypeMap["int"] = ValueType::Int;
	TypeDict::ValueTypeMap["long"] = ValueType::Long;
	TypeDict::ValueTypeMap["float"] = ValueType::Float;
	TypeDict::ValueTypeMap["double"] = ValueType::Double;
	TypeDict::ValueTypeMap["string"] = ValueType::String;
	TypeDict::ValueTypeMap["object"] = ValueType::Object;
	TypeDict::ValueTypeMap["function"] = ValueType::Function;
	//ValueTypeMapEnd

	//InstTypeNameStart
	TypeDict::InstTypeName[InstType::Nop] = "Nop";
	TypeDict::InstTypeName[InstType::Jump] = "Jump";
	TypeDict::InstTypeName[InstType::JumpFalse] = "JumpFalse";
	TypeDict::InstTypeName[InstType::Return] = "Return";
	TypeDict::InstTypeName[InstType::PushBool] = "PushBool";
	TypeDict::InstTypeName[InstType::PushByte] = "PushByte";
	TypeDict::InstTypeName[InstType::PushInt] = "PushInt";
	TypeDict::InstTypeName[InstType::PushLong] = "PushLong";
	TypeDict::InstTypeName[InstType::PushFloat] = "PushFloat";
	TypeDict::InstTypeName[InstType::PushDouble] = "PushDouble";
	TypeDict::InstTypeName[InstType::PushString] = "PushString";
	TypeDict::InstTypeName[InstType::Pop] = "Pop";
	TypeDict::InstTypeName[InstType::PopTo] = "PopTo";
	TypeDict::InstTypeName[InstType::CallStatic] = "CallStatic";
	TypeDict::InstTypeName[InstType::CallVirtual] = "CallVirtual";
	TypeDict::InstTypeName[InstType::SetLocal] = "SetLocal";
	TypeDict::InstTypeName[InstType::SetField] = "SetField";
	TypeDict::InstTypeName[InstType::SetStatic] = "SetStatic";
	TypeDict::InstTypeName[InstType::LoadLocal] = "LoadLocal";
	TypeDict::InstTypeName[InstType::LoadField] = "LoadField";
	TypeDict::InstTypeName[InstType::LoadStatic] = "LoadStatic";
	TypeDict::InstTypeName[InstType::CopyLocal] = "CopyLocal";
	TypeDict::InstTypeName[InstType::CopyField] = "CopyField";
	TypeDict::InstTypeName[InstType::CopyStatic] = "CopyStatic";
	TypeDict::InstTypeName[InstType::VoidToVoid] = "VoidToVoid";
	TypeDict::InstTypeName[InstType::VoidToBool] = "VoidToBool";
	TypeDict::InstTypeName[InstType::VoidToByte] = "VoidToByte";
	TypeDict::InstTypeName[InstType::VoidToInt] = "VoidToInt";
	TypeDict::InstTypeName[InstType::VoidToLong] = "VoidToLong";
	TypeDict::InstTypeName[InstType::VoidToFloat] = "VoidToFloat";
	TypeDict::InstTypeName[InstType::VoidToDouble] = "VoidToDouble";
	TypeDict::InstTypeName[InstType::VoidToString] = "VoidToString";
	TypeDict::InstTypeName[InstType::VoidToObject] = "VoidToObject";
	TypeDict::InstTypeName[InstType::VoidToFunction] = "VoidToFunction";
	TypeDict::InstTypeName[InstType::BoolToVoid] = "BoolToVoid";
	TypeDict::InstTypeName[InstType::BoolToBool] = "BoolToBool";
	TypeDict::InstTypeName[InstType::BoolToByte] = "BoolToByte";
	TypeDict::InstTypeName[InstType::BoolToInt] = "BoolToInt";
	TypeDict::InstTypeName[InstType::BoolToLong] = "BoolToLong";
	TypeDict::InstTypeName[InstType::BoolToFloat] = "BoolToFloat";
	TypeDict::InstTypeName[InstType::BoolToDouble] = "BoolToDouble";
	TypeDict::InstTypeName[InstType::BoolToString] = "BoolToString";
	TypeDict::InstTypeName[InstType::BoolToObject] = "BoolToObject";
	TypeDict::InstTypeName[InstType::BoolToFunction] = "BoolToFunction";
	TypeDict::InstTypeName[InstType::ByteToVoid] = "ByteToVoid";
	TypeDict::InstTypeName[InstType::ByteToBool] = "ByteToBool";
	TypeDict::InstTypeName[InstType::ByteToByte] = "ByteToByte";
	TypeDict::InstTypeName[InstType::ByteToInt] = "ByteToInt";
	TypeDict::InstTypeName[InstType::ByteToLong] = "ByteToLong";
	TypeDict::InstTypeName[InstType::ByteToFloat] = "ByteToFloat";
	TypeDict::InstTypeName[InstType::ByteToDouble] = "ByteToDouble";
	TypeDict::InstTypeName[InstType::ByteToString] = "ByteToString";
	TypeDict::InstTypeName[InstType::ByteToObject] = "ByteToObject";
	TypeDict::InstTypeName[InstType::ByteToFunction] = "ByteToFunction";
	TypeDict::InstTypeName[InstType::IntToVoid] = "IntToVoid";
	TypeDict::InstTypeName[InstType::IntToBool] = "IntToBool";
	TypeDict::InstTypeName[InstType::IntToByte] = "IntToByte";
	TypeDict::InstTypeName[InstType::IntToInt] = "IntToInt";
	TypeDict::InstTypeName[InstType::IntToLong] = "IntToLong";
	TypeDict::InstTypeName[InstType::IntToFloat] = "IntToFloat";
	TypeDict::InstTypeName[InstType::IntToDouble] = "IntToDouble";
	TypeDict::InstTypeName[InstType::IntToString] = "IntToString";
	TypeDict::InstTypeName[InstType::IntToObject] = "IntToObject";
	TypeDict::InstTypeName[InstType::IntToFunction] = "IntToFunction";
	TypeDict::InstTypeName[InstType::LongToVoid] = "LongToVoid";
	TypeDict::InstTypeName[InstType::LongToBool] = "LongToBool";
	TypeDict::InstTypeName[InstType::LongToByte] = "LongToByte";
	TypeDict::InstTypeName[InstType::LongToInt] = "LongToInt";
	TypeDict::InstTypeName[InstType::LongToLong] = "LongToLong";
	TypeDict::InstTypeName[InstType::LongToFloat] = "LongToFloat";
	TypeDict::InstTypeName[InstType::LongToDouble] = "LongToDouble";
	TypeDict::InstTypeName[InstType::LongToString] = "LongToString";
	TypeDict::InstTypeName[InstType::LongToObject] = "LongToObject";
	TypeDict::InstTypeName[InstType::LongToFunction] = "LongToFunction";
	TypeDict::InstTypeName[InstType::FloatToVoid] = "FloatToVoid";
	TypeDict::InstTypeName[InstType::FloatToBool] = "FloatToBool";
	TypeDict::InstTypeName[InstType::FloatToByte] = "FloatToByte";
	TypeDict::InstTypeName[InstType::FloatToInt] = "FloatToInt";
	TypeDict::InstTypeName[InstType::FloatToLong] = "FloatToLong";
	TypeDict::InstTypeName[InstType::FloatToFloat] = "FloatToFloat";
	TypeDict::InstTypeName[InstType::FloatToDouble] = "FloatToDouble";
	TypeDict::InstTypeName[InstType::FloatToString] = "FloatToString";
	TypeDict::InstTypeName[InstType::FloatToObject] = "FloatToObject";
	TypeDict::InstTypeName[InstType::FloatToFunction] = "FloatToFunction";
	TypeDict::InstTypeName[InstType::DoubleToVoid] = "DoubleToVoid";
	TypeDict::InstTypeName[InstType::DoubleToBool] = "DoubleToBool";
	TypeDict::InstTypeName[InstType::DoubleToByte] = "DoubleToByte";
	TypeDict::InstTypeName[InstType::DoubleToInt] = "DoubleToInt";
	TypeDict::InstTypeName[InstType::DoubleToLong] = "DoubleToLong";
	TypeDict::InstTypeName[InstType::DoubleToFloat] = "DoubleToFloat";
	TypeDict::InstTypeName[InstType::DoubleToDouble] = "DoubleToDouble";
	TypeDict::InstTypeName[InstType::DoubleToString] = "DoubleToString";
	TypeDict::InstTypeName[InstType::DoubleToObject] = "DoubleToObject";
	TypeDict::InstTypeName[InstType::DoubleToFunction] = "DoubleToFunction";
	TypeDict::InstTypeName[InstType::StringToVoid] = "StringToVoid";
	TypeDict::InstTypeName[InstType::StringToBool] = "StringToBool";
	TypeDict::InstTypeName[InstType::StringToByte] = "StringToByte";
	TypeDict::InstTypeName[InstType::StringToInt] = "StringToInt";
	TypeDict::InstTypeName[InstType::StringToLong] = "StringToLong";
	TypeDict::InstTypeName[InstType::StringToFloat] = "StringToFloat";
	TypeDict::InstTypeName[InstType::StringToDouble] = "StringToDouble";
	TypeDict::InstTypeName[InstType::StringToString] = "StringToString";
	TypeDict::InstTypeName[InstType::StringToObject] = "StringToObject";
	TypeDict::InstTypeName[InstType::StringToFunction] = "StringToFunction";
	TypeDict::InstTypeName[InstType::ObjectToVoid] = "ObjectToVoid";
	TypeDict::InstTypeName[InstType::ObjectToBool] = "ObjectToBool";
	TypeDict::InstTypeName[InstType::ObjectToByte] = "ObjectToByte";
	TypeDict::InstTypeName[InstType::ObjectToInt] = "ObjectToInt";
	TypeDict::InstTypeName[InstType::ObjectToLong] = "ObjectToLong";
	TypeDict::InstTypeName[InstType::ObjectToFloat] = "ObjectToFloat";
	TypeDict::InstTypeName[InstType::ObjectToDouble] = "ObjectToDouble";
	TypeDict::InstTypeName[InstType::ObjectToString] = "ObjectToString";
	TypeDict::InstTypeName[InstType::ObjectToObject] = "ObjectToObject";
	TypeDict::InstTypeName[InstType::ObjectToFunction] = "ObjectToFunction";
	TypeDict::InstTypeName[InstType::FunctionToVoid] = "FunctionToVoid";
	TypeDict::InstTypeName[InstType::FunctionToBool] = "FunctionToBool";
	TypeDict::InstTypeName[InstType::FunctionToByte] = "FunctionToByte";
	TypeDict::InstTypeName[InstType::FunctionToInt] = "FunctionToInt";
	TypeDict::InstTypeName[InstType::FunctionToLong] = "FunctionToLong";
	TypeDict::InstTypeName[InstType::FunctionToFloat] = "FunctionToFloat";
	TypeDict::InstTypeName[InstType::FunctionToDouble] = "FunctionToDouble";
	TypeDict::InstTypeName[InstType::FunctionToString] = "FunctionToString";
	TypeDict::InstTypeName[InstType::FunctionToObject] = "FunctionToObject";
	TypeDict::InstTypeName[InstType::FunctionToFunction] = "FunctionToFunction";
	TypeDict::InstTypeName[InstType::MinusVoid] = "MinusVoid";
	TypeDict::InstTypeName[InstType::MinusBool] = "MinusBool";
	TypeDict::InstTypeName[InstType::MinusByte] = "MinusByte";
	TypeDict::InstTypeName[InstType::MinusInt] = "MinusInt";
	TypeDict::InstTypeName[InstType::MinusLong] = "MinusLong";
	TypeDict::InstTypeName[InstType::MinusFloat] = "MinusFloat";
	TypeDict::InstTypeName[InstType::MinusDouble] = "MinusDouble";
	TypeDict::InstTypeName[InstType::MinusString] = "MinusString";
	TypeDict::InstTypeName[InstType::MinusObject] = "MinusObject";
	TypeDict::InstTypeName[InstType::MinusFunction] = "MinusFunction";
	TypeDict::InstTypeName[InstType::AddVoid] = "AddVoid";
	TypeDict::InstTypeName[InstType::AddBool] = "AddBool";
	TypeDict::InstTypeName[InstType::AddByte] = "AddByte";
	TypeDict::InstTypeName[InstType::AddInt] = "AddInt";
	TypeDict::InstTypeName[InstType::AddLong] = "AddLong";
	TypeDict::InstTypeName[InstType::AddFloat] = "AddFloat";
	TypeDict::InstTypeName[InstType::AddDouble] = "AddDouble";
	TypeDict::InstTypeName[InstType::AddString] = "AddString";
	TypeDict::InstTypeName[InstType::AddObject] = "AddObject";
	TypeDict::InstTypeName[InstType::AddFunction] = "AddFunction";
	TypeDict::InstTypeName[InstType::SubVoid] = "SubVoid";
	TypeDict::InstTypeName[InstType::SubBool] = "SubBool";
	TypeDict::InstTypeName[InstType::SubByte] = "SubByte";
	TypeDict::InstTypeName[InstType::SubInt] = "SubInt";
	TypeDict::InstTypeName[InstType::SubLong] = "SubLong";
	TypeDict::InstTypeName[InstType::SubFloat] = "SubFloat";
	TypeDict::InstTypeName[InstType::SubDouble] = "SubDouble";
	TypeDict::InstTypeName[InstType::SubString] = "SubString";
	TypeDict::InstTypeName[InstType::SubObject] = "SubObject";
	TypeDict::InstTypeName[InstType::SubFunction] = "SubFunction";
	TypeDict::InstTypeName[InstType::MulVoid] = "MulVoid";
	TypeDict::InstTypeName[InstType::MulBool] = "MulBool";
	TypeDict::InstTypeName[InstType::MulByte] = "MulByte";
	TypeDict::InstTypeName[InstType::MulInt] = "MulInt";
	TypeDict::InstTypeName[InstType::MulLong] = "MulLong";
	TypeDict::InstTypeName[InstType::MulFloat] = "MulFloat";
	TypeDict::InstTypeName[InstType::MulDouble] = "MulDouble";
	TypeDict::InstTypeName[InstType::MulString] = "MulString";
	TypeDict::InstTypeName[InstType::MulObject] = "MulObject";
	TypeDict::InstTypeName[InstType::MulFunction] = "MulFunction";
	TypeDict::InstTypeName[InstType::DivVoid] = "DivVoid";
	TypeDict::InstTypeName[InstType::DivBool] = "DivBool";
	TypeDict::InstTypeName[InstType::DivByte] = "DivByte";
	TypeDict::InstTypeName[InstType::DivInt] = "DivInt";
	TypeDict::InstTypeName[InstType::DivLong] = "DivLong";
	TypeDict::InstTypeName[InstType::DivFloat] = "DivFloat";
	TypeDict::InstTypeName[InstType::DivDouble] = "DivDouble";
	TypeDict::InstTypeName[InstType::DivString] = "DivString";
	TypeDict::InstTypeName[InstType::DivObject] = "DivObject";
	TypeDict::InstTypeName[InstType::DivFunction] = "DivFunction";
	TypeDict::InstTypeName[InstType::EqualVoid] = "EqualVoid";
	TypeDict::InstTypeName[InstType::EqualBool] = "EqualBool";
	TypeDict::InstTypeName[InstType::EqualByte] = "EqualByte";
	TypeDict::InstTypeName[InstType::EqualInt] = "EqualInt";
	TypeDict::InstTypeName[InstType::EqualLong] = "EqualLong";
	TypeDict::InstTypeName[InstType::EqualFloat] = "EqualFloat";
	TypeDict::InstTypeName[InstType::EqualDouble] = "EqualDouble";
	TypeDict::InstTypeName[InstType::EqualString] = "EqualString";
	TypeDict::InstTypeName[InstType::EqualObject] = "EqualObject";
	TypeDict::InstTypeName[InstType::EqualFunction] = "EqualFunction";
	TypeDict::InstTypeName[InstType::NotEqualVoid] = "NotEqualVoid";
	TypeDict::InstTypeName[InstType::NotEqualBool] = "NotEqualBool";
	TypeDict::InstTypeName[InstType::NotEqualByte] = "NotEqualByte";
	TypeDict::InstTypeName[InstType::NotEqualInt] = "NotEqualInt";
	TypeDict::InstTypeName[InstType::NotEqualLong] = "NotEqualLong";
	TypeDict::InstTypeName[InstType::NotEqualFloat] = "NotEqualFloat";
	TypeDict::InstTypeName[InstType::NotEqualDouble] = "NotEqualDouble";
	TypeDict::InstTypeName[InstType::NotEqualString] = "NotEqualString";
	TypeDict::InstTypeName[InstType::NotEqualObject] = "NotEqualObject";
	TypeDict::InstTypeName[InstType::NotEqualFunction] = "NotEqualFunction";
	TypeDict::InstTypeName[InstType::GreaterVoid] = "GreaterVoid";
	TypeDict::InstTypeName[InstType::GreaterBool] = "GreaterBool";
	TypeDict::InstTypeName[InstType::GreaterByte] = "GreaterByte";
	TypeDict::InstTypeName[InstType::GreaterInt] = "GreaterInt";
	TypeDict::InstTypeName[InstType::GreaterLong] = "GreaterLong";
	TypeDict::InstTypeName[InstType::GreaterFloat] = "GreaterFloat";
	TypeDict::InstTypeName[InstType::GreaterDouble] = "GreaterDouble";
	TypeDict::InstTypeName[InstType::GreaterString] = "GreaterString";
	TypeDict::InstTypeName[InstType::GreaterObject] = "GreaterObject";
	TypeDict::InstTypeName[InstType::GreaterFunction] = "GreaterFunction";
	TypeDict::InstTypeName[InstType::GreaterEqualVoid] = "GreaterEqualVoid";
	TypeDict::InstTypeName[InstType::GreaterEqualBool] = "GreaterEqualBool";
	TypeDict::InstTypeName[InstType::GreaterEqualByte] = "GreaterEqualByte";
	TypeDict::InstTypeName[InstType::GreaterEqualInt] = "GreaterEqualInt";
	TypeDict::InstTypeName[InstType::GreaterEqualLong] = "GreaterEqualLong";
	TypeDict::InstTypeName[InstType::GreaterEqualFloat] = "GreaterEqualFloat";
	TypeDict::InstTypeName[InstType::GreaterEqualDouble] = "GreaterEqualDouble";
	TypeDict::InstTypeName[InstType::GreaterEqualString] = "GreaterEqualString";
	TypeDict::InstTypeName[InstType::GreaterEqualObject] = "GreaterEqualObject";
	TypeDict::InstTypeName[InstType::GreaterEqualFunction] = "GreaterEqualFunction";
	TypeDict::InstTypeName[InstType::LessVoid] = "LessVoid";
	TypeDict::InstTypeName[InstType::LessBool] = "LessBool";
	TypeDict::InstTypeName[InstType::LessByte] = "LessByte";
	TypeDict::InstTypeName[InstType::LessInt] = "LessInt";
	TypeDict::InstTypeName[InstType::LessLong] = "LessLong";
	TypeDict::InstTypeName[InstType::LessFloat] = "LessFloat";
	TypeDict::InstTypeName[InstType::LessDouble] = "LessDouble";
	TypeDict::InstTypeName[InstType::LessString] = "LessString";
	TypeDict::InstTypeName[InstType::LessObject] = "LessObject";
	TypeDict::InstTypeName[InstType::LessFunction] = "LessFunction";
	TypeDict::InstTypeName[InstType::LessEqualVoid] = "LessEqualVoid";
	TypeDict::InstTypeName[InstType::LessEqualBool] = "LessEqualBool";
	TypeDict::InstTypeName[InstType::LessEqualByte] = "LessEqualByte";
	TypeDict::InstTypeName[InstType::LessEqualInt] = "LessEqualInt";
	TypeDict::InstTypeName[InstType::LessEqualLong] = "LessEqualLong";
	TypeDict::InstTypeName[InstType::LessEqualFloat] = "LessEqualFloat";
	TypeDict::InstTypeName[InstType::LessEqualDouble] = "LessEqualDouble";
	TypeDict::InstTypeName[InstType::LessEqualString] = "LessEqualString";
	TypeDict::InstTypeName[InstType::LessEqualObject] = "LessEqualObject";
	TypeDict::InstTypeName[InstType::LessEqualFunction] = "LessEqualFunction";
	//InstTypeNameEnd
}

const std::string &fay::TypeDict::ToName(TokenType value)
{
	return TypeDict::TokenTypeName[value];
}

const std::string & fay::TypeDict::ToName(ValueType value)
{
	return TypeDict::ValueTypeName[value];
}

const std::string & fay::TypeDict::ToName(InstType value)
{
	return TypeDict::InstTypeName[value];
}

const ValueType fay::TypeDict::ToValueType(const std::string & name)
{
	auto it = TypeDict::ValueTypeMap.find(name);
	if (it != TypeDict::ValueTypeMap.end())
		return it->second;

	return ValueType::Void;
}
