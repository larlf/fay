#pragma once

#include <iostream>
#include <string>

namespace mirror
{
	namespace util
	{
		class TimeUtils
		{
		public:
			//时间戳，精确到秒
			static uint32_t Time();
			//时间戳，精确到毫秒
			static uint64_t MSTime();
			//生成时间字符串
			static std::string TimeString(uint32_t t = 0);
			//生成豪秒级的时间字符串
			static std::string MSTimeString(uint64_t t = 0);
		};
	}
}


