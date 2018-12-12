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

	class VoidClass : public ObjectClass
	{
	public:
		VoidClass(const std::string &package, const std::string &name)
			: ObjectClass(package, name)
		{
			this->_fullname = TypeDict::ToName(ValueType::Void);
		}

		virtual ValueType valueType() override { return ValueType::Void; }
	};

	class BoolClass : public ObjectClass
	{
	public:
		BoolClass(const std::string &package, const std::string &name)
			: ObjectClass(package, name)
		{
			this->_fullname = TypeDict::ToName(ValueType::Bool);
		}

		virtual ValueType valueType() override { return ValueType::Bool; }
	};

	class ByteClass : public ObjectClass
	{
	public:
		ByteClass(const std::string &package, const std::string &name)
			: ObjectClass(package, name)
		{
			this->_fullname = TypeDict::ToName(ValueType::Byte);
		}

		virtual ValueType valueType() override { return ValueType::Byte; }
	};

	class IntClass : public ObjectClass
	{
	public:
		IntClass(const std::string &package, const std::string &name)
			: ObjectClass(package, name)
		{
			this->_fullname = TypeDict::ToName(ValueType::Int);
		}

		virtual ValueType valueType() override { return ValueType::Int; }
	};

	class LongClass : public ObjectClass
	{
	public:
		LongClass(const std::string &package, const std::string &name)
			: ObjectClass(package, name)
		{
			this->_fullname = TypeDict::ToName(ValueType::Long);
		}

		virtual ValueType valueType() override { return ValueType::Long; }
	};

	class FloatClass : public ObjectClass
	{
	public:
		FloatClass(const std::string &package, const std::string &name)
			: ObjectClass(package, name)
		{
			this->_fullname = TypeDict::ToName(ValueType::Float);
		}

		virtual ValueType valueType() override { return ValueType::Float; }
	};

	class DoubleClass : public ObjectClass
	{
	public:
		DoubleClass(const std::string &package, const std::string &name)
			: ObjectClass(package, name)
		{
			this->_fullname = TypeDict::ToName(ValueType::Double);
		}

		virtual ValueType valueType() override { return ValueType::Double; }
	};

	class StringClass : public ObjectClass
	{
	public:
		StringClass(const std::string &package, const std::string &name)
			: ObjectClass(package, name)
		{
			this->_fullname = TypeDict::ToName(ValueType::String);
		}

		virtual ValueType valueType() override { return ValueType::String; }
	};

	class IOClass : public ObjectClass
	{
		using ObjectClass::ObjectClass;
	public:
		virtual PTR(FayClass) init() override;
	};

	class DateClass : public ObjectClass
	{
		using ObjectClass::ObjectClass;
	public:
		static void Now(std::stack<FayValue> &stack);

		virtual PTR(FayClass) init() override;
	};
}
