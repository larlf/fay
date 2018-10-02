﻿#pragma once

#include <fay_token.h>

namespace fay
{
	class Lexer
	{
	private:
		std::vector<ITokenRule*> _rules;
		LexMode _mode = LexMode::Program;

		/**
		* 如果有需要对模式进行变量的操作，在这里进行处理
		*/
		PTR(Token) changeMode(PTR(Token) t);

	public:
		Lexer();
		~Lexer();
		PTR(std::vector<PTR(Token)>) Execute(std::string text);
	};

	/**
	* 词法解析错误后的异常处理
	*/
	class LexerException : std::exception
	{
		using std::exception::exception;
	};
}

