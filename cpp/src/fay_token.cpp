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

fay::Token* fay::CharTokenRule::match(ByteData &data, int pos, int line, int col)
{
	if (data[pos] == '\'' && data.size() > pos + 2)
	{
		if (data[pos + 1] == '\\')  //是否有转义字符
		{
			if (data.size() > pos + 3 && data[pos + 3] == '\'')
				return new Token(TokenType::Char, data, pos, 4, line, col);
		}
		else if (data[pos + 2] == '\'')
			return new Token(TokenType::Char, data, pos, 3, line, col);
	}

	return nullptr;
}

Token * fay::NumberTokenRule::match(ByteData & data, int pos, int line, int col)
{
	int startPos = pos;  //记录下开始位置

	//处理不符合要求的情况
	if ((data[pos] < '0' || data[pos] > '9') && data[pos] != '.')
		return nullptr;
	if (data[pos] == '.' && (data[pos + 1] < '0' || data[pos + 1] > '9'))
		return nullptr;

	bool hasDot = false;  //是否已经有.
	while (pos < data.size() && ((data[pos] >= '0' && data[pos] <= '9') || data[pos] == '.'))
	{
		if (data[pos] == '.')
		{
			if (hasDot)
				return nullptr;
			else
				hasDot = true;
		}

		pos++;
	}

	//处理结尾的描述符
	if (data[pos] == 'l' || data[pos] == 'L' || data[pos] == 'd' || data[pos] == 'D')
		pos++;

	return new Token(this->_type, data, startPos, pos - startPos + 1, line, col);
}
