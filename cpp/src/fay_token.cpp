#include "fay_token.h"

std::string fay::Token::toString()
{
	std::string str;
	//str.append(this->_type);
	str.append(TypeDict::ToName(this->_type));
	str.append(" ");
	str.append(this->_text);
	return str;
}

PTR(Token) fay::CharTokenRule::match(PTR(ByteData) data, int pos, int line, int col)
{


	return MKPTR(Token)();
}
