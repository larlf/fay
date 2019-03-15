#include "fay_internal_object.h"
#include <fay_internal_fun.h>
#include <mirror_utils_time.h>
#include <mirror_utils_lang.h>

using namespace fay;
using namespace mirror;

PTR(FayClass) fay::internal::ObjectClass::initInternalFun()
{
	PTR(FayInternalFun) fun;
	this->addFun(MKPTR(FayInternalFun)("toString", false, this->lib->findClass(ValueType::String), fay::internal::InternalFun::ToString));

	return this->shared_from_this();
}

void fay::internal::SystemLib::preInit()
{
	this->addClass(MKPTR(VoidClass)(this->shared_from_this(), "fay.system", "Void"));
	this->addClass(MKPTR(BoolClass)(this->shared_from_this(), "fay.system", "Bool"));
	this->addClass(MKPTR(ByteClass)(this->shared_from_this(), "fay.system", "Byte"));
	this->addClass(MKPTR(IntClass)(this->shared_from_this(), "fay.system", "Int"));
	this->addClass(MKPTR(LongClass)(this->shared_from_this(), "fay.system", "Long"));
	this->addClass(MKPTR(FloatClass)(this->shared_from_this(), "fay.system", "Float"));
	this->addClass(MKPTR(DoubleClass)(this->shared_from_this(), "fay.system", "Double"));
	this->addClass(MKPTR(StringClass)(this->shared_from_this(), "fay.system", "String"));
	this->addClass(MKPTR(ObjectClass)(this->shared_from_this(), "fay.system", "Object"));

	this->addClass(MKPTR(IOClass)(this->shared_from_this(), "fay.system", "IO"));
	this->addClass(MKPTR(DateClass)(this->shared_from_this(), "fay.system", "Date"));
	this->addClass(MKPTR(ErrorClass)(this->shared_from_this(), "fay.system", "Error"));
}

void fay::internal::SystemLib::postInit()
{
	for(const auto &it : this->classes.list())
		TOPTR(ObjectClass, it)->initInternalFun();
}

PTR(FayClass) fay::internal::IOClass::initInternalFun()
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
	fun = MKPTR(FayInternalFun)("Now", true, this->lib->findClass(ValueType::Long), DateClass::Now);
	this->addFun(fun);

	return this->shared_from_this();
}

PTR(FayClass) fay::internal::ErrorClass::initInternalFun()
{
	this->addVar("code", this->lib->findClass(ValueType::Int));
	this->addVar("message", this->lib->findClass(ValueType::String));
	return PTR(FayClass)();
}
