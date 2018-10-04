#pragma once

#include <fay_ast.h>
#include <fay_token_stack.h>

namespace fay
{
	//语法解析器
	class Parser
	{
	private:
		static PTR(AstNode) _File(TokenStack *stack, const std::string &filename);

	public:
		static PTR(AstNode) Parse(PTR(std::vector<PTR(Token)>) tokens, const std::string &filename);
	};
}