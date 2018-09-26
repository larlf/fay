#pragma once

#ifdef WIN32
#include <sys/timeb.h>
#include <Winsock2.h>
#include <Windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif
#include <time.h>
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <functional>
#include <sstream>
#include <memory>
#include <algorithm>
#include <cctype>
#include <locale>
#include <mirror_sys_termcolor.h>

/*******************************************
特殊数据类型定义
*******************************************/

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

	//重新定义的数据类型
	typedef unsigned char		byte;		//字节数据类型

#ifdef __cplusplus
}
#endif

namespace mirror
{
	namespace sys
	{
		//这里用来处理一些平台底层的接口
		class SystemInterface
		{
		public:
			//时间戳，精确到秒
			static uint32_t Time();
			//时间戳，精确到毫秒
			static uint64_t MSTime();
		};
	}

}

