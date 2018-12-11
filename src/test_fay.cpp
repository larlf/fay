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

TEST_F(FayLang, Type)
{
	PRINT(project->findSource("TypeTest.fay")->tokensStr());
	PRINT(project->findSource("TypeTest.fay")->astStr());

	auto type = project->domain()->findClass("fay.dev.test.TypeTest");

	{
		auto funs = type->findFunByName("test1");

		utils::StringBuilder sb;
		funs[0]->buildString(&sb);
		PRINT(sb.toString());

		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 11);
	}

	{
		auto funs = type->findFunByName("test2");

		utils::StringBuilder sb;
		funs[0]->buildString(&sb);
		PRINT(sb.toString());

		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Long);
		ASSERT_EQ(rs.longVal(), 14);
	}

	{
		auto funs = type->findFunByName("test3");

		utils::StringBuilder sb;
		funs[0]->buildString(&sb);
		PRINT(sb.toString());

		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Long);
		ASSERT_EQ(rs.longVal(), 101);
	}

	{
		auto funs = type->findFunByName("test4");

		utils::StringBuilder sb;
		funs[0]->buildString(&sb);
		PRINT(sb.toString());

		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::String);
		ASSERT_EQ(*rs.strVal(), "23");
	}
}

//数据计算相关的测试
TEST_F(FayLang, Math)
{
	PRINT(project->findSource("MathTest.fay")->tokensStr());
	PRINT(project->findSource("MathTest.fay")->astStr());
	auto type = project->domain()->findClass("fay.dev.test.MathTest");

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test1");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), -5);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test2");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 6);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test3");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 4);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test4");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 6);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test5");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 5);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test6");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 11);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test7");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Long);
		ASSERT_EQ(rs.longVal(), 101);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test8");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Double);
		ASSERT_EQ(rs.doubleVal(), 101);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test9");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), true);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test10");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), false);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test11");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), false);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test12");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Byte);
		ASSERT_EQ(rs.byteVal(), (byte)~15);
	}
	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test13");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Byte);
		ASSERT_EQ(rs.byteVal(), (byte)~15);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test14");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 3);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test15");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 5 << 2);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test16");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 88990011 >> 3);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test17");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), true);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test18");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), true);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test19");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), true);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test20");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 1);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test21");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 184);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test22");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 107);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test23");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), false);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test24");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Long);
		ASSERT_EQ(rs.longVal(), (7788 & (99999999 | 361)));
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test25");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), true);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test26");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), false);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test27");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 15);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test28");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 1);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test29");
		PRINT(funs[0]->toString());
		auto rs = vm->run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 15);
	}
}

TEST_F(FayLang, Flow)
{
	PRINT(project->findSource("FlowTest.fay")->tokensStr());
	PRINT(project->findSource("FlowTest.fay")->astStr());
	auto type = project->domain()->findClass("fay.dev.test.FlowTest");

	std::vector<PTR(FayFun)> funs;
	FayValue rs;

	funs = type->findFunByName("test1");
	PRINT(funs[0]->toString());
	rs = vm->run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 5);

	funs = type->findFunByName("test2");
	PRINT(funs[0]->toString());
	rs = vm->run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 7);

	funs = type->findFunByName("test3");
	PRINT(funs[0]->toString());
	rs = vm->run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 55);

	funs = type->findFunByName("test4");
	PRINT(funs[0]->toString());
	rs = vm->run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 6);

	funs = type->findFunByName("test5");
	PRINT(funs[0]->toString());
	rs = vm->run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 16);

	funs = type->findFunByName("test6");
	PRINT(funs[0]->toString());
	rs = vm->run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 16);
}

TEST_F(FayLang, String)
{
	PRINT(project->findSource("StringTest.fay")->tokensStr());
	PRINT(project->findSource("StringTest.fay")->astStr());
	auto type = project->domain()->findClass("fay.dev.test.StringTest");

	std::vector<PTR(FayFun)> funs;
	FayValue rs;

	funs = type->findFunByName("test1");
	PRINT(funs[0]->toString());
	rs = vm->run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::String);
	ASSERT_EQ(utils::StringUtils::Encoding(*rs.strVal(), "UTF-8", "GBK"), "abc我是字符串");

	funs = type->findFunByName("test2");
	PRINT(funs[0]->toString());
	rs = vm->run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::String);
	ASSERT_EQ(*rs.strVal(), "105");

	funs = type->findFunByName("test3");
	PRINT(funs[0]->toString());
	rs = vm->run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::String);
	ASSERT_EQ(*rs.strVal(), "Hello,larlf!");

	funs = type->findFunByName("test4");
	PRINT(funs[0]->toString());
	rs = vm->run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::String);
	ASSERT_EQ(*rs.strVal(), "15");

	funs = type->findFunByName("test5");
	PRINT(funs[0]->toString());
	rs = vm->run(funs[0]);
}

TEST_F(FayLang, OOP)
{
	PRINT(project->findSource("OOPTest.fay")->tokensStr());
	PRINT(project->findSource("OOPTest.fay")->astStr());
	auto type = project->domain()->findClass("fay.dev.test.OOPTest");

	std::vector<PTR(FayFun)> funs;
	FayValue rs;

	funs = type->findFunByName(".init");
	ASSERT_GT(funs.size(), 0);
	PRINT(funs[0]->toString());

	funs = type->findFunByName(".create");
	ASSERT_GT(funs.size(), 0);
	PRINT(funs[0]->toString());

	funs = type->findFunByName("test1");
	ASSERT_GT(funs.size(), 0);
	PRINT(funs[0]->toString());
	rs = vm->run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 10001);

	funs = type->findFunByName("test2");
	ASSERT_GT(funs.size(), 0);
	PRINT(funs[0]->toString());
	rs = vm->run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	//ASSERT_EQ(rs.intVal(), 10002);
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

		for(auto i = 0; i < 100000000; ++i)
			v7 = v6;

		LOG_DEBUG("Value : " << v7.intVal());

		auto t2 = utils::TimeUtils::MSTime();

		LOG_INFO("Time : " << (t2 - t1));
		ASSERT_LE((t2 - t1), 1000) << "赋值性能测试";
	}

	ASSERT_EQ(*v4.strVal(), "one");
	ASSERT_EQ(*v5.strVal(), "two");
}

FayValue test::FayLang::makeValue(const std::string &str)
{
	FayValue v = FayValue(str.c_str());
	return v;
}

TEST_F(FayLang, RTTI)
{
	PTR(AstNode) ast1 = MKPTR(AstFor)(nullptr);
	ASSERT_EQ(ast1->is<AstFor>(), true);
	ASSERT_EQ(ast1->is<AstNode>(), true);
	ASSERT_EQ(ast1->is<test::FayLang>(), false);
}

