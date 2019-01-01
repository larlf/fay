#pragma once

#include <fay_object.h>
#include <fay_lang.h>

namespace fay
{
	//编译结果
	class FayCompileResult
	{
	public:
		void warn(PTR(FayPart) part, const std::string &msg);
		void error(PTR(FayPart) part, const std::string &msg);
	};

	//编译任务，多个任务可能会返回到一个编译结果里
	class FayCompileTask
	{

	};
}