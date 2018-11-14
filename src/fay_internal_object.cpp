#include "fay_internal_object.h"
#include <fay_internal_fun.h>

using namespace fay;

PTR(FayClass) fay::ObjectClass::init()
{
	PTR(FayInternalFun) fun=MKPTR(FayInternalFun)(this->domain(), "toString", false, fay::InternalFun::ToString);
	fun->addReturns(this->domain()->findClass("string"));
	this->addFun(fun);

	return this->shared_from_this();
}

PTR(FayLib) fay::SystemLib::init1()
{
	PTR(ObjectClass) type = MKPTR(ObjectClass)(this->domain(), "fay.system", "Object");
	this->addClass(type);

	return this->shared_from_this();
}

void fay::SystemLib::init2()
{
}
