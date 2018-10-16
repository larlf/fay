#include <fay_builder.h>
#include <mirror_utils_log.h>

using namespace fay;

void fay::FayBuilder::package(const std::string & name)
{
	this->_package = name;
}

void fay::FayBuilder::beginFile(const std::string & filename)
{
	this->_filename = filename;
}

void fay::FayBuilder::endFile()
{
	this->_filename = "";
}

void fay::FayBuilder::beginLib(const std::string & name)
{
	this->_lib = MKPTR(FayLib)(name);
	this->_domain->addLib(this->_lib);
}

pos_t fay::FayBuilder::beginClass(const std::string & name)
{
	PTR(FayClass) clazz = MKPTR(FayClass)(this->_package, name);
	this->_class = clazz;
	return this->_lib->addClass(clazz);
}

void fay::FayBuilder::bindClass(pos_t index)
{
	this->_class = TOPTR(FayClass, this->_domain->findType(index));
}

pos_t fay::FayBuilder::beginFun(const std::string & name)
{
	this->_fun = MKPTR(FayFun)(name);
	return this->_class->addFun(this->_fun);
}

void fay::FayBuilder::bindFun(pos_t index)
{
	if (index < 0)
	{
		LOG_ERROR("Bad fun index : "<<index);
		return;
	}

	this->_fun = this->_class->findFun(index);
}

void fay::FayBuilder::addParamDefine(const std::string & name, const std::string & type)
{
	PTR(FayType) t=this->_domain->findType(type);
	PTR(FayParamDef) def = MKPTR(FayParamDef)(name, t);
	this->_fun->addParam(def);
}
