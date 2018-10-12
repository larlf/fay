#include "fay_lang.h"
#include <fay_lang.h>
#include <mirror_utils_log.h>

using namespace fay;

const void fay::FayLib::addClass(PTR(FayClass) clazz)
{
	this->classes.push_back(clazz);
}

PTR(FayFun) fay::FayLib::findFun(const std::string & className, const std::string & funName, std::vector<PTR(FayType)> paramsType)
{
	return nullptr;
}

void fay::FayLib::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayLib]")->add(this->name)->endl();
	sb->increaseIndent();
	for each(auto it in this->classes)
		it->toString(sb);
	sb->decreaseIndent();
}

fay::FayFun::~FayFun()
{
}

const std::string & fay::FayFun::fullname()
{
	if (this->_fullname.size() <= 0)
	{
		std::string str;
		for each(auto it in this->_params)
		{
			if (str.size() > 0)
				str += ",";
			str += it->fullname();
		}

		this->_fullname += this->clazz.lock()->fullname();
		this->_fullname += ".";
		this->_fullname += this->_name;
		this->_fullname += "(" + str + ")";
	}

	return this->_fullname;
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
	sb->add("[FayFun]")->add(this->fullname())->endl();
}

void fay::FayClass::addFun(PTR(FayFun) fun)
{
	//LOG_DEBUG("Add fun " << fun->name() << " to class " << this->name());
	this->funs.push_back(fun);
}

PTR(FayFun) fay::FayClass::findFun(const std::string & funName, std::vector<PTR(FayType)> paramsType)
{
	return PTR(FayFun)();
}

void fay::FayClass::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayClass]")->add(this->_fullname)->endl();
	sb->increaseIndent();
	for each(auto it in this->funs)
		it->toString(sb);
	sb->decreaseIndent();
}

const std::string & fay::FayClass::fullname()
{
	return this->_fullname;
}

fay::FayDomain::FayDomain()
{
	//内置类型在这里创建
	this->_types.add("byte", SimpleType::Get(ValueType::Byte));
	this->_types.add("int", SimpleType::Get(ValueType::Int));
	this->_types.add("long", SimpleType::Get(ValueType::Long));
	this->_types.add("float", SimpleType::Get(ValueType::Float));
	this->_types.add("double", SimpleType::Get(ValueType::Double));
	this->_types.add("bool", SimpleType::Get(ValueType::Bool));
	this->_types.add("string", SimpleType::Get(ValueType::String));
}

void fay::FayDomain::addLib(PTR(FayLib) lib)
{
	//先生成相互的引用关系
	this->_libs.push_back(lib);

	//然后对Lib里的东西进行一下排序
	for each(auto it in lib->classes)
	{
		this->_types.add(it->fullname(), it);
	}
}

void fay::FayDomain::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayDomain]")->endl();
	sb->increaseIndent();
	for (auto i = 0; i < this->_types.list().size(); ++i)
	{
		auto t = this->_types.list()[i];
		sb->add(i)->add(" : ")->add(t->fullname())->endl();
	}

	for each(auto it in this->_libs)
		it->toString(sb);
	sb->decreaseIndent();
}

PTR(FayType) fay::FayDomain::findType(const std::string & typeFullname)
{
	return this->_types.find(typeFullname);
}

std::vector<PTR(FayType)> fay::FayDomain::findType(std::vector<std::string>& imports, const std::string & typeName)
{
	std::vector<PTR(FayType)> types;

	for each(auto it in imports)
	{
		std::string typeFullname = it + "." + typeName;
		auto type = this->findType(typeFullname);
		if (type)
			types.push_back(type);
	}

	return types;
}

PTR(FayFun) fay::FayDomain::findFun(const std::string & className, const std::string & funName, std::vector<PTR(FayType)> paramsType)
{
	return nullptr;
}

const std::string & fay::FayParamDef::fullname()
{
	if (this->_fullname.size() <= 0)
	{
		this->_fullname += this->_name;
		this->_fullname += ":";
		this->_fullname += this->_typeName;
	}

	return this->_fullname;
}

void fay::FayParamDef::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayParamDef] ")->add(this->_name)->endl();
}
