#include <test_fay.h>
#include <mirror.h>
#include <stack>
#include <gtest/gtest.h>

using namespace mirror;
using namespace fay;
using namespace test;

PTR(FayProject) test::TestFay::project;

void test::TestFay::SetUpTestCase()
{
	LOG_DEBUG("Path : " << fs::current_path());
	LOG_DEBUG("Path : " << fs::temp_directory_path());

	//初始化系统环境
	fay::SystemEnv::Init();

	//初始化字典数据
	fay::TypeDict::Init();

	//初始化国际化信息
	fay::I18N::Init("../doc/i18n.cn.json");

	FayDomain::InitSysLib();

	//取得所有的代码文件
	std::string projectPath = "../script/test1";
	std::vector<fs::path> files;
	utils::FileUtils::FindFiles(files, projectPath + "/src", true, ".fay");

	project = MKPTR(FayProject)(projectPath);
	//project->addFiles(files);
	//project->parse();
	//project->build();

	utils::FileUtils::WriteTextFile("domain.txt", FayDomain::ToString());
}

TEST_F(TestFay, Type)
{
	PRINT(project->findSource("TypeTest.fay")->tokensStr());
	PRINT(project->findSource("TypeTest.fay")->astStr());

	auto type = FayDomain::FindClass("fay.dev.test.TypeTest");

	{
		auto funs = type->findFunByName("test1", true);

		utils::StringBuilder sb;
		funs[0]->buildString(&sb);
		PRINT(sb.toString());

		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 11);
	}

	{
		auto funs = type->findFunByName("test2", true);

		utils::StringBuilder sb;
		funs[0]->buildString(&sb);
		PRINT(sb.toString());

		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Long);
		ASSERT_EQ(rs.longVal(), 14);
	}

	{
		auto funs = type->findFunByName("test3", true);

		utils::StringBuilder sb;
		funs[0]->buildString(&sb);
		PRINT(sb.toString());

		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Long);
		ASSERT_EQ(rs.longVal(), 101);
	}

	{
		auto funs = type->findFunByName("test4", true);

		utils::StringBuilder sb;
		funs[0]->buildString(&sb);
		PRINT(sb.toString());

		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::String);
		ASSERT_EQ(*rs.strVal(), "23");
	}
}

//数据计算相关的测试
TEST_F(TestFay, Math)
{
	PRINT(project->findSource("MathTest.fay")->tokensStr());
	PRINT(project->findSource("MathTest.fay")->astStr());
	auto type = FayDomain::FindClass("fay.dev.test.MathTest");

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test1", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), -5);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test2", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 6);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test3", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 4);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test4", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 6);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test5", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 5);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test6", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 11);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test7", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Long);
		ASSERT_EQ(rs.longVal(), 101);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test8", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Double);
		ASSERT_EQ(rs.doubleVal(), 101);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test9", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), true);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test10", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), false);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test11", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), false);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test12", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Byte);
		ASSERT_EQ(rs.byteVal(), (byte)~15);
	}
	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test13", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Byte);
		ASSERT_EQ(rs.byteVal(), (byte)~15);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test14", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 3);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test15", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 5 << 2);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test16", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 88990011 >> 3);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test17", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), true);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test18", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), true);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test19", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), true);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test20", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 1);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test21", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 184);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test22", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 107);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test23", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), false);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test24", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Long);
		ASSERT_EQ(rs.longVal(), (7788 & (99999999 | 361)));
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test25", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), true);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test26", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Bool);
		ASSERT_EQ(rs.boolVal(), false);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test27", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 15);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test28", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 1);
	}

	{
		std::vector<PTR(FayFun)>funs = type->findFunByName("test29", true);
		PRINT(funs[0]->toString());
		auto rs = FayVM::Run(funs[0]);
		ASSERT_EQ(rs.type(), ValueType::Int);
		ASSERT_EQ(rs.intVal(), 15);
	}
}

TEST_F(TestFay, Flow)
{
	PRINT(project->findSource("FlowTest.fay")->tokensStr());
	PRINT(project->findSource("FlowTest.fay")->astStr());
	auto type = FayDomain::FindClass("fay.dev.test.FlowTest");

	std::vector<PTR(FayFun)> funs;
	FayValue rs;

	funs = type->findFunByName("test1", true);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 5);

	funs = type->findFunByName("test2", true);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 7);

	funs = type->findFunByName("test3", true);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 55);

	funs = type->findFunByName("test4", true);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 6);

	funs = type->findFunByName("test5", true);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 16);

	funs = type->findFunByName("test6", true);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 16);

	funs = type->findFunByName("test7", true);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);
	//ASSERT_EQ(rs.type(), ValueType::Int);
	//ASSERT_EQ(rs.intVal(), 16);
}

TEST_F(TestFay, String)
{
	PRINT(project->findSource("StringTest.fay")->tokensStr());
	PRINT(project->findSource("StringTest.fay")->astStr());
	auto type = FayDomain::FindClass("fay.dev.test.StringTest");

	std::vector<PTR(FayFun)> funs;
	FayValue rs;

	funs = type->findFunByName("test1", true);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::String);
	ASSERT_EQ(utils::StringUtils::Encoding(*rs.strVal(), "UTF-8", "GBK"), "abc我是字符串");

	funs = type->findFunByName("test2", true);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::String);
	ASSERT_EQ(*rs.strVal(), "105");

	funs = type->findFunByName("test3", true);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::String);
	ASSERT_EQ(*rs.strVal(), "Hello,larlf!");

	funs = type->findFunByName("test4", true);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::String);
	ASSERT_EQ(*rs.strVal(), "15");

	funs = type->findFunByName("test5", true);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);
}

