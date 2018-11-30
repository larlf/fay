#include <test_fay.h>
#include <mirror_utils_file.h>
#include <mirror_utils_log.h>
#include <fay_token_stack.h>
#include <fay_lexer.h>
#include <fay_parser.h>
#include <fay_internal_fun.h>
#include <fay_vm.h>
#include <fay_i18n.h>
#include <fay_project.h>
#include <stack>
#include <gtest/gtest.h>

using namespace mirror;
using namespace fay;
using namespace test;

PTR(FayProject) test::FayLang::project;
PTR(FayVM) test::FayLang::vm;

void test::FayLang::SetUpTestCase()
{
	//初始化字典数据
	fay::TypeDict::Init();

	//初始化国际化信息
	std::string i18nText = utils::FileUtils::ReadTextFile("../doc/i18n.cn.json");
	fay::I18N::Init(i18nText);

	//取得所有的代码文件
	std::string projectPath = "../script/test1";
	std::vector<std::string> files = utils::FileUtils::FindFiles(projectPath + "/src", true, ".fay");

	project = MKPTR(FayProject)("Test1", 1, 0);
	project->addFiles(files);
	project->parse();
	project->build();

	vm = MKPTR(FayVM)(project->domain());
}

TEST_F(FayLang, TypeTest1)
{
	PRINT(project->findSource("TypeTest.fay")->tokensStr());
	PRINT(project->findSource("TypeTest.fay")->astStr());

	auto type = project->domain()->findClass("fay.dev.test.TypeTest");

	{
		auto funs = type->findFunByName("test1", true);

		utils::StringBuilder sb;
		funs[0]->buildString(&sb);
		PRINT(sb.toString());

		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 11);
	}

	{
		auto funs = type->findFunByName("test2", true);

		utils::StringBuilder sb;
		funs[0]->buildString(&sb);
		PRINT(sb.toString());

		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Long);
		ASSERT_EQ(rs.longVal(), 14);
	}

	{
		auto funs = type->findFunByName("test3", true);

		utils::StringBuilder sb;
		funs[0]->buildString(&sb);
		PRINT(sb.toString());

		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Long);
		ASSERT_EQ(rs.longVal(), 101);
	}

	{
		auto funs = type->findFunByName("test4", true);

		utils::StringBuilder sb;
		funs[0]->buildString(&sb);
		PRINT(sb.toString());

		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::String);
		ASSERT_EQ(*rs.strVal(), "23");
	}
}

//数据计算相关的测试
TEST_F(FayLang, MathTest)
{
	PRINT(project->findSource("MathTest.fay")->tokensStr());
	PRINT(project->findSource("MathTest.fay")->astStr());
	auto type = project->domain()->findClass("fay.dev.test.MathTest");

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test1", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), -5);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test2", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 6);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test3", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 4);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test4", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 6);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test5", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 5);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test6", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 11);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test7", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Long);
		ASSERT_EQ(rs.longVal(), 101);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test8", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Double);
		ASSERT_EQ(rs.doubleVal(), 101);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test9", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), true);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test10", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), false);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test11", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), false);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test12", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Byte);
		ASSERT_EQ(rs.byteVal(), (byte)~15);
	}
	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test13", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Byte);
		ASSERT_EQ(rs.byteVal(), (byte)~15);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test14", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 3);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test15", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 5<<2);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test16", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 88990011>>3);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test17", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), true);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test18", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), true);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test19", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), true);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test20", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 1);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test21", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 184);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test22", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 107);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test23", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), false);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test24", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Long);
		ASSERT_EQ(rs.longVal(), (7788 & (99999999 | 361)));
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test25", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), true);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test26", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), false);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test27", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 15);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test28", true);
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 1);
	}
}

