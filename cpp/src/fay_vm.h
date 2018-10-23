#pragma once
#include <fay_lang.h>
#include <fay_stack.h>

namespace fay
{
	class FayVM
	{
	private:
		VMStack stack;

	public:
		void run(std::vector<fay::FayInst *> *insts, PTR(FayLib) lib);
	};
}