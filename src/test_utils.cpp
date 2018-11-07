#include <test_main.h>
#include <mirror_utils_log.h>
#include <mirror_utils_lang.h>
#include <vector>

using namespace mirror;

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
