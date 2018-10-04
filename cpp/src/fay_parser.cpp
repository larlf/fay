#include <fay_parser.h>
#include <mirror_utils_string.h>

using namespace fay;
using namespace mirror;

PTR(AstNode) fay::Parser::_File(TokenStack* stack, const std::string &filename)
{
	PTR(AstFile) ast = MKPTR(AstFile)(filename);

	while (true)
	{
		PTR(Token) token = stack->findNextToken(
		{
			TokenType::Class,
			TokenType::Using,
			TokenType::Package
		});

		PTR(AstNode) node;
		switch (token->type())
		{
			case TokenType::Class:
				break;
			case TokenType::Using:
				node = Parser::_Using(stack);
				break;
			case TokenType::Package:
				node = Parser::_Package(stack);
				break;
		}

		if (node)
			ast->addChildNode(node);
		else
			break;
	}

	return ast;
}

PTR(AstNode) fay::Parser::_Using(TokenStack* stack)
{
	PTR(AstNode) node;

	if (stack->now()->is(TokenType::Using))
	{
		if (!stack->next()->is(TokenType::ID))
			throw ParseException(stack, "using format error");

		node = MKPTR(AstUsing)(stack->now()->text());

		if (!stack->next()->is(TokenType::Semicolon))
			throw ParseException(stack, "using not stop with ;");

		stack->next();
	}

	return node;
}

PTR(AstNode) fay::Parser::_Package(TokenStack* stack)
{
	PTR(AstPackage) node;

	if (stack->now()->is(TokenType::Package))
	{
		if (!stack->next()->is(TokenType::ID))
			throw ParseException(stack, "package format error");

		node = MKPTR(AstPackage)(stack->now()->text());

		if (!stack->next()->is(TokenType::Semicolon))
			throw ParseException(stack, "package not stop with ;");

		stack->next();
	}

	return node;
}

PTR(AstNode) fay::Parser::Parse(PTR(std::vector<PTR(Token)>) tokens, const std::string &filename)
{
	TokenStack stack(tokens);
	return Parser::_File(&stack, filename);
}

fay::ParseException::ParseException(TokenStack* stack, const std::string &msg)
	: std::exception::exception((msg + "\n" + stack->now()->toString()).c_str())
{
}
