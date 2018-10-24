#include <test_fay.h>
#include <mirror_utils_file.h>
#include <mirror_utils_log.h>
#include <fay_token_stack.h>
#include <fay_lexer.h>
#include <fay_parser.h>
#include <fay_stack.h>
#include <fay_internal_fun.h>
#include <fay_vm.h>

using namespace mirror;
using namespace fay;

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
		PRINT(i<<"\t"<<(*tokens)[i]->toString());

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
	sb.clear();
	domain->toString(&sb);
	PRINT(sb.toString());

	fay::FayVM vm;
}

void test::FayTests::Test1()
{
	WPTR(TestA) p1;
	PTR(TestA) p2 = p1.lock();
	if (p2==nullptr)
		LOG_DEBUG("P1 is OK");
}

void test::FayTests::TestRTTI()
{
	PTR(AstNode) node = MKPTR(AstFile)("aabbccdd");
	LOG_DEBUG(node->className());
	LOG_DEBUG(node->is(typeid(AstNode)));
}

void test::FayTests::TestInternalFun()
{
	VMStack stack;
	stack.push(new FayValue("aabbccdd"));
	stack.push(new FayValue("larlf.wang"));
	InternalFun::Print(&stack);
}
