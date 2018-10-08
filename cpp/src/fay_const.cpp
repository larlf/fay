#include <fay_const.h>

MAP<fay::TokenType, std::string> fay::TypeDict::TokenTypeName;

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
}

std::string fay::TypeDict::ToName(TokenType value)
{
	return TypeDict::TokenTypeName[value];
}
