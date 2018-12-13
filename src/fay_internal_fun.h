#pragma once
#include <stack>
#include <fay_type.h>

namespace fay
{
	namespace internal
	{
		class InternalFun
		{
		public:
			static void ToString(PTR(std::stack<FayValue>) stack);
			static void Print_Bool(PTR(std::stack<FayValue>) stack);
			static void Print_Byte(PTR(std::stack<FayValue>) stack);
			static void Print_Int(PTR(std::stack<FayValue>) stack);
			static void Print_Long(PTR(std::stack<FayValue>) stack);
			static void Print_Float(PTR(std::stack<FayValue>) stack);
			static void Print_Double(PTR(std::stack<FayValue>) stack);
			static void Print_String(PTR(std::stack<FayValue>) stack);
			static void MSTime(PTR(std::stack<FayValue>) stack);
			static void StringToBool(PTR(std::stack<FayValue>) stack);
			static void StringToByte(PTR(std::stack<FayValue>) stack);
			static void StringToInt(PTR(std::stack<FayValue>) stack);
			static void StringToLong(PTR(std::stack<FayValue>) stack);
			static void StringToFloat(PTR(std::stack<FayValue>) stack);
			static void StringToDouble(PTR(std::stack<FayValue>) stack);
		};

	}
}

