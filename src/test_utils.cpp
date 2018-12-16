#include <test_utils.h>
#include <mirror_utils_log.h>
#include <mirror_utils_lang.h>
#include <vector>
#include <filesystem>
#include <regex>

using namespace mirror;
namespace fs = std::experimental::filesystem;

void test::Utils::findFiles(std::vector<std::string> &list, const fs::path &path)
{
	for(auto &fe : fs::directory_iterator(path))
	{
		if(fs::is_directory(fe.path()))
			test::Utils::findFiles(list, fe.path());
		else
			list.push_back(fe.path().string());
	}
}

TEST(Utils, Log)
{
	std::string str = mirror::utils::TimeUtils::MSTimeString();
	std::cout << str << std::endl;

	PRINT("This is print");
	LOG_DEBUG("This is debug");
	LOG_INFO("This is info");
	LOG_WARN("This is warn");
	LOG_ERROR("This is error");
}

TEST(Utils, Trace)
{
	LOG_INFO("Test Trace");
	std::string stackMsg = mirror::log::SysTrace::TraceInfo();
	LOG_DEBUG(stackMsg);
}

TEST(Utils, Files)
{
	fs::path path("../project1");
	std::vector<std::string> list;
	test::Utils::findFiles(list, path);

	for (auto &it : list)
	{
		LOG_DEBUG(it);
	}
}

TEST(Utils, Tuple)
{
	std::tuple<std::string, int, int> v = std::make_tuple("abc", 5, 6);
	LOG_DEBUG(std::get<0>(v));
}



