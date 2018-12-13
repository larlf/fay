#pragma once
#include <fay_lang.h>
#include <stack>

namespace fay
{
	//运行时的异常
	class FayRuntimeException : public std::exception
	{
	public:
		FayRuntimeException(const std::string &msg) : std::exception(msg.c_str()) {}
	};

	class FayThread
	{

	};

	class FayVM
	{
	private:


		static void _run(PTR(std::stack<FayValue>) stack, PTR(FayInstFun) fun);
		static void _run(PTR(std::stack<FayValue>) stack, PTR(FayFun) fun);

	public:

		//对外部提供的运行接口，会把堆栈里的值返回
		static FayValue Run(PTR(FayFun) fun);
	};
}