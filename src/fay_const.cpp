#include <fay_const.h>

using namespace fay;

MAP<fay::TokenType, std::string> fay::TypeDict::TokenTypeName;
MAP<fay::ValueType, std::string> fay::TypeDict::ValueTypeName;
MAP<fay::InstType, std::string> fay::TypeDict::InstTypeName;
MAP<std::string, fay::ValueType> fay::TypeDict::ValueTypeMap;

void fay::TypeDict::Init()
{
	//TokenTypeNameStart
	TypeDict::TokenTypeName[TokenType::None] = "None";
	TypeDict::TokenTypeName[TokenType::Assign] = "Assign";
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
	//TokenTypeNameEnd

	//ValueTypeNameStart
	TypeDict::ValueTypeName[ValueType::Void] = "Void";
	TypeDict::ValueTypeName[ValueType::Bool] = "Bool";
	TypeDict::ValueTypeName[ValueType::Byte] = "Byte";
	TypeDict::ValueTypeName[ValueType::Int] = "Int";
	TypeDict::ValueTypeName[ValueType::Long] = "Long";
	TypeDict::ValueTypeName[ValueType::Float] = "Float";
	TypeDict::ValueTypeName[ValueType::Double] = "Double";
	TypeDict::ValueTypeName[ValueType::String] = "String";
	TypeDict::ValueTypeName[ValueType::Object] = "Object";
	TypeDict::ValueTypeName[ValueType::Function] = "Function";
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
	TypeDict::InstTypeName[InstType::PushBool] = "PushBool";
	TypeDict::InstTypeName[InstType::PushByte] = "PushByte";
	TypeDict::InstTypeName[InstType::PushInt] = "PushInt";
	TypeDict::InstTypeName[InstType::PushLong] = "PushLong";
	TypeDict::InstTypeName[InstType::PushFloat] = "PushFloat";
	TypeDict::InstTypeName[InstType::PushDouble] = "PushDouble";
	TypeDict::InstTypeName[InstType::PushString] = "PushString";
	TypeDict::InstTypeName[InstType::Pop] = "Pop";
	TypeDict::InstTypeName[InstType::PopTo] = "PopTo";
	TypeDict::InstTypeName[InstType::AddByte] = "AddByte";
	TypeDict::InstTypeName[InstType::AddInt] = "AddInt";
	TypeDict::InstTypeName[InstType::AddLong] = "AddLong";
	TypeDict::InstTypeName[InstType::AddFloat] = "AddFloat";
	TypeDict::InstTypeName[InstType::AddDouble] = "AddDouble";
	TypeDict::InstTypeName[InstType::AddBool] = "AddBool";
	TypeDict::InstTypeName[InstType::AddString] = "AddString";
	TypeDict::InstTypeName[InstType::SubByte] = "SubByte";
	TypeDict::InstTypeName[InstType::SubInt] = "SubInt";
	TypeDict::InstTypeName[InstType::SubLong] = "SubLong";
	TypeDict::InstTypeName[InstType::SubFloat] = "SubFloat";
	TypeDict::InstTypeName[InstType::SubDouble] = "SubDouble";
	TypeDict::InstTypeName[InstType::SubBool] = "SubBool";
	TypeDict::InstTypeName[InstType::SubString] = "SubString";
	TypeDict::InstTypeName[InstType::CallStatic] = "CallStatic";
	TypeDict::InstTypeName[InstType::CallVirtual] = "CallVirtual";
	TypeDict::InstTypeName[InstType::SetLocal] = "SetLocal";
	TypeDict::InstTypeName[InstType::SetField] = "SetField";
	TypeDict::InstTypeName[InstType::SetStatic] = "SetStatic";
	TypeDict::InstTypeName[InstType::LoadLocal] = "LoadLocal";
	TypeDict::InstTypeName[InstType::LoadField] = "LoadField";
	TypeDict::InstTypeName[InstType::LoadStatic] = "LoadStatic";
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
