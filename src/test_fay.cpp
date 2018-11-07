#include <test_fay.h>
#include <mirror_utils_file.h>
#include <mirror_utils_log.h>
#include <fay_token_stack.h>
#include <fay_lexer.h>
#include <fay_parser.h>
#include <fay_internal_fun.h>
#include <fay_vm.h>
#include <fay_i18n.h>
#include <stack>
#include <gtest/gtest.h>

using namespace mirror;
using namespace fay;

TEST(FayLang, Run)
{
	try
	{
		std::string i18nText = utils::FileUtils::ReadTextFile("../doc/i18n.cn.json");
		fay::I18N::Init(i18nText);

		std::string filename = "../script/test1.fay";
		std::string text = utils::FileUtils::ReadTextFile(filename);
		LOG_DEBUG(text);
		PTR(FayFile) file = MKPTR(FayFile)(filename, text);

		PRINT("----------------------------------------");

		fay::Lexer lexer;
		auto tokens = lexer.Execute(file);

		for (auto i = 0; i < tokens->size(); ++i)
			PRINT(i << "\t" << (*tokens)[i]->toString());

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
		domain->initSysLib();

		PTR(fay::FayBuilder) builder = MKPTR(fay::FayBuilder)(domain);
		builder->beginLib("TestLib");

		ast->dig1(builder.get());

		PRINT("--------------------Dig1--------------------");
		utils::StringBuilder sb;
		domain->toString(&sb);
		PRINT(sb.toString());

		ast->dig2(builder.get());

		PRINT("--------------------Dig2--------------------");
		sb.clear();
		domain->toString(&sb);
		PRINT(sb.toString());

		ast->dig3(builder.get());
		PRINT("--------------------Dig3--------------------");
		if (ast)
		{
			utils::StringBuilder sb;
			ast->toString(&sb);
			PRINT(sb.toString());
		}

		ast->dig4(builder.get());
		PRINT("--------------------Dig4--------------------");
		sb.clear();
		domain->toString(&sb);
		PRINT(sb.toString());

		fay::FayVM vm(domain);
		auto fun = domain->findFun("com.larlf.MyTest", "fun4()", false);

		PRINT("-------------------Start--------------------");
		vm.run(fun);
		PRINT("--------------------End--------------------");

		LOG_DEBUG("Stack Size : " << vm.stackSize());
	}
	catch (fay::FayCompileException &e)
	{
		LOG_ERROR(e.what());
	}
	catch (std::exception &e)
	{
		LOG_ERROR(e.what());
	}

}

TEST(FayLang, RTTI)
{
	PTR(AstNode) ast1 = MKPTR(AstFor)(nullptr);
	ASSERT_EQ(ast1->is<AstFor>(), true); 
	ASSERT_EQ(ast1->is<AstNode>(), true);
	ASSERT_EQ(ast1->is<test::FayLang>(), false);
}
