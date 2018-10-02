#include "test_fay.h"
#include "mirror_utils_file.h"
#include "mirror_utils_log.h"
#include <fay_lexer.h>

using namespace mirror;

void test::FayTests::Init()
{
	fay::TypeDict::Init();
}

void test::FayTests::TestLexer()
{
	std::string text = utils::FileUtils::ReadTextFile("../../script/test6.fay");
	LOG_DEBUG(text);

	fay::Lexer lexer;
	auto tokens=lexer.Execute(text);

	for each(auto it in (*tokens))
	{
		PRINT(it->toString());
	}
}
