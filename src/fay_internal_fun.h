#pragma once
#include <stack>
#include <fay_type.h>

namespace fay
{
	class InternalFun
	{
	public:
		static void Print_Bool(std::stack<PTR(FayValue)> &stack);
		static void Print_Byte(std::stack<PTR(FayValue)> &stack);
		static void Print_Int(std::stack<PTR(FayValue)> &stack);
		static void Print_Long(std::stack<PTR(FayValue)> &stack);
		static void Print_Float(std::stack<PTR(FayValue)> &stack);
		static void Print_Double(std::stack<PTR(FayValue)> &stack);
		static void Print_String(std::stack<PTR(FayValue)> &stack);
	};
}

