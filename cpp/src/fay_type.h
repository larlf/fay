#pragma once
#include <mirror_sys_const.h>
#include <fay_const.h>
#include <fay_object.h>

//****************************
//Fay语言中的数据类型
//****************************

namespace fay
{
	//值类型中的联合体
	union FayValueUnion
	{
		bool boolVal;
		unsigned char byteVal;
		int32_t intVal;
		int64_t longVal;
		float flaotVal;
		double doubleVal;
		void *ptrValue;
		std::string *strVal;
	};

	//语言中的值类型
	class FayValue
	{
	private:
		ValueType _type;
		FayValueUnion _val;

	public:
		FayValue() : _type(ValueType::Void) {}
		FayValue(int32_t val) : _type(ValueType::Int) { _val.intVal = val; }
		FayValue(int64_t val) : _type(ValueType::Long) { _val.longVal = val; }
		FayValue(const std::string &str) : _type(ValueType::String) { _val.strVal = new std::string(str); }
		~FayValue()
		{
			switch(this->_type)
			{
				case ValueType::String:
					delete this->_val.strVal;
					break;
			}
		}

		const int32_t intVal() { return (this->_type == ValueType::Int) ? this->_val.intVal : 0; }
		const std::string strVal() { return (this->_type == ValueType::String) ? *this->_val.strVal : ""; }
	};

}