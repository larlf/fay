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

	for (auto i = 0; i < tokens->size(); ++i)
	{
		PRINT(i<<"\t"<<(*tokens)[i]->toString());
	}

	PRINT("----------------------------------------");

	PTR(fay::AstNode) ast = fay::Parser::Parse(tokens, filename);
	if (ast)
	{
		utils::StringBuilder sb;
		ast->toString(&sb);
		PRINT(sb.toString());
	}

	PRINT("----------------------------------------");

	PTR(fay::FayDomain) domain = MKPTR(fay::FayDomain)();
	PTR(fay::FayLib) lib = MKPTR(fay::FayLib)(domain, "TestLib");

	PTR(fay::OutlineBuilder) builder = MKPTR(fay::OutlineBuilder)(lib);
	ast->makeOutline(builder.get());

	domain->addLib(lib);

	{
		utils::StringBuilder sb;
		domain->toString(&sb);
		PRINT(sb.toString());
	}

}
