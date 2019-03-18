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
			void preInit();
			void postInit();
		};

		class ObjectClass : public FayClass
		{
			using FayClass::FayClass;
		public:
			virtual PTR(FayClass) initInternalFun();
			virtual ValueType valueType() override { return ValueType::Object; }
		};

		class VoidClass : public ObjectClass
		{
		public:
			VoidClass(PTR(FayLib) lib, const std::string &package, const std::string &name)
				: ObjectClass(lib, "", "void")
			{
				this->_fullname = TypeDict::ToName(ValueType::Void);
			}

			virtual ValueType valueType() override { return ValueType::Void; }
		};

		class BoolClass : public ObjectClass
		{
		public:
			BoolClass(PTR(FayLib) lib, const std::string &package, const std::string &name)
				: ObjectClass(lib, package, name)
			{
				this->_fullname = TypeDict::ToName(ValueType::Bool);
			}

			virtual ValueType valueType() override { return ValueType::Bool; }
		};

		class ByteClass : public ObjectClass
		{
		public:
			ByteClass(PTR(FayLib) lib, const std::string &package, const std::string &name)
				: ObjectClass(lib, package, name)
			{
				this->_fullname = TypeDict::ToName(ValueType::Byte);
			}

			virtual ValueType valueType() override { return ValueType::Byte; }
		};

		class IntClass : public ObjectClass
		{
		public:
			IntClass(PTR(FayLib) lib, const std::string &package, const std::string &name)
				: ObjectClass(lib, "", "int")
			{
				this->_fullname = TypeDict::ToName(ValueType::Int);
			}

			virtual ValueType valueType() override { return ValueType::Int; }
		};

		class LongClass : public ObjectClass
		{
		public:
			LongClass(PTR(FayLib) lib, const std::string &package, const std::string &name)
				: ObjectClass(lib, package, name)
			{
				this->_fullname = TypeDict::ToName(ValueType::Long);
			}

			virtual ValueType valueType() override { return ValueType::Long; }
		};

		class FloatClass : public ObjectClass
		{
		public:
			FloatClass(PTR(FayLib) lib, const std::string &package, const std::string &name)
				: ObjectClass(lib, package, name)
			{
				this->_fullname = TypeDict::ToName(ValueType::Float);
			}

			virtual ValueType valueType() override { return ValueType::Float; }
		};

		class DoubleClass : public ObjectClass
		{
		public:
			DoubleClass(PTR(FayLib) lib, const std::string &package, const std::string &name)
				: ObjectClass(lib, package, name)
			{
				this->_fullname = TypeDict::ToName(ValueType::Double);
			}

			virtual ValueType valueType() override { return ValueType::Double; }
		};

		class StringClass : public ObjectClass
		{
		public:
			StringClass(PTR(FayLib) lib, const std::string &package, const std::string &name)
				: ObjectClass(lib, package, name)
			{
				this->_fullname = TypeDict::ToName(ValueType::String);
			}

			virtual ValueType valueType() override { return ValueType::String; }
		};

		class IOClass : public ObjectClass
		{
			using ObjectClass::ObjectClass;
		public:
			virtual PTR(FayClass) initInternalFun() override;
		};

		class DateClass : public ObjectClass
		{
			using ObjectClass::ObjectClass;
		public:
			static void Now(PTR(std::stack<FayValue>) stack);

			virtual PTR(FayClass) initInternalFun() override;
		};

		class ErrorClass : public ObjectClass
		{
			using ObjectClass::ObjectClass;
		public:
			virtual PTR(FayClass) initInternalFun() override;
		};
	}


}
