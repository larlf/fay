#pragma once
#include <fay_lang.h>
#include <stack>

namespace fay
{
	class FayVM
	{
	private:
		static void _run(PTR(std::stack<FayValue>) stack, PTR(FayValue) &error, PTR(FayInstFun) fun);
		static void _run(PTR(std::stack<FayValue>) stack, PTR(FayValue) &error, PTR(FayFun) fun);

		//检查对象不为空
		static void checkObject(PTR(std::stack<FayValue>) stack)
		{
			if(stack->top().type() == ValueType::Void || stack->top().val()->ptrValue == nullptr)
				throw std::exception("Null pointer");
		}

	public:
		//对外部提供的运行接口，会把堆栈里的值返回
		static FayValue Run(PTR(FayFun) fun, FayValue &self, std::vector<FayValue> &params);
		static FayValue Run(PTR(FayFun) fun) { return FayVM::Run(fun, FayValue(), std::vector<FayValue>()); }
		static FayValue Run(PTR(FayFun) fun, FayValue &self) { return FayVM::Run(fun, self, std::vector<FayValue>()); }
	};
}