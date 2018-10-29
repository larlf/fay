﻿#pragma once
#include <mirror_sys_const.h>
#include <mirror_utils_log.h>
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
		std::string* strVal;
		void* ptrValue;
	};

	//语言中的值类型
	class FayValue
	{
	private:
		ValueType _type;
		FayValueUnion _val;

	public:
		FayValue() : _type(ValueType::Void) {}
		FayValue(bool val) : _type(ValueType::Bool) { _val.boolVal = val; }
		FayValue(unsigned char val) : _type(ValueType::Byte) { _val.byteVal = val; }
		FayValue(int32_t val) : _type(ValueType::Int) { _val.intVal = val; }
		FayValue(int64_t val) : _type(ValueType::Long) { _val.longVal = val; }
		FayValue(float val) : _type(ValueType::Float) { _val.flaotVal = val; }
		FayValue(double val) : _type(ValueType::Double) { _val.doubleVal = val; }
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

		inline const ValueType type() { return this->_type; }
		inline const int32_t intVal() { return (this->_type == ValueType::Int) ? this->_val.intVal : 0; }
		inline const int64_t longValue() { return (this->_type == ValueType::Long) ? this->_val.longVal : 0; }
		inline const float floatValue() { return (this->_type == ValueType::Float) ? this->_val.flaotVal : 0; }
		inline const double doubleValue() { return (this->_type == ValueType::Double) ? this->_val.doubleVal : 0; }
		inline const std::string* strVal() { return (this->_type == ValueType::String) ? this->_val.strVal : nullptr; }
		inline const void* objectVal() { return (this->_type == ValueType::Object) ? this->_val.ptrValue : nullptr; }
		inline const void* funVal() { return (this->_type == ValueType::Function) ? this->_val.ptrValue : nullptr; }
		template<class T>
		inline const T* ptrValue() { return (T*)this->_val.ptrValue; }

		//switch (this->_type)
		//{
		//case ValueType::Void:
		//	break;
		//case ValueType::Bool:
		//	break;
		//case ValueType::Byte:
		//	break;
		//case ValueType::Int:
		//	break;
		//case ValueType::Long:
		//	break;
		//case ValueType::Float:
		//	break;
		//case ValueType::Double:
		//	break;
		//case ValueType::String:
		//	break;
		//case ValueType::Object:
		//	break;
		//case ValueType::Function:
		//	break;
		//default:
		//	LOG_ERROR("Cannot convert " << (int)this->_type << " to bool");
		//	break;
		//}

		inline const void convertToBool()
		{
			switch(this->_type)
			{
				case ValueType::Void:
					this->_val.boolVal = false;
					break;
				case ValueType::Bool:
					break;
				case ValueType::Byte:
					this->_val.boolVal = (this->_val.byteVal == 0) ? false : true;
					break;
				case ValueType::Int:
					this->_val.boolVal = (this->_val.intVal == 0) ? false : true;
					break;
				case ValueType::Long:
					this->_val.boolVal = (this->_val.longVal == 0) ? false : true;
					break;
				case ValueType::Float:
					this->_val.boolVal = (this->_val.flaotVal == 0) ? false : true;
					break;
				case ValueType::Double:
					this->_val.boolVal = (this->_val.doubleVal == 0) ? false : true;
					break;
				case ValueType::String:
					if(this->_val.strVal == nullptr || this->_val.strVal->size() <= 0)
						this->_val.boolVal = false;
					else
						this->_val.boolVal = true;
					break;
				case ValueType::Object:
					this->_val.boolVal = (this->_val.ptrValue == nullptr) ? false : true;
					break;
				case ValueType::Function:
					this->_val.boolVal = (this->_val.ptrValue == nullptr) ? false : true;
					break;
				default:
					LOG_ERROR("Cannot convert " << (int)this->_type << "to bool");
					this->_val.boolVal = false;
					break;
			}

			this->_type = ValueType::Bool;
		}


	};

}