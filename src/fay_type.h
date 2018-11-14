#pragma once
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
		float floatVal;
		double doubleVal;
		std::string* strVal;
		void* ptrValue;
	};

	//语言中的值类型
	class FayValue // : public std::enable_shared_from_this<FayValue>
	{
	private:
		ValueType _type;
		FayValueUnion _val;

		//用于对值进行移动
		inline static void _move(FayValue &left, const FayValue &right)
		{
			switch(right._type)
			{
				case ValueType::String:
					left._val.strVal = new std::string(*right._val.strVal);
					break;
				default:
					left._val = right._val;
					break;
			}

			left._type = right._type;
		}

	public:
		FayValue() : _type(ValueType::Void) {  }
		FayValue(bool val) : _type(ValueType::Bool) {  _val.boolVal = val; }
		FayValue(unsigned char val) : _type(ValueType::Byte) { _val.byteVal = val; }
		FayValue(int32_t val) : _type(ValueType::Int) {  _val.intVal = val; }
		FayValue(int64_t val) : _type(ValueType::Long) {  _val.longVal = val; }
		FayValue(float val) : _type(ValueType::Float) {  _val.floatVal = val; }
		FayValue(double val) : _type(ValueType::Double) { _val.doubleVal = val; }
		FayValue(const char* str) : _type(ValueType::String) { _val.strVal = new std::string(str); }
		FayValue(const std::string &str) : _type(ValueType::String) { _val.strVal = new std::string(str); }

		inline FayValue(const FayValue &v)
		{
			_move(*this, v);
		}

		~FayValue()
		{
			//LOG_DEBUG("Destory " << this);

			switch(this->_type)
			{
				case ValueType::String:
					delete this->_val.strVal;
					break;
			}

			this->_type = ValueType::Void;
		}

		inline ValueType type() { return this->_type; }
		inline FayValueUnion* val() { return &this->_val; }
		inline bool boolVal() { return (this->_type == ValueType::Bool) ? this->_val.boolVal : false; }
		inline byte byteVal() { return (this->_type == ValueType::Byte) ? this->_val.byteVal : 0; }
		inline int32_t intVal() { return (this->_type == ValueType::Int) ? this->_val.intVal : 0; }
		inline int64_t longVal() { return (this->_type == ValueType::Long) ? this->_val.longVal : 0; }
		inline float floatVal() { return (this->_type == ValueType::Float) ? this->_val.floatVal : 0; }
		inline double doubleVal() { return (this->_type == ValueType::Double) ? this->_val.doubleVal : 0; }
		inline std::string* strVal() { return (this->_type == ValueType::String) ? this->_val.strVal : nullptr; }
		inline void* objectVal() { return (this->_type == ValueType::Object) ? this->_val.ptrValue : nullptr; }
		inline void* funVal() { return (this->_type == ValueType::Function) ? this->_val.ptrValue : nullptr; }
		template<class T>
		inline T* ptrValue() { return (T*)this->_val.ptrValue; }

		void operator =(const FayValue &value)
		{
			_move(*this, value);
		}
	};

}