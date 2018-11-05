﻿#pragma once
#include <fay_lang.h>
#include <stack>

namespace fay
{
	class FayVM
	{
	private:
		PTR(FayDomain) _domain;
		std::stack<PTR(FayValue)> stack;

		void _run(PTR(FayInstFun) fun);

	public:
		FayVM(PTR(FayDomain) domain)
			: _domain(domain) {}

		size_t stackSize() { return this->stack.size(); }

		void run(PTR(FayFun) fun);
	};
}