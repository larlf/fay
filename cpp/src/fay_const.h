#pragma once

#include <iostream>
#include <mirror_sys_const.h>

namespace fay
{
	/**
	* 词法分析器的模式
	*/
	enum class LexMode
	{
		Program,
		Comment
	};

	/**
	 * Token的类型
	 */
	enum class TokenType
	{
		//TokenTypeStart
		None,
		Assign,
		Colon,
		Semicolon,  //;
		Comma,
		LeftBrace,
		RightBrace,
		OP,
		DoubleSlash,
		Comment,
		Using,
		Package,
		Var,
		Function,
		Class,
		Interface,
		DescSymbol,
		SystemName,
		If,
		Else,
		ElseIf,
		For,
		Foreach,
		While,
		Do,
		Return,
		ID,
		BasicType,
		Number,
		String,
		Char,
		//TokenTypeEnd
	};

	class TypeDict
	{
	private:
		static MAP<TokenType, std::string> TokenTypeName;

	public:
		static void Init();
		static std::string ToName(TokenType value);
	};
}

