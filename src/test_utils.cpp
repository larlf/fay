#include <test_utils.h>
#include <mirror_utils_log.h>
#include <mirror_utils_lang.h>
#include <vector>
#include <filesystem>
#include <regex>
#include <mirror_sys_const.h>
#include <thread>

using namespace test;
using namespace mirror;
namespace fs = std::experimental::filesystem;

void test::TestUtils::findFiles(std::vector<std::string> &list, const fs::path &path)
{
	for(auto &fe : fs::directory_iterator(path))
	{
		if(fs::is_directory(fe.path()))
			test::TestUtils::findFiles(list, fe.path());
		else
			list.push_back(fe.path().string());
	}
}

void test::TestUtils::logThread()
{
	LOG_DEBUG("one," << "two," << "three," << "four," << "five");
}

TEST(TestUtils, Log)
{
	std::string str = mirror::utils::TimeUtils::MSTimeString();
	std::cout << str << std::endl;

	PRINT("This is print");
	LOG_DEBUG("This is debug");
	LOG_INFO("This is info");
	LOG_WARN("This is warn");
	LOG_ERROR("This is error");

	//多线程下的Log
	for(auto i = 0; i < 10; ++i)
	{
		std::thread t(TestUtils::logThread);
		t.detach();
	}

	std::this_thread::sleep_for(chrono::seconds(1));
}

TEST(TestUtils, Trace)
{
	LOG_INFO("Test Trace");
	std::string stackMsg = mirror::log::SysTrace::TraceInfo();
	LOG_DEBUG(stackMsg);
}

TEST(TestUtils, Files)
{
	fs::path path("../project1");
	std::vector<std::string> list;
	test::TestUtils::findFiles(list, path);

	for(auto &it : list)
		LOG_DEBUG(it);
}

TEST(TestUtils, Tuple)
{
	std::tuple<std::string, int, int> v = std::make_tuple("abc", 5, 6);
	LOG_DEBUG(std::get<0>(v));
}


TEST(TestUtils, Hash)
{
	std::vector<std::string> v1
	{
		"oneoneoneoneoneoneoneoneoneoneoneoneone",
		"twotwotwotwotwotwotwotwotwotwotwotwotwotwotwo",
		"threethreethreethreethreethreethreethreethreethreethreethreethreethree",
		"fourfourfourfourfourfourfourfourfourfour",
		"fivefivefivefivefivefivefivefivefivefive",
		"sixsixsixsixsixsixsixsixsixsixsix"
	};

	std::vector<void*> v3
	{
		"abc", "abc", "abc", "abc", "abc",
	};

	MAP<std::string, std::vector<int>> v2;
	for(std::string str : v1)
		v2[str] = std::vector<int> {};

	std::vector<int> r1;
	auto t1 = utils::TimeUtils::MSTime();

	for(auto i = 0; i < 100000; ++i)
	{
		for(void* str : v3)
		{
			if(str == this)
				break;
		}

		//r1 = v2["sixsixsixsixsixsixsixsixsixsixsix1"];
	}

	auto t2 = utils::TimeUtils::MSTime();
	LOG_DEBUG(r1.size());
	LOG_DEBUG("Time : " << (t2 - t1));
}


