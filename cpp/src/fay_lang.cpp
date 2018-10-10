#include <fay_lang.h>
#include <mirror_utils_log.h>

fay::FayLib::FayLib(const std::string &name)
	: _name(name)
{
}

fay::FayLib::~FayLib()
{
}

void fay::FayLib::addClass(PTR(FayClass) clazz)
{
	LOG_DEBUG("Add class : " << clazz->name() << " to " << this->name());
	this->_classes.push_back(clazz);
}

fay::FayFun::~FayFun()
{
}

void fay::FayClass::addFun(PTR(FayFun) fun)
{
	LOG_DEBUG("Add fun " << fun->name() << " to class " << this->name());
	this->_funs.push_back(fun);
}
