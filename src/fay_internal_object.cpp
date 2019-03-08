﻿#include "fay_internal_object.h"
#include <fay_internal_fun.h>
#include <mirror_utils_time.h>
#include <mirror_utils_lang.h>

using namespace fay;
using namespace mirror;

PTR(FayClass) fay::internal::ObjectClass::initInternalFun()
{
	PTR(FayInternalFun) fun;
	this->addFun(MKPTR(FayInternalFun)("toString", false, FayDomain::FindClass(ValueType::String), fay::internal::InternalFun::ToString));

	return this->shared_from_this();
}

void fay::internal::SystemLib::preInit()
{
	this->addClass(MKPTR(VoidClass)("fay.system", "Void"));
	this->addClass(MKPTR(BoolClass)("fay.system", "Bool"));
	this->addClass(MKPTR(ByteClass)("fay.system", "Byte"));
	this->addClass(MKPTR(IntClass)("fay.system", "Int"));
	this->addClass(MKPTR(LongClass)("fay.system", "Long"));
	this->addClass(MKPTR(FloatClass)("fay.system", "Float"));
	this->addClass(MKPTR(DoubleClass)("fay.system", "Double"));
	this->addClass(MKPTR(StringClass)("fay.system", "String"));
	this->addClass(MKPTR(ObjectClass)("fay.system", "Object"));

	this->addClass(MKPTR(IOClass)("fay.system", "IO"));
	this->addClass(MKPTR(DateClass)("fay.system", "Date"));
	this->addClass(MKPTR(ErrorClass)("fay.system", "Error"));
}

void fay::internal::SystemLib::postInit()
{
	for(const auto &it : this->classes)
		TOPTR(ObjectClass, it)->initInternalFun();
}

PTR(FayClass) fay::internal::IOClass::initInternalFun()
{
	PTR(FayInternalFun) fun;
	std::vector<PTR(FayParamDef)> params;

	params = { MKPTR(FayParamDef)("value", FayDomain::FindClass(ValueType::Bool)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Bool);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", FayDomain::FindClass(ValueType::Byte)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Byte);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", FayDomain::FindClass(ValueType::Int)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Int);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", FayDomain::FindClass(ValueType::Long)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Long);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", FayDomain::FindClass(ValueType::Float)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Float);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", FayDomain::FindClass(ValueType::Double)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_Double);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", FayDomain::FindClass(ValueType::String)) };
	fun = MKPTR(FayInternalFun)("Print", true, params, nullptr, fay::internal::InternalFun::Print_String);
	this->addFun(fun);

	return this->shared_from_this();
}

void fay::internal::DateClass::Now(PTR(std::stack<FayValue>) stack)
{
	int64_t t = utils::TimeUtils::MSTime();
	stack->push(FayValue(t));
}

PTR(FayClass) fay::internal::DateClass::initInternalFun()
{
	PTR(FayInternalFun) fun;
	fun = MKPTR(FayInternalFun)("Now", true, FayDomain::FindClass(ValueType::Long), DateClass::Now);
	this->addFun(fun);

	return this->shared_from_this();
}

PTR(FayClass) fay::internal::ErrorClass::initInternalFun()
{
	this->addVar("code", FayDomain::FindClass(ValueType::Int));
	this->addVar("message", FayDomain::FindClass(ValueType::String));
	return PTR(FayClass)();
}
