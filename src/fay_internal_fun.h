#pragma once
#include <stack>
#include <fay_type.h>

namespace fay
{
	class InternalFun
	{
	public:
		static void Print_String(std::stack<FayValue> &stack);
		static void Print_Int(std::stack<FayValue> &stack);
	};
}

