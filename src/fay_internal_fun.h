#pragma once
#include <stack>
#include <fay_type.h>

namespace fay
{
	class InternalFun
	{
	public:
		static void ToString(std::stack<FayValue> &stack);
		static void Print_Bool(std::stack<FayValue> &stack);
		static void Print_Byte(std::stack<FayValue> &stack);
		static void Print_Int(std::stack<FayValue> &stack);
		static void Print_Long(std::stack<FayValue> &stack);
		static void Print_Float(std::stack<FayValue> &stack);
		static void Print_Double(std::stack<FayValue> &stack);
		static void Print_String(std::stack<FayValue> &stack);
		static void MSTime(std::stack<FayValue> &stack);
		static void StringToBool(std::stack<FayValue> &stack);
		static void StringToByte(std::stack<FayValue> &stack);
		static void StringToInt(std::stack<FayValue> &stack);
		static void StringToLong(std::stack<FayValue> &stack);
		static void StringToFloat(std::stack<FayValue> &stack);
		static void StringToDouble(std::stack<FayValue> &stack);
	};
}

