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
		float flaotVal;
		double doubleVal;
		std::string* strVal;
		void* ptrValue;
	};

	//语言中的值类型
	class FayValue
	{
	private:
		ValueType _class;
		FayValueUnion _val;

	public:
		FayValue(const FayValue &value)
		{
			if(value._class == ValueType::String)
			{
				this->_class = value._class;
				this->_val.strVal = new std::string(*value._val.strVal);
			}
			else
			{
				this->_class = value._class;
				this->_val = value._val;
			}
		}

		FayValue() : _class(ValueType::Void) {}
		FayValue(bool val) : _class(ValueType::Bool) { _val.boolVal = val; }
		FayValue(unsigned char val) : _class(ValueType::Byte) { _val.byteVal = val; }
		FayValue(int32_t val) : _class(ValueType::Int) { _val.intVal = val; }
		FayValue(int64_t val) : _class(ValueType::Long) { _val.longVal = val; }
		FayValue(float val) : _class(ValueType::Float) { _val.flaotVal = val; }
		FayValue(double val) : _class(ValueType::Double) { _val.doubleVal = val; }
		FayValue(const char* str) : _class(ValueType::String) { _val.strVal = new std::string(str); }
		FayValue(const std::string &str) : _class(ValueType::String) { _val.strVal = new std::string(str); }
		~FayValue()
		{
			this->reset();
		}

		inline void reset()
		{
			switch (this->_class)
			{
			case ValueType::String:
				delete this->_val.strVal;
				break;
			}

			this->_class = ValueType::Void;
		}

		inline const ValueType type() { return this->_class; }
		inline const int32_t intVal() { return (this->_class == ValueType::Int) ? this->_val.intVal : 0; }
		inline const int64_t longValue() { return (this->_class == ValueType::Long) ? this->_val.longVal : 0; }
		inline const float floatValue() { return (this->_class == ValueType::Float) ? this->_val.flaotVal : 0; }
		inline const double doubleValue() { return (this->_class == ValueType::Double) ? this->_val.doubleVal : 0; }
		inline const std::string* strVal() { return (this->_class == ValueType::String) ? this->_val.strVal : nullptr; }
		inline const void* objectVal() { return (this->_class == ValueType::Object) ? this->_val.ptrValue : nullptr; }
		inline const void* funVal() { return (this->_class == ValueType::Function) ? this->_val.ptrValue : nullptr; }
		template<class T>
		inline const T* ptrValue() { return (T*)this->_val.ptrValue; }

		//FayValue &operator =(const FayValue& value)
		//{
		//	if (value._type == ValueType::String)
		//	{
		//		this->_type = value._type;
		//		this->_val.strVal = new std::string(*value._val.strVal);
		//	}
		//	else
		//	{
		//		this->_type = value._type;
		//		this->_val = value._val;
		//	}

		//	return *this;
		//}

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
			switch(this->_class)
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
				{
					std::string* str = this->_val.strVal;
					if (str == nullptr)
					{
						this->_val.boolVal = false;
					}
					else
					{
						if (str->size() <= 0)
							this->_val.boolVal = false;
						else
							this->_val.boolVal = true;

						delete str;
					}
					break;
				}
				case ValueType::Object:
					this->_val.boolVal = (this->_val.ptrValue == nullptr) ? false : true;
					break;
				case ValueType::Function:
					this->_val.boolVal = (this->_val.ptrValue == nullptr) ? false : true;
					break;
				default:
					LOG_ERROR("Cannot convert " << (int)this->_class << "to bool");
					this->_val.boolVal = false;
					break;
			}

			this->_class = ValueType::Bool;
		}


	};

}