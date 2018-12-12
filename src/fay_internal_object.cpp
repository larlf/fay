#include "fay_internal_object.h"
#include <fay_internal_fun.h>
#include <mirror_utils_time.h>
#include <mirror_utils_lang.h>

using namespace fay;
using namespace mirror;

PTR(FayClass) fay::ObjectClass::init()
{
	PTR(FayInternalFun) fun;
	fun = MKPTR(FayInternalFun)( "toString", false, fay::InternalFun::ToString);
	fun->returnValue(FayLang::Domain.findClass(ValueType::String));
	this->addFun(fun);

	return this->shared_from_this();
}

void fay::SystemLib::preInit()
{
	this->addClass(MKPTR(VoidClass)( "fay.system", "Void"));
	this->addClass(MKPTR(BoolClass)( "fay.system", "Bool"));
	this->addClass(MKPTR(ByteClass)( "fay.system", "Byte"));
	this->addClass(MKPTR(IntClass)( "fay.system", "Int"));
	this->addClass(MKPTR(LongClass)( "fay.system", "Long"));
	this->addClass(MKPTR(FloatClass)( "fay.system", "Float"));
	this->addClass(MKPTR(DoubleClass)( "fay.system", "Double"));
	this->addClass(MKPTR(StringClass)( "fay.system", "String"));
	this->addClass(MKPTR(ObjectClass)( "fay.system", "Object"));
	this->addClass(MKPTR(IOClass)( "fay.system", "IO"));
	this->addClass(MKPTR(DateClass)( "fay.system", "Date"));
}

void fay::SystemLib::postInit()
{
	for(auto it : this->classes)
		it->init();
}

PTR(FayClass) fay::IOClass::init()
{
	PTR(FayInternalFun) fun;
	std::vector<PTR(FayParamDef)> params;

	params = { MKPTR(FayParamDef)("value", FayLang::Domain[ValueType::Bool]) };
	fun = MKPTR(FayInternalFun)("Print", true, params, fay::InternalFun::Print_Bool);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", FayLang::Domain[ValueType::Byte]) };
	fun = MKPTR(FayInternalFun)( "Print", true, params, fay::InternalFun::Print_Byte);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", FayLang::Domain[ValueType::Int]) };
	fun = MKPTR(FayInternalFun)( "Print", true, params, fay::InternalFun::Print_Int);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", FayLang::Domain[ValueType::Long]) };
	fun = MKPTR(FayInternalFun)( "Print", true, params, fay::InternalFun::Print_Long);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", FayLang::Domain[ValueType::Float]) };
	fun = MKPTR(FayInternalFun)( "Print", true, params, fay::InternalFun::Print_Float);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", FayLang::Domain[ValueType::Double]) };
	fun = MKPTR(FayInternalFun)( "Print", true, params, fay::InternalFun::Print_Double);
	this->addFun(fun);

	params = { MKPTR(FayParamDef)("value", FayLang::Domain[ValueType::String]) };
	fun = MKPTR(FayInternalFun)( "Print", true, params, fay::InternalFun::Print_String);
	this->addFun(fun);

	return this->shared_from_this();
}

void fay::DateClass::Now(std::stack<FayValue> &stack)
{
	int64_t t = utils::TimeUtils::MSTime();
	stack.push(FayValue(t));
}

PTR(FayClass) fay::DateClass::init()
{
	PTR(FayInternalFun) fun;
	fun = MKPTR(FayInternalFun)( "Now", true, DateClass::Now);
	fun->returnValue(FayLang::Domain.findClass(ValueType::Long));
	this->addFun(fun);

	return this->shared_from_this();
}
