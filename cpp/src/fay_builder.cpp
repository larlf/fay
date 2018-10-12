#include "fay_builder.h"
#include "fay_builder.h"
#include <fay_builder.h>


using namespace fay;

fay::OutlineBuilder::OutlineBuilder(PTR(FayLib) lib)
{
	this->_lib = lib;
}

fay::OutlineBuilder::~OutlineBuilder()
{
}

void fay::OutlineBuilder::package(const std::string & name)
{
	this->_package = name;
}

void fay::OutlineBuilder::beginFile(const std::string & filename)
{
	this->_filename = filename;
}

void fay::OutlineBuilder::endFile()
{
	this->_filename = "";
}

void fay::OutlineBuilder::beginClass(const std::string & name)
{
	PTR(FayClass) clazz = MKPTR(FayClass)(this->_package, name);
	this->_class = clazz;
}

void fay::OutlineBuilder::endClass()
{
	this->_lib->addClass(this->_class);
}

void fay::OutlineBuilder::beginFun(const std::string & name, PTR(FayCode) code)
{
	this->_fun = MKPTR(FayFun)(this->_class, name, code);
}

void fay::OutlineBuilder::endFun()
{
	this->_class->addFun(this->_fun);
}

void fay::InstBuilder::addInst(PTR(FayInst) inst)
{
	this->_insts.push_back(inst);
}

std::vector<PTR(FayInst)> fay::InstBuilder::insts()
{
	return this->_insts;
}
