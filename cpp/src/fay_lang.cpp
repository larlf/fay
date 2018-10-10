#include <fay_lang.h>
#include <mirror_utils_log.h>

using namespace fay;

fay::FayLib::FayLib(const std::string &name)
	: _name(name)
{
}

fay::FayLib::~FayLib()
{
}

void fay::FayLib::addClass(PTR(FayClass) clazz)
{
	//LOG_DEBUG("Add class : " << clazz->name() << " to " << this->name());
	this->_classes.push_back(clazz);
}

PTR(FayFun) fay::FayLib::findFun(const std::string & className, const std::string & funName, std::vector<PTR(FayType)> paramsType)
{
	return nullptr;
}

void fay::FayLib::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayLib]")->add(this->_name)->endl();
	sb->increaseIndent();
	for each(auto it in this->_classes)
		it->toString(sb);
	sb->decreaseIndent();
}

fay::FayFun::~FayFun()
{
}

const std::string & fay::FayFun::fullname()
{
	return "";
}

void fay::FayFun::addParam(PTR(FayParamDef) p1)
{
	this->_params.push_back(p1);
}

bool fay::FayFun::matchParams(std::vector<PTR(FayType)> paramsType)
{
	if (paramsType.size() == this->_params.size())
	{
		//TODO
	}

	return false;
}

void fay::FayFun::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayFun]")->add(this->_name)->endl();

	for each(auto it in this->_params)
	{
		it->toString(sb);
	}
}

void fay::FayClass::addFun(PTR(FayFun) fun)
{
	//LOG_DEBUG("Add fun " << fun->name() << " to class " << this->name());
	this->_funs.push_back(fun);
}

PTR(FayFun) fay::FayClass::findFun(const std::string & funName, std::vector<PTR(FayType)> paramsType)
{
	return PTR(FayFun)();
}

void fay::FayClass::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayClass]")->add(this->_name)->endl();
	sb->increaseIndent();
	for each(auto it in this->_funs)
		it->toString(sb);
	sb->decreaseIndent();
}

void fay::FayDomain::addLib(PTR(FayLib) lib)
{
	this->_libs.push_back(lib);
	lib->domain = this->shared_from_this();
}

void fay::FayDomain::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayDomain]")->endl();
	sb->increaseIndent();
	for each(auto it in this->_libs)
		it->toString(sb);
	sb->decreaseIndent();
}

PTR(FayFun) fay::FayDomain::findFun(const std::string & className, const std::string & funName, std::vector<PTR(FayType)> paramsType)
{
	return nullptr;
}

void fay::FayParamDef::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayParamDef] ")->add(this->_name)->endl();
}