TEST_F(TestFay, OOP1)
{
	PRINT(project->findSource("OOPTest.fay")->tokensStr());
	PRINT(project->findSource("OOPTest.fay")->astStr());
	auto type = FayDomain::FindClass("fay.dev.test.OOPTest");

	std::vector<PTR(FayFun)> funs;
	FayValue rs;

	funs = type->findFunByName(FUN_STATIC_INIT, true);
	ASSERT_GT(funs.size(), 0);
	PRINT(funs[0]->toString());

	funs = type->findFunByName(FUN_CREATE, false);
	ASSERT_GT(funs.size(), 0);
	PRINT(funs[0]->toString());

	funs = type->findFunByName("test1", true);
	ASSERT_GT(funs.size(), 0);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 10001);

	funs = type->findFunByName("test2", true);
	ASSERT_GT(funs.size(), 0);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 99);

	funs = type->findFunByName("test3", true);
	ASSERT_GT(funs.size(), 0);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 10002);

	funs = type->findFunByName("test4", true);
	ASSERT_GT(funs.size(), 0);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 11);

	funs = type->findFunByName("test5", true);
	ASSERT_GT(funs.size(), 0);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	ASSERT_EQ(rs.intVal(), 24);
}

TEST_F(TestFay, OOP2)
{
	//PRINT(project->findSource("OOPTest.fay")->tokensStr());
	//PRINT(project->findSource("OOPTest.fay")->astStr());
	//auto type = FayDomain::FindClass("fay.dev.test.BaseA");
	//PRINT(type->toString());
	//type = FayDomain::FindClass("fay.dev.test.SubA");
	//PRINT(type->toString());

	auto type = FayDomain::FindClass("fay.dev.test.OOPTest");
	auto funs = type->findFunByName("test6", true);
	ASSERT_GT(funs.size(), 0);
	PRINT(funs[0]->toString());
	auto rs = FayVM::Run(funs[0]);
	ASSERT_EQ(rs.type(), ValueType::Int);
	//ASSERT_EQ(rs.intVal(), 24);
}

TEST_F(TestFay, FayValue)
{
	std::string str = "one";
	FayValue v4;
	FayValue v5;

	{
		FayValue v1(str);
		FayValue v2("two");

		FayValue v3 = TestFay::makeValue(*v1.strVal());
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

FayValue test::TestFay::makeValue(const std::string &str)
{
	FayValue v = FayValue(str.c_str());
	return v;
}

TEST_F(TestFay, RTTI)
{
	PTR(AstNode) ast1 = MKPTR(AstFor)(nullptr);
	ASSERT_EQ(ast1->is<AstFor>(), true);
	ASSERT_EQ(ast1->is<AstNode>(), true);
	ASSERT_EQ(ast1->is<test::TestFay>(), false);
}

TEST_F(TestFay, TaskQueue)
{
	PTR(fay::BuildTaskQueue<TestClass>) queue = MKPTR(fay::BuildTaskQueue<TestClass>)();
	queue->add(MKPTR(TestClass)("one"));
	queue->add(MKPTR(TestClass)("two"));
	queue->add(MKPTR(TestClass)("three"));
	ASSERT_EQ(queue->activeSize(), 3);
	ASSERT_EQ(queue->waitSize(), 3);

	PTR(TestClass) task1 = queue->get();
	ASSERT_EQ(queue->activeSize(), 3);
	ASSERT_EQ(queue->waitSize(), 2);

	PTR(TestClass) task2 = queue->get();
	ASSERT_EQ(queue->activeSize(), 3);
	ASSERT_EQ(queue->waitSize(), 1);

	queue->complete(task1);
	ASSERT_EQ(queue->activeSize(), 2);
	ASSERT_EQ(queue->waitSize(), 1);

	queue->complete(task1);
	ASSERT_EQ(queue->activeSize(), 2);
	ASSERT_EQ(queue->waitSize(), 1);

	queue->complete(task1);
	ASSERT_EQ(queue->activeSize(), 2);
	ASSERT_EQ(queue->waitSize(), 1);

	queue->complete(task2);
	ASSERT_EQ(queue->activeSize(), 1);
	ASSERT_EQ(queue->waitSize(), 1);

	auto list = queue->completedTasks();
	ASSERT_EQ(list.size(), 2);
	ASSERT_EQ(list[0], task1);
	ASSERT_EQ(list[1], task2);

	PTR(TestClass) task3 = queue->get();
	ASSERT_EQ(queue->activeSize(), 1);
	ASSERT_EQ(queue->waitSize(), 0);

	PTR(TestClass) task4 = queue->get();
	ASSERT_EQ(queue->activeSize(), 1);
	ASSERT_EQ(queue->waitSize(), 0);
	ASSERT_EQ(task4, nullptr);

	queue->complete(task4);
	ASSERT_EQ(queue->activeSize(), 1);
	ASSERT_EQ(queue->waitSize(), 0);

	queue->complete(task3);
	ASSERT_EQ(queue->activeSize(), 0);
	ASSERT_EQ(queue->waitSize(), 0);
	ASSERT_EQ(queue->completedTasks().size(), 3);
}

