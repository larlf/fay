#include <fay_parser.h>

using namespace fay;

PTR(AstNode) fay::Parser::_File(TokenStack * stack, const std::string & filename)
{
	PTR(AstFile) node = MKPTR(AstFile)(filename);
	return node->shared_from_this();
}

PTR(AstNode) fay::Parser::Parse(PTR(std::vector<PTR(Token)>) tokens, const std::string & filename)
{
	TokenStack stack(tokens);
	return Parser::_File(&stack, filename);
}
