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

		std::vector<FayValue> run(PTR(FayFun) fun);
	};
}