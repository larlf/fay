#include "fay_internal_object.h"
#include <fay_internal_fun.h>
#include <mirror_utils_time.h>
#include <mirror_utils_lang.h>

using namespace fay;
using namespace mirror;

void fay::internal::SystemLib::onAddToDomain()
{
	this->deps->addLib(this->shared_from_this());

	this->addClass(MKPTR(VoidClass)());
	this->addClass(MKPTR(BoolClass)());
	this->addClass(MKPTR(ByteClass)());
	this->addClass(MKPTR(IntClass)());
	this->addClass(MKPTR(LongClass)());
	this->addClass(MKPTR(FloatClass)());
	this->addClass(MKPTR(DoubleClass)());
	this->addClass(MKPTR(StringClass)());
	this->addClass(MKPTR(ObjectClass)("fay", "Object"));
	this->addClass(MKPTR(IOClass)("fay", "IO"));
	this->addClass(MKPTR(DateClass)("fay", "Date"));
	this->addClass(MKPTR(ErrorClass)("fay", "Error"));
}

void fay::internal::IOClass::onAddToLib(PTR(FayLib) lib)
{
	FayClass::onAddToLib(lib);

	PTR(FayInternalFun) fun;
	std::vector<PTR(FayParamDef)> params;

	params = { MKPTR(FayParamDef)("value", this->lib.lock()->findClass(ValueType::Bool)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Bool);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", this->lib.lock()->findClass(ValueType::Byte)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Byte);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", this->lib.lock()->findClass(ValueType::Int)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Int);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", this->lib.lock()->findClass(ValueType::Long)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Long);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", this->lib.lock()->findClass(ValueType::Float)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Float);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", this->lib.lock()->findClass(ValueType::Double)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Double);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", this->lib.lock()->findClass(ValueType::String)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_String);
	this->addFun(fun);
}

void fay::internal::DateClass::Now(PTR(std::stack<FayValue>) stack)
{
	int64_t t = utils::TimeUtils::MSTime();
	stack->push(FayValue(t));
}

void fay::internal::DateClass::onAddToLib(PTR(FayLib) lib)
{
	FayClass::onAddToLib(lib);

	PTR(FayInternalFun) fun;
	fun = MKPTR(FayInternalFun)("Now", true, this->lib.lock()->findClass(ValueType::Long), DateClass::Now);
	this->addFun(fun);
}

void fay::internal::ErrorClass::onAddToLib(PTR(FayLib) lib)
{
	FayClass::onAddToLib(lib);

	this->addVar("code", this->lib.lock()->findClass(ValueType::Int));
	this->addVar("message", this->lib.lock()->findClass(ValueType::String));
}

void fay::internal::ObjectClass::onAddToLib(PTR(FayLib) lib)
{
	FayClass::onAddToLib(lib);

	PTR(FayInternalFun) fun;
	this->addFun(MKPTR(FayInternalFun)("toString", false, this->lib.lock()->findClass(ValueType::String), fay::internal::InternalFun::ToString));
}

