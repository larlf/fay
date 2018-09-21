#pragma once
#include <string>
#include <mirage/mirage_const.h>
#include "fay_runtime.h"

namespace fay
{
	enum FayValueType
	{
		Void,
		Integer,
		Long,
		Float,
		Double,
		String
	};

	union FayStackValue
	{
		uint64 value;
		int intValue;
		int64 longValue;
		float floatValue;
		double doubleValue;
		const std::string* strValue;
	};

	class FayStackItem
	{
	public:
		FayValueType type;
		FayStackValue value;

		FayStackItem() : type(FayValueType::Void)
		{
			this->value.value = 0;
		}

		FayStackItem(const std::string* str)
		{
			this->type = FayValueType::String;
			this->value.strValue = str;
		}
	};

	class FayStack
	{
	private:
		std::vector<FayStackItem> items;
		int pos;

	public:
		FayStack() : pos(0) {}

		void push(FayStackItem item);
		FayStackItem pop();
	};

	class FayVM
	{
	private:
		static FayStack* stack;

	public:
		static void Execute(std::vector<FayInst*> insts);
	};

	class InnerFun
	{
	public:
		static void Print(FayStack* stack, int argc);
	};
}


