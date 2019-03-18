#include "fay_internal_object.h"
#include <fay_internal_fun.h>
#include <mirror_utils_time.h>
#include <mirror_utils_lang.h>

using namespace fay;
using namespace mirror;

void fay::internal::SystemLib::init()
{
	this->deps->addLib(this->shared_from_this());

	this->newClass<VoidClass>("", "void");
	this->newClass<BoolClass>("", "bool");
	this->newClass<ByteClass>("", "byte");
	this->newClass<IntClass>("", "int");
	this->newClass<LongClass>("", "long");
	this->newClass<FloatClass>("", "float");
	this->newClass<DoubleClass>("", "double");
	this->newClass<StringClass>("", "string");

	this->newClass<ObjectClass>("fay", "Object");
	this->newClass<IOClass>("fay", "IO");
	this->newClass<DateClass>("fay", "Date");
	this->newClass<ErrorClass>("fay", "Error");
}

void fay::internal::IOClass::init()
{
	PTR(FayInternalFun) fun;
	std::vector<PTR(FayParamDef)> params;

	params = { MKPTR(FayParamDef)("value", this->lib->findClass(ValueType::Bool)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Bool);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", this->lib->findClass(ValueType::Byte)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Byte);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", this->lib->findClass(ValueType::Int)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Int);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", this->lib->findClass(ValueType::Long)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Long);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", this->lib->findClass(ValueType::Float)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Float);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", this->lib->findClass(ValueType::Double)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Double);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", this->lib->findClass(ValueType::String)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_String);
	this->addFun(fun);
}

void fay::internal::DateClass::Now(PTR(std::stack<FayValue>) stack)
{
	int64_t t = utils::TimeUtils::MSTime();
	stack->push(FayValue(t));
}

void fay::internal::DateClass::init()
{
	PTR(FayInternalFun) fun;
	fun = MKPTR(FayInternalFun)("Now", true, this->lib->findClass(ValueType::Long), DateClass::Now);
	this->addFun(fun);
}

void fay::internal::ErrorClass::init()
{
	this->addVar("code", this->lib->findClass(ValueType::Int));
	this->addVar("message", this->lib->findClass(ValueType::String));
}

void fay::internal::ObjectClass::init()
{
	PTR(FayInternalFun) fun;
	this->addFun(MKPTR(FayInternalFun)("toString", false, this->lib->findClass(ValueType::String), fay::internal::InternalFun::ToString));
}
