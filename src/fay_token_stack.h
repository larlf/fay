﻿#pragma once

#include <fay_token.h>
#include <fay_object.h>

namespace fay
{
	//用于进行语法解析处理时的数据结构
	class TokenStack
	{
	private:
		PTR(std::vector<PTR(Token)>) _tokens;
		int _pos = 0;
		int _sign = -1;

	public:
		static PTR(Token) EndToken;
		PTR(FayFile) file;

		TokenStack(PTR(FayFile) file, PTR(std::vector<PTR(Token)>) tokens)
			: file(file), _tokens(tokens)
		{
			//跳过开头的注释
			while(this->now()->is(TokenType::Comment))
				this->next();
		}

		//当前的位置
		int pos() { return this->_pos; }
		//返回当前的Token
		PTR(Token) now();
		//返回前一个Token
		PTR(Token) before();
		//返回后一个Token
		PTR(Token) after();
		PTR(Token) after(size_t index);
		//移动并返回下一个Token
		PTR(Token) next();
		//返回当前Token并移动到下一个
		PTR(Token) move();
		//查找符合要求的下一个关键字
		PTR(Token) findNextToken(const std::vector<TokenType> &types);
	};
}