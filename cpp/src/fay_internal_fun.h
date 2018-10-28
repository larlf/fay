#pragma once
#include <fay_stack.h>

namespace fay
{
	class InternalFun
	{
	public:
		static void Print_String(VMStack *stack);
		static void Print_Int(VMStack *stack);
	};
}

