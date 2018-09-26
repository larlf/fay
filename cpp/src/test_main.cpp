#include <iostream>
#include <mirror_util_time.h>
#include <mirror_util_log.h>
#include <string>

void main()
{
	std::string str = mirror::util::TimeUtils::MSTimeString();
	std::cout << str << std::endl;

	PRINT("This is print");
	LOG_DEBUG("This is debug");
	LOG_INFO("This is info");
	LOG_WARN("This is warn");
	LOG_ERROR("This is error");
}

