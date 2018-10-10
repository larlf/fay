#pragma once
#include <mirror_sys_const.h>
#include <fay_const.h>

//语言中的数据类型

namespace fay
{
	//数据类型
	class FayType
	{
	public:
		virtual const std::string &toString() { return ""; }
	};

	//简单类型
	class SimpleType : public FayType
	{
	private:
		static MAP<ValueType, PTR(FayType)> _Types;

		ValueType _type;

		//这种类型，不能从外部创建
		SimpleType(ValueType valueType)
			: _type(valueType) {}

	public:
		static PTR(FayType) Get(ValueType valueType);
	};
}