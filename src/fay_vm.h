#pragma once
#include <fay_lang.h>
#include <stack>

namespace fay
{
	class FayVM
	{
	private:
		PTR(FayDomain) _domain;
		std::stack<FayValue> stack;

		void _run(PTR(FayInstFun) fun);
		void _run(PTR(FayFun) fun);

	public:
		FayVM(PTR(FayDomain) domain)
			: _domain(domain) {}

		size_t stackSize() { return this->stack.size(); }

		//对外部提供的运行接口，会把堆栈里的值返回
		FayValue run(PTR(FayFun) fun);
	};
}