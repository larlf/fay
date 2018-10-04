#pragma once

#include <fay_ast.h>
#include <fay_token_stack.h>

namespace fay
{
	class ParseException : public std::exception
	{
	public:
		ParseException(TokenStack *stack, const std::string &msg);
	};

	//语法解析器
	class Parser
	{
	private:
		static PTR(AstNode) _File(TokenStack *stack, const std::string &filename);
		static PTR(AstNode) _Using(TokenStack *stack);
		static PTR(AstNode) _Package(TokenStack *stack);

	public:
		static PTR(AstNode) Parse(PTR(std::vector<PTR(Token)>) tokens, const std::string &filename);
	};
}