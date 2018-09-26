#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <functional>
#ifdef WIN32
#include <sys/timeb.h>
#include <Winsock2.h>
#include <Windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif
#include "mirror_termcolor.h"

/*******************************************
特殊数据类型定义
*******************************************/

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

	//重新定义的数据类型
	typedef unsigned char		byte;		///< 字节数据类型

#ifdef __cplusplus
}
#endif

namespace mirror
{
	class SystemInterface
	{
	public:
		static uint64_t MSTime();
	};
}

