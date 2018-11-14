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

void fay::SystemLib::preInit()
{
	this->addClass(MKPTR(BoolClass)(this->domain(), "fay.system", "Bool"));
	this->addClass(MKPTR(ByteClass)(this->domain(), "fay.system", "Byte"));
	this->addClass(MKPTR(IntClass)(this->domain(), "fay.system", "Int"));
	this->addClass(MKPTR(LongClass)(this->domain(), "fay.system", "Long"));
	this->addClass(MKPTR(FloatClass)(this->domain(), "fay.system", "Float"));
	this->addClass(MKPTR(DoubleClass)(this->domain(), "fay.system", "Double"));
	this->addClass(MKPTR(StringClass)(this->domain(), "fay.system", "String"));
	this->addClass(MKPTR(ObjectClass)(this->domain(), "fay.system", "Object"));
}

void fay::SystemLib::postInit()
{
	for (auto it : this->classes)
	{
		it->init();
	}
}
