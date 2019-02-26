#include <fay_token.h>
#include <stdio.h>
#include <string.h>

using namespace mirror;
using namespace mirror::data;
using namespace fay;

std::string fay::Token::toString()
{
	std::string text = utils::StringUtils::EncodeSpecialChar(this->text);
	int size = text.size() + 32;
	PTR(char) buffer(new char[size], std::default_delete<char[]>());

	sprintf_s(&*buffer, size, "%-5i %-5i %-16s  %s",
		this->line,
		this->col,
		TypeDict::ToName(this->type).c_str(),
		text.c_str());
	return std::string(&*buffer);
}

fay::Token* fay::CharTokenRule::match(ByteData &data, int pos, int line, int col)
{
	if(data[pos] == '\'' && data.size() > pos + 2)
	{
		if(data[pos + 1] == '\\')   //是否有转义字符
		{
			if(data.size() > pos + 3 && data[pos + 3] == '\'')
				return new Token(this->_class, data, pos, 4, line, col);
		}
		else if(data[pos + 2] == '\'')
			return new Token(this->_class, data, pos, 3, line, col);
	}

	return nullptr;
}

Token* fay::NumberTokenRule::match(ByteData &data, int pos, int line, int col)
{
	int startPos = pos;  //记录下开始位置

	//处理不符合要求的情况
	if((data[pos] < '0' || data[pos] > '9') && data[pos] != '.')
		return nullptr;
	if(data[pos] == '.' && (data[pos + 1] < '0' || data[pos + 1] > '9'))
		return nullptr;

	bool hasDot = false;  //是否已经有.
	while(pos < data.size() && ((data[pos] >= '0' && data[pos] <= '9') || data[pos] == '.'))
	{
		if(data[pos] == '.')
		{
			if(hasDot)
				return nullptr;
			else
				hasDot = true;
		}

		pos++;
	}

	//处理结尾的描述符
	if(data[pos] == 'l' || data[pos] == 'L' || data[pos] == 'd' || data[pos] == 'D')
		pos++;

	return new Token(this->_class, data, startPos, pos - startPos, line, col);
}

Token* fay::StringTokenRule::match(ByteData &data, int pos, int line, int col)
{
	if(data[pos] == '"')
	{
		for(auto i = pos + 1; i < data.size(); ++i)
		{
			//不能换行
			if(data[i] == '\n' || data[i] == '\r')
				break;

			//字符串结束
			if(data[i] == '"' && data[i - 1] != '\\')
			{
				//返回字符串的token
				return new Token(this->_class, data, pos, i - pos + 1, line, col);
			}
		}
	}

	return nullptr;
}

fay::Token* fay::SymbolTokenRule::match(ByteData &data, int pos, int line, int col)
{

	if(data[pos] == this->_value)
		return new Token(this->_class, data, pos, 1, line, col);

	return nullptr;
}

Token* fay::WordsTokenRule::match(ByteData &data, int pos, int line, int col)
{
	for(auto i = 0; i < this->_words.size(); ++i)
	{
		std::string* word = &this->_words[i];
		bool isMatch = true;
		for(auto i = 0; i < word->size(); ++i)
		{
			if((*word)[i] != data[pos + i])
			{
				isMatch = false;
				break;
			}
		}

		if(isMatch)
			return new Token(this->type(), data, pos, word->size(), line, col);
	}

	return nullptr;
}

fay::Token* fay::WordTokenRule::match(ByteData &data, int pos, int line, int col)
{
	for(auto i = 0; i < this->_word.size(); ++i)
	{
		if(this->_word[i] != data[pos + i])
			return nullptr;
	}

	return new Token(this->type(), data, pos, this->_word.size(), line, col);
}

Token* fay::IDTokenRule::match(ByteData &data, int pos, int line, int col)
{

	if((data[pos] >= 'a' && data[pos] <= 'z') || (data[pos] >= 'A' && data[pos] <= 'Z') || data[pos] == '_')
	{
		int p = pos + 1;
		while(p < data.size())
		{
			if((data[p] >= 'a' && data[p] <= 'z')
				|| (data[p] >= 'A' && data[p] <= 'Z')
				|| (data[p] >= '0' && data[p] <= '9')
				|| data[p] == '_'
				|| data[p] == '.')
				p++;
			else
				break;
		}

		return new Token(this->_class, data, pos, p - pos, line, col);
	}

	return nullptr;
}

Token* fay::SingleCommentTokenRule::match(ByteData &data, int pos, int line, int col)
{
	int p = pos;
	while(p < data.size())
	{
		if(data[p] == '\n' || data[p] == '\r')
			break;
		else
			p++;
	}

	if(p > pos)
		return new Token(this->_class, data, pos, p - pos, line, col);

	return nullptr;
}
