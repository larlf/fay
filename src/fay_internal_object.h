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
			virtual void init() override;
		};

		class ObjectClass : public FayClass
		{
			using FayClass::FayClass;
		public:
			virtual void init() override;
			virtual ValueType valueType() override { return ValueType::Object; }
		};

		class VoidClass : public FayClass
		{
		public:
			VoidClass(PTR(FayLib) lib)
				: FayClass(lib, "", "void") {}

			virtual ValueType valueType() override { return ValueType::Void; }
		};

		class BoolClass : public FayClass
		{
		public:
			BoolClass(PTR(FayLib) lib)
				: FayClass(lib, "", "bool") {}

			virtual ValueType valueType() override { return ValueType::Bool; }
		};

		class ByteClass : public FayClass
		{
		public:
			ByteClass(PTR(FayLib) lib)
				: FayClass(lib, "", "byte") {}

			virtual ValueType valueType() override { return ValueType::Byte; }
		};

		class IntClass : public FayClass
		{
		public:
			IntClass(PTR(FayLib) lib)
				: FayClass(lib, "", "int") {}

			virtual ValueType valueType() override { return ValueType::Int; }
		};

		class LongClass : public FayClass
		{
		public:
			LongClass(PTR(FayLib) lib)
				: FayClass(lib, "", "long") {}

			virtual ValueType valueType() override { return ValueType::Long; }
		};

		class FloatClass : public FayClass
		{
		public:
			FloatClass(PTR(FayLib) lib)
				: FayClass(lib, "", "float") {}

			virtual ValueType valueType() override { return ValueType::Float; }
		};

		class DoubleClass : public FayClass
		{
		public:
			DoubleClass(PTR(FayLib) lib)
				: FayClass(lib, "", "double") {}

			virtual ValueType valueType() override { return ValueType::Double; }
		};

		class StringClass : public FayClass
		{
		public:
			StringClass(PTR(FayLib) lib)
				: FayClass(lib, "", "string") {}

			virtual ValueType valueType() override { return ValueType::String; }
		};

		class IOClass : public ObjectClass
		{
			using ObjectClass::ObjectClass;

		public:
			virtual void init() override;
		};

		class DateClass : public ObjectClass
		{
			using ObjectClass::ObjectClass;

		public:
			static void Now(PTR(std::stack<FayValue>) stack);

			virtual void init() override;
		};

		class ErrorClass : public ObjectClass
		{
			using ObjectClass::ObjectClass;

		public:
			virtual void init() override;
		};
	}


}
