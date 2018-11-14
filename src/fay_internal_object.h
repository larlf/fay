#pragma once
#include <fay_lang.h>

namespace fay
{
	class SystemLib : public FayLib
	{
		using FayLib::FayLib;
	public:
		void preInit();
		void postInit();
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
	public:
		virtual ValueType valueType() override { return ValueType::Bool; }
		virtual const std::string & fullname() override { return TypeDict::ToName(ValueType::Bool); }
	};

	class ByteClass : public ObjectClass
	{
		using ObjectClass::ObjectClass;
	public:
		virtual ValueType valueType() override { return ValueType::Byte; }
		virtual const std::string & fullname() override { return TypeDict::ToName(ValueType::Byte); }
	};

	class IntClass : public ObjectClass
	{
		using ObjectClass::ObjectClass;
	public:
		virtual ValueType valueType() override { return ValueType::Int; }
		virtual const std::string & fullname() override { return TypeDict::ToName(ValueType::Int); }
	};

	class LongClass : public ObjectClass
	{
		using ObjectClass::ObjectClass;
	public:
		virtual ValueType valueType() override { return ValueType::Long; }
		virtual const std::string & fullname() override { return TypeDict::ToName(ValueType::Long); }
	};

	class FloatClass : public ObjectClass
	{
		using ObjectClass::ObjectClass;
	public:
		virtual ValueType valueType() override { return ValueType::Float; }
		virtual const std::string & fullname() override { return TypeDict::ToName(ValueType::Float); }
	};

	class DoubleClass : public ObjectClass
	{
		using ObjectClass::ObjectClass;
	public:
		virtual ValueType valueType() override { return ValueType::Double; }
		virtual const std::string & fullname() override { return TypeDict::ToName(ValueType::Double); }
	};

	class StringClass : public ObjectClass
	{
		using ObjectClass::ObjectClass;
	public:
		virtual ValueType valueType() override { return ValueType::String; }
		virtual const std::string & fullname() override { return TypeDict::ToName(ValueType::String); }
	};
}
