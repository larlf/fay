#pragma once
#include <fay_lang.h>
#include <fay_stack.h>

namespace fay
{
	class FayVM
	{
	private:
		PTR(FayDomain) _domain;
		VMStack stack;

		void _run(PTR(FayInstFun) fun);

	public:
		FayVM(PTR(FayDomain) domain)
			: _domain(domain) {}

		void run(PTR(FayFun) fun);
	};
}