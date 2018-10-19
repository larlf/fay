#pragma once
#include <mirror_sys_const.h>
#include <fay_const.h>
#include <fay_object.h>

//语言中的数据类型

namespace fay
{
	//字类型的联合体
	union FayValueUnion
	{
		bool boolVal;
		unsigned char byteVal;
		int32_t intVal;
		int64_t longVal;
		float flaotVal;
		double doubleVal;
		void* ptrValue;
		std::string* strVal;
	};


	class FayValueType
	{
	private:
		ValueType _type;
		FayValueUnion _value;

	public:
		FayValueType(int32_t val) : _type(ValueType::Int) { _value.intVal = val; }
		~FayValueType()
		{
			switch(this->_type)
			{
			}
		}

		int32_t intVal() { return (this->_type == ValueType::Int) ? this->_value.intVal : 0; }
	};

}