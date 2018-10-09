#include <fay_builder.h>

using namespace fay;

fay::OutlineBuilder::OutlineBuilder(PTR(FayLib) lib)
{
	this->_lib = lib;
}

fay::OutlineBuilder::~OutlineBuilder()
{
}

void fay::OutlineBuilder::bindClass(const std::string & name)
{
	PTR(FayClass) clazz = MKPTR(FayClass)(name);
	this->_lib->addClass(clazz);
	this->_class = clazz;
}
