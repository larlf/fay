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

	class FayVM
	{
	private:
		std::stack<FayValue> stack;

		void _run(PTR(FayInstFun) fun);
		void _run(PTR(FayFun) fun);

	public:
		FayVM() {}

		size_t stackSize() { return this->stack.size(); }

		//对外部提供的运行接口，会把堆栈里的值返回
		FayValue run(PTR(FayFun) fun);
	};
}