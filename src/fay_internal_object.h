#pragma once
#include <fay_lang.h>

namespace fay
{
	namespace internal
	{
		class SystemLib : public FayLib
		{
			using FayLib::FayLib;
		public:
			SystemLib() : FayLib("fay", MKPTR(FayLibSet)(), 1, 1) {}
			virtual void onAddToDomain() override;
		};

		class ObjectClass : public FayClass
		{
			using FayClass::FayClass;
		public:
			virtual void onAddToLib(PTR(FayLib) lib) override;
			virtual ValueType valueType() override { return ValueType::Object; }
		};

		class VoidClass : public FayClass
		{
		public:
			VoidClass()
				: FayClass("", "void") {}

			virtual ValueType valueType() override { return ValueType::Void; }
		};

		class BoolClass : public FayClass
		{
		public:
			BoolClass()
				: FayClass("", "bool") {}

			virtual ValueType valueType() override { return ValueType::Bool; }
		};

		class ByteClass : public FayClass
		{
		public:
			ByteClass()
				: FayClass("", "byte") {}

			virtual ValueType valueType() override { return ValueType::Byte; }
		};

		class IntClass : public FayClass
		{
		public:
			IntClass()
				: FayClass("", "int") {}

			virtual ValueType valueType() override { return ValueType::Int; }
		};

		class LongClass : public FayClass
		{
		public:
			LongClass()
				: FayClass("", "long") {}

			virtual ValueType valueType() override { return ValueType::Long; }
		};

		class FloatClass : public FayClass
		{
		public:
			FloatClass()
				: FayClass("", "float") {}

			virtual ValueType valueType() override { return ValueType::Float; }
		};

		class DoubleClass : public FayClass
		{
		public:
			DoubleClass()
				: FayClass("", "double") {}

			virtual ValueType valueType() override { return ValueType::Double; }
		};

		class StringClass : public FayClass
		{
		public:
			StringClass()
				: FayClass("", "string") {}

			virtual ValueType valueType() override { return ValueType::String; }
		};

		class IOClass : public ObjectClass
		{
			using ObjectClass::ObjectClass;

		public:
			virtual void onAddToLib(PTR(FayLib) lib) override;
		};

		class DateClass : public ObjectClass
		{
			using ObjectClass::ObjectClass;

		public:
			static void Now(PTR(std::stack<FayValue>) stack);

			virtual void onAddToLib(PTR(FayLib) lib) override;
		};

		class ErrorClass : public ObjectClass
		{
			using ObjectClass::ObjectClass;

		public:
			virtual void onAddToLib(PTR(FayLib) lib) override;
		};
	}


}
