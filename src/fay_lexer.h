﻿#pragma once
#include <fay_token.h>
#include <fay_object.h>

namespace fay
{
	//词法分析器
	class Lexer
	{
	private:
		std::vector<ITokenRule*> _rules;
		LexMode _mode = LexMode::Program;

		//如果有需要对模式进行变量的操作，在这里进行处理
		PTR(Token) changeMode(PTR(Token) t);

	public:
		Lexer();
		~Lexer();
		PTR(std::vector<PTR(Token)>) Execute(PTR(fay::FayFile) file);
	};
}

