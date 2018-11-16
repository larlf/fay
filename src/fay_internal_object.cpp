﻿#include "fay_internal_object.h"
#include <fay_internal_fun.h>

using namespace fay;

PTR(FayClass) fay::ObjectClass::init()
{
	PTR(FayInternalFun) fun;
	fun = MKPTR(FayInternalFun)(this->domain(), "toString", false, fay::InternalFun::ToString);
	fun->addReturns((*this->domain())[ValueType::String]);
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
	this->addClass(MKPTR(IOClass)(this->domain(), "fay.system", "IO"));
}

void fay::SystemLib::postInit()
{
	for(auto it : this->classes)
		it->init();
}

PTR(FayClass) fay::IOClass::init()
{
	PTR(FayInternalFun) fun;

	fun = MKPTR(FayInternalFun)(this->domain(), "print", true, fay::InternalFun::Print_Bool);
	fun->addParam(MKPTR(FayParamDef)(this->domain(), "value", (*this->domain())[ValueType::Bool]));
	this->addFun(fun);

	fun = MKPTR(FayInternalFun)(this->domain(), "print", true, fay::InternalFun::Print_Byte);
	fun->addParam(MKPTR(FayParamDef)(this->domain(), "value", (*this->domain())[ValueType::Byte]));
	this->addFun(fun);

	fun = MKPTR(FayInternalFun)(this->domain(), "print", true, fay::InternalFun::Print_Int);
	fun->addParam(MKPTR(FayParamDef)(this->domain(), "value", (*this->domain())[ValueType::Int]));
	this->addFun(fun);

	fun = MKPTR(FayInternalFun)(this->domain(), "print", true, fay::InternalFun::Print_Long);
	fun->addParam(MKPTR(FayParamDef)(this->domain(), "value", (*this->domain())[ValueType::Long]));
	this->addFun(fun);

	fun = MKPTR(FayInternalFun)(this->domain(), "print", true, fay::InternalFun::Print_Float);
	fun->addParam(MKPTR(FayParamDef)(this->domain(), "value", (*this->domain())[ValueType::Float]));
	this->addFun(fun);

	fun = MKPTR(FayInternalFun)(this->domain(), "print", true, fay::InternalFun::Print_Double);
	fun->addParam(MKPTR(FayParamDef)(this->domain(), "value", (*this->domain())[ValueType::Double]));
	this->addFun(fun);

	fun = MKPTR(FayInternalFun)(this->domain(), "print", true, fay::InternalFun::Print_String);
	fun->addParam(MKPTR(FayParamDef)(this->domain(), "value", (*this->domain())[ValueType::String]));
	this->addFun(fun);

	return this->shared_from_this();
}
