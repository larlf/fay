#pragma once
#include <fay_lang.h>

namespace fay
{
	class SystemLib : public FayLib
	{
		using FayLib::FayLib;
	public:
		PTR(FayLib) init1();
		void init2();
	};

	class ObjectClass : public FayClass
	{
		using FayClass::FayClass;
	public:
		virtual PTR(FayClass) init();
	};

	class BoolClass : public ObjectClass
	{
		using ObjectClass::ObjectClass;
	};

	class byteClass : public ObjectClass
	{
		using ObjectClass::ObjectClass;
	};

	class IntClass : public ObjectClass
	{
		using ObjectClass::ObjectClass;
	};

	class LongClass : public ObjectClass
	{
		using ObjectClass::ObjectClass;
	};

	class FloatClass : public ObjectClass
	{
		using ObjectClass::ObjectClass;
	};

	class DoubleClass : public ObjectClass
	{
		using ObjectClass::ObjectClass;
	};

	class StringClass : public ObjectClass
	{
		using ObjectClass::ObjectClass;
	};
}
