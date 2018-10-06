#include <test_fay.h>
#include <mirror_utils_file.h>
#include <mirror_utils_log.h>
#include <fay_token_stack.h>
#include <fay_lexer.h>
#include <fay_parser.h>

using namespace mirror;

void test::FayTests::Init()
{
	fay::TypeDict::Init();
}

void test::FayTests::TestLexer()
{
	std::string filename = "../../script/test6.fay";
	std::string text = utils::FileUtils::ReadTextFile(filename);
	LOG_DEBUG(text);

	PRINT("----------------------------------------");

	fay::Lexer lexer;
	auto tokens=lexer.Execute(text);

	for each(auto it in (*tokens))
		PRINT(it->toString());

	PRINT("----------------------------------------");

	PTR(fay::AstNode) ast = fay::Parser::Parse(tokens, filename);
	if (ast)
	{
		utils::StringBuilder sb;
		ast->toString(sb);
		PRINT(sb.toString());
		ast->destory();
	}
}
