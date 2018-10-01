#include "fay_token.h"
#include <mirror_utils_lang.h>

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
				return new Token(this->_type, data, pos, 4, line, col);
		}
		else if (data[pos + 2] == '\'')
			return new Token(this->_type, data, pos, 3, line, col);
	}

	return nullptr;
}

Token* fay::NumberTokenRule::match(ByteData &data, int pos, int line, int col)
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

Token* fay::StringTokenRule::match(ByteData &data, int pos, int line, int col)
{
	if (data[pos] == '"')
	{
		for (auto i = pos + 1; i < data.size(); ++i)
		{
			//不能换行
			if (data[i] == '\n' || data[i] == '\r')
				break;

			//字符串结束
			if (data[i] == '"' && data[i - 1] != '\\')
			{
				//返回字符串的token
				return new Token(this->_type, data, pos, i - pos + 1, line, col);
			}
		}
	}

	return nullptr;
}

fay::Token* fay::SymbolTokenRule::match(ByteData &data, int pos, int line, int col)
{

	if (data[pos] == this->_value)
		return new Token(this->_type, data, pos, 1, line, col);

	return nullptr;
}

fay::WordsTokenRule::WordsTokenRule(const std::string words[], LexMode mode, TokenType type)
	: ITokenRule(mode, type)
{
	int size = mirror::utils::LangUtils::SizeOfArray(words);
	for (auto i = 0; i < size; ++i)
		this->_words.push_back(words[i]);
}

Token * fay::WordsTokenRule::match(ByteData & data, int pos, int line, int col)
{
	for each (auto word in this->_words)
	{
		bool isMatch = true;
		for (auto i = 0; i < word.size(); ++i)
		{
			if (word[i] != data[pos + i])
			{
				isMatch = false;
				break;
			}
		}

		if (isMatch)
		{
			return new Token(this->type(), data, pos, word.size(), line, col);
		}
	}

	return nullptr;
}

fay::Token* fay::WordTokenRule::match(ByteData &data, int pos, int line, int col)
{
	for (auto i = 0; i < this->_word.size(); ++i)
	{
		if (this->_word[i] != data[pos + i])
			return nullptr;
	}

	return new Token(this->type(), data, pos, this->_word.size(), line, col);
}