TEST_F(FayLang, Test1)
{
	auto type = project->domain()->findClass("com.larlf.MyTest");
	if (type == nullptr)
	{
		LOG_ERROR("Cannot find type : com.larlf.MyTest");
		return;
	}

	auto funs = type->findFunByName("fun5", true);
	if (funs.size() > 0)
	{
		//显示函数的内容
		utils::StringBuilder sb;
		funs[0]->buildString(&sb);
		PRINT(sb.toString());

		vm->run(funs[0]);
	}
}

TEST_F(FayLang, FayValue)
{
	std::string str = "one";
	FayValue v4;
	FayValue v5;

	{
		FayValue v1(str);
		FayValue v2("two");

		FayValue v3 = FayLang::makeValue(*v1.strVal());
		v4 = v3;
		v3 = v2;
		v5 = FayValue(v3);
		 
		FayValue v6(100); 
		FayValue v7;

		TestValue v11;
		TestValue v12;

		auto t1 = utils::TimeUtils::MSTime();

		for (auto i = 0; i < 100000000; ++i)
			v7 = v6;

		LOG_DEBUG("Value : " << v7.intVal());

		auto t2 = utils::TimeUtils::MSTime();

		LOG_INFO("Time : " << (t2 - t1));
		ASSERT_LE((t2 - t1), 1000)<<"赋值性能测试";
	}

	ASSERT_EQ(*v4.strVal(), "one");
	ASSERT_EQ(*v5.strVal(), "two");
}

FayValue test::FayLang::makeValue(const std::string &str)
{
	FayValue v = FayValue(str.c_str());
	return v;
}



TEST_F(FayLang, Run)
{
	try
	{
		std::string filename = "../script/test1.fay";
		std::string text = utils::FileUtils::ReadTextFile(filename);
		LOG_DEBUG(text);
		PTR(FayFile) file = MKPTR(FayFile)(filename, text);

		PRINT("----------------------------------------");

		fay::Lexer lexer;
		auto tokens = lexer.Execute(file);

		for(auto i = 0; i < tokens->size(); ++i)
			PRINT(i << "\t" << (*tokens)[i]->toString());

		PRINT("----------------------------------------");

		PTR(fay::AstNode) ast = fay::Parser::Parse(tokens, filename);
		if(ast)
		{
			utils::StringBuilder sb;
			ast->buildString(&sb);
			PRINT(sb.toString());
		}

		PRINT("----------------------------------------");

		PTR(fay::FayDomain) domain = MKPTR(fay::FayDomain)();
		domain->init();

		PTR(fay::FayBuilder) builder = MKPTR(fay::FayBuilder)(domain);
		builder->beginLib("TestLib");

		ast->dig1(builder.get());

		PRINT("--------------------Dig1--------------------");
		utils::StringBuilder sb;
		domain->buildString(&sb);
		PRINT(sb.toString());

		ast->dig2(builder.get());

		PRINT("--------------------Dig2--------------------");
		sb.clear();
		domain->buildString(&sb);
		PRINT(sb.toString());

		ast->dig3(builder.get());
		PRINT("--------------------Dig3--------------------");
		if(ast)
		{
			utils::StringBuilder sb;
			ast->buildString(&sb);
			PRINT(sb.toString());
		}

		ast->dig4(builder.get());
		PRINT("--------------------Dig4--------------------");
		sb.clear();
		domain->buildString(&sb);
		PRINT(sb.toString());

		fay::FayVM vm(domain);
		auto fun = domain->findFun("com.larlf.MyTest", "fun4()", false);

		PRINT("-------------------Start--------------------");
		vm.run(fun);
		PRINT("--------------------End--------------------");

		LOG_DEBUG("Stack Size : " << vm.stackSize());
	}
	catch(fay::FayCompileException &e)
	{
		LOG_ERROR(e.what());
	}
}

TEST_F(FayLang, RTTI)
{
	PTR(AstNode) ast1 = MKPTR(AstFor)(nullptr);
	ASSERT_EQ(ast1->is<AstFor>(), true);
	ASSERT_EQ(ast1->is<AstNode>(), true);
	ASSERT_EQ(ast1->is<test::FayLang>(), false);
}

