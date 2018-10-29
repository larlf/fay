#include "fay_lang.h"
#include "fay_lang.h"
#include <fay_lang.h>
#include <mirror_utils_log.h>
#include <fay_internal_fun.h>

using namespace fay;

MAP<fay::ValueType, PTR(FayType)> fay::SimpleType::_Types;

PTR(FayType) fay::SimpleType::Get(ValueType valueType)
{
	auto it = _Types.find(valueType);
	if (it != _Types.end())
		return it->second;

	//没有这个类型，新建一个
	PTR(SimpleType) type(new SimpleType(valueType));
	_Types[valueType] = type;
	return type;
}

const std::string &fay::SimpleType::fullname()
{
	return TypeDict::ToName(this->_type);
}

bool fay::FayType::match(PTR(FayType) type)
{
	//TODO ：对接口的处理

	if (type && type.get() == this)
		return true;

	PTR(FayType) parent = this->_parent.lock();
	if (parent && parent->match(type))
		return true;

	return false;
}

std::vector<pos_t> fay::FayType::matchFun(const std::string &funName, const std::vector<PTR(FayType)> &paramsType, bool isStatic)
{
	if (isStatic)
		return this->_sft.matchFun(funName, paramsType);
	else
		return this->_vft.matchFun(funName, paramsType);
}

PTR(FayFun) fay::FayType::findFun(pos_t index, bool isStatic)
{
	if (isStatic)
		return this->_sft.getFun(index);
	else
		return this->_vft.getFun(index);
}

PTR(FayFun) fay::FayType::findFun(const std::string &fullname, bool isStatic)
{
	if (isStatic)
		return this->_sft.findFun(fullname);
	else
		return this->_vft.findFun(fullname);
}

pos_t fay::FayType::getFunIndex(const std::string &fullname, bool isStatic)
{
	if (isStatic)
		return this->_sft.findFunIndex(fullname);
	else
		return this->_vft.findFunIndex(fullname);
}

pos_t fay::FayLib::addClass(PTR(FayClass) clazz)
{
	clazz->lib(this->shared_from_this());
	this->classes.push_back(clazz);

	auto domain = this->domain();
	if (domain)
		return domain->addType(clazz);

	return -1;
}

pos_t fay::FayLib::findOutsideFun(const std::string &className, const std::string &funName, const std::vector<PTR(FayType)> &paramsType)
{
	//查找当前是否已经有了
	std::string fullname = FayLangUtils::Fullname(className, funName, paramsType);
	pos_t index=this->_outsideFuns.findIndex(fullname);
	if (index>=0) return index;

	//检查domain是否正常
	auto domain = this->domain();
	if (!domain)
	{
		LOG_ERROR("Cannot find domain");
		return -1;
	}

	auto clazz=domain->findType(className);
	if (!clazz)
	{
		LOG_ERROR("Cannot find type : " << className);
		return -1;
	}

	auto funs=clazz->matchFun(funName, paramsType, true);
	if (funs.size() <= 0)
	{
		LOG_ERROR("Cannot find fun : "<<fullname);
		return -1;
	}
	else if (funs.size() > 1)
	{
		LOG_ERROR("Too many fun : " << fullname);
		return -1;
	}

	//添加外部函数
	PTR(OutsideFun) ofun = MKPTR(OutsideFun)(
			className, domain->getTypeIndex(clazz),
			funName, funs[0]);
	return this->_outsideFuns.add(fullname, ofun);
}

void fay::FayLib::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayLib]")->add(this->name)->endl();
	sb->increaseIndent();
	//显示外部函数
	sb->add("[OutSideFun]")->endl();
	sb->increaseIndent();
	for (auto i = 0; i < this->_outsideFuns.list().size(); ++i)
	{
		sb->add(i)->add(" : ");
		this->_outsideFuns.list()[i]->toString(sb);
	}
	sb->decreaseIndent();
	//显示Class的内容
	sb->add("[Classes]")->endl();
	sb->increaseIndent();
	for each(auto it in this->classes)
		it->toString(sb);
	sb->decreaseIndent();
	sb->decreaseIndent();
}

void fay::FayInstFun::prepareInsts()
{
	for (auto i = 0; i < this->_insts.size(); ++i)
	{
		FayInst* inst = this->_insts[i];
		switch (inst->type())
		{
			case InstType::CallStatic:
			{
				//取出调用方法的索引值
				inst::CallStatic* i = static_cast<inst::CallStatic*>(inst);
				PTR(OutsideFun) fun = this->clazz()->lib()->findOutsideFun(i->outsideFunIndex);
				i->typeIndex = fun->typeIndex();
				i->funIndex = fun->funIndex();
				break;
			}
		}
	}
}

fay::FayInstFun::~FayInstFun()
{
	for each(auto it in this->_insts)
		delete it;

	this->_insts.clear();
}

std::vector<FayInst*>* fay::FayInstFun::getPreparedInsts()
{
	if (!this->isPrepared)
	{
		this->prepareInsts();
		this->isPrepared = true;
	}

	return &this->_insts;
}

pos_t fay::FayInstFun::addVar(const std::string &name, PTR(FayType) type)
{
	auto it = this->_vars.find(name);
	if (it)
	{
		if (it->type() != type)
		{
			LOG_ERROR("Same var name, diff type : "<<it->type()<<", "<<type);
			return -1;
		}

		return this->_vars.findIndex(name);
	}

	//创建并加入变量定义
	PTR(FayVarDef) def = MKPTR(FayVarDef)(this->domain(), name, type);
	return this->_vars.add(name, def);
}

PTR(FayVarDef) fay::FayInstFun::findVar(const std::string & name)
{
	return this->_vars.find(name);
}

pos_t fay::FayInstFun::getVarIndex(const std::string & name)
{
	return this->_vars.findIndex(name);
}

void fay::FayInstFun::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayInstFun]")->add(this->fullname())->endl();

	sb->increaseIndent();

	for each(auto it in this->_params)
		it->toString(sb);

	for (auto i = 0; i < this->_insts.size(); ++i)
	{
		auto it = this->_insts[i];
		sb->add(i)->add(" : ");
		it->toString(sb);
	}

	sb->decreaseIndent();
}

const std::string &fay::FayFun::fullname()
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

		this->_fullname += this->_name;
		this->_fullname += "(" + str + ")";
	}

	return this->_fullname;
}

void fay::FayFun::addParam(PTR(FayParamDef) def)
{
	this->_params.push_back(def);
}

void fay::FayFun::addReturn(PTR(FayType) type)
{
	this->_returns.push_back(type);
}

bool fay::FayFun::match(const std::vector<PTR(FayType)> &paramsType)
{
	//参数不一致
	if (paramsType.size() != this->_params.size())
		return false;

	for (auto i = 0; i < this->_params.size(); ++i)
	{
		PTR(FayParamDef) p = this->_params[i];
		if (!p->type()->match(paramsType[i]))
			return false;
	}

	return true;
}

void fay::FayFun::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayFun]")->add(this->fullname())->endl();

	sb->increaseIndent();

	for each(auto it in this->_params)
		it->toString(sb);

	sb->decreaseIndent();
}

pos_t fay::FayType::addFun(PTR(FayFun) fun)
{
	fun->clazz(this->shared_from_this());
	if (fun->isStatic())
		return this->_sft.addFun(fun);
	else
		return this->_vft.addFun(fun);
}

void fay::FayClass::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayClass]")->add(this->_fullname)->endl();
	sb->increaseIndent();

	sb->add("SFT : ")->endl();
	sb->increaseIndent();
	this->_sft.toString(sb);
	sb->decreaseIndent();

	sb->add("VFT : ")->endl();
	sb->increaseIndent();
	this->_vft.toString(sb);
	sb->decreaseIndent();

	sb->decreaseIndent();
}

const std::string &fay::FayClass::fullname()
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

void fay::FayDomain::initSysLib()
{
	PTR(FayLib) lib(new FayLib(MYPTR, "System"));
	PTR(FayClass) clazz(new FayClass(MYPTR, "fay", "System"));
	clazz->addFun(MKPTR(FayInternalFun)(MYPTR, "Print", InternalFun::Print_String, std::vector<std::string>({ "string" })));
	clazz->addFun(MKPTR(FayInternalFun)(MYPTR, "Print", InternalFun::Print_Int, std::vector<std::string>({ "int" })));

	lib->addClass(clazz);
	this->addLib(lib);
}

void fay::FayDomain::addLib(PTR(FayLib) lib)
{
	//先生成相互的引用关系
	this->_libs.push_back(lib);

	//然后对Lib里的东西进行一下排序
	for each(auto it in lib->classes)
		this->_types.add(it->fullname(), it);
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

pos_t fay::FayDomain::addType(PTR(FayType) t)
{
	std::string fullname = t->fullname();

	//如果已经有了，就返回现有的位置
	pos_t index = this->_types.findIndex(fullname);
	if (index >= 0)
		return index;

	return this->_types.add(fullname, t);
}

pos_t fay::FayDomain::getTypeIndex(PTR(FayType) t)
{
	return this->_types.findIndex(t->fullname());
}

PTR(FayType) fay::FayDomain::findType(const std::string &typeFullname)
{
	auto type = this->_types.find(typeFullname);
	if (!type)
		LOG_ERROR("Cannot find type : " << typeFullname);

	return type;
}

PTR(FayType) fay::FayDomain::findType(pos_t index)
{
	auto type = this->_types.find(index);
	if (!type)
		LOG_ERROR("Cannot find type by index : " << index);

	return type;
}

PTR(FayType) fay::FayDomain::findType(ValueType type)
{
	return SimpleType::Get(type);
}

std::vector<PTR(FayType)> fay::FayDomain::findType(std::vector<std::string> &imports, const std::string &typeName)
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

PTR(FayFun) fay::FayDomain::findFun(const std::string &typeFullname, const std::string &funFullname, bool isStatic)
{
	auto type = this->findType(typeFullname);
	if (type)
		return type->findFun(funFullname, isStatic);

	return nullptr;
}

PTR(FayFun) fay::FayDomain::findFun(pos_t typeIndex, pos_t funIndex, bool isStatic)
{
	auto type = this->_types[typeIndex];
	if (!type)
	{
		LOG_ERROR("Cannot find type : " << typeIndex);
		return nullptr;
	}

	auto fun = type->findFun(funIndex, isStatic);
	if (!fun)
	{
		LOG_ERROR("Cannot find fun : " << funIndex << " in " << type->fullname());
		return nullptr;
	}

	return fun;
}

bool fay::FayDomain::findFunIndex(const std::string &typeFullname, const std::string &funFullname, bool isStatic, pos_t &typeIndex, pos_t &funIndex)
{
	typeIndex = -1;
	funIndex = -1;

	typeIndex = this->_types.findIndex(typeFullname);
	if (typeIndex<0)
	{
		LOG_ERROR("Cannot find type : " << typeIndex);
		return false;
	}

	funIndex = this->_types[typeIndex]->getFunIndex(funFullname, funIndex);
	if (funIndex < 0)
	{
		LOG_ERROR("Cannot find fun : " << funIndex << " in " << this->_types[typeIndex]->fullname());
		return false;
	}

	return true;
}

bool fay::FayDomain::findFunIndex(PTR(FayFun) fun, pos_t &typeIndex, pos_t &funIndex)
{
	typeIndex = -1;
	funIndex = -1;

	if (!fun)
	{
		LOG_ERROR("Fun is null");
		return false;
	}

	typeIndex = this->_types.findIndex(fun->clazz()->fullname());
	if (typeIndex < 0)
	{
		LOG_ERROR("Cannot find type : " << fun->clazz()->fullname());
		return false;
	}

	funIndex = this->_types[typeIndex]->getFunIndex(fun->fullname(), fun->isStatic());
	if (funIndex < 0)
	{
		LOG_ERROR("Cannot find fun : " << fun->fullname());
		return false;
	}

	return true;
}

const std::string &fay::FayParamDef::fullname()
{
	if (this->_fullname.size() <= 0)
	{
		this->_fullname += this->_name;
		this->_fullname += ":";
		this->_fullname += this->_type.expired()?"?":this->_type.lock()->fullname();
	}

	return this->_fullname;
}

void fay::FayParamDef::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayParamDef] ")->add(this->fullname())->endl();
}

std::string fay::FayLangUtils::Fullname(const std::string &funName, const std::vector<PTR(FayType)> &params)
{
	std::string str;
	for each(auto it in params)
	{
		if (str.size() > 0)
			str.append(",");

		if (it)
			str.append(it->fullname());
		else
			str.append("?");
	}

	str = funName + "(" + str + ")";
	return str;
}

std::string fay::FayLangUtils::Fullname(const std::string &className, const std::string &funName, const std::vector<PTR(FayType)> &params)
{
	return className + "." + FayLangUtils::Fullname(funName, params);
}

void fay::OutsideFun::toString(mirror::utils::StringBuilder* sb)
{
	sb->add(this->_typeFullname)->add(":")->add(this->_typeIndex)->add(" ");
	sb->add(this->_funFullname)->add(":")->add(this->_funIndex)->endl();
}

fay::FayInternalFun::FayInternalFun(PTR(FayDomain) domain, const std::string &name, std::function<void(VMStack*)> fun, std::vector<std::string> params)
	: FayFun(domain, name, FunType::Internal, true), _fun(fun)
{
	for(auto i=0; i<params.size(); ++i)
	{
		auto it = params[i];
		std::string paramName = "p" + std::to_string(i);
		auto t=domain->findType(it);
		PTR(FayParamDef) p = MKPTR(FayParamDef)(domain, paramName, t);
		this->addParam(p);
	}
}

pos_t fay::FunTable::addFun(PTR(FayFun) fun)
{
	for (auto i = 0; i < this->_funs.size(); ++i)
	{
		//如果是同一个函数，用新的替换旧的
		if (this->_funs[i]->fullname() == fun->fullname())
		{
			this->_funs[i] = fun;
			return i;
		}
	}

	//如果当前没有这个函数，就加到最后
	this->_funs.push_back(fun);
	return this->_funs.size() - 1;
}

PTR(FayFun) fay::FunTable::getFun(pos_t index)
{
	if (index >= 0 && index < this->_funs.size())
		return this->_funs[index];

	return nullptr;
}

void fay::FunTable::rebuild(std::vector<PTR(FayFun)> &parentFuns)
{
	std::vector<PTR(FayFun)> funs = this->_funs;
	this->_funs = parentFuns;

	for (auto i = 0; i < funs.size(); ++i)
		this->addFun(funs[i]);
}

std::vector<pos_t> fay::FunTable::matchFun(const std::string &funName, const std::vector<PTR(FayType)> &paramsType)
{
	std::vector<pos_t> funs;

	for(auto i = 0; i < this->_funs.size(); ++i)
	{
		auto it = this->_funs[i];
		if (it->name() == funName && it->match(paramsType))
			funs.push_back(i);
	}

	return funs;
}

pos_t fay::FunTable::findFunIndex(const std::string &fullname)
{
	for (auto i = 0; i < this->_funs.size(); ++i)
	{
		if (this->_funs[i]->fullname() == fullname)
			return i;
	}

	return -1;
}

PTR(FayFun) fay::FunTable::findFun(const std::string &fullname)
{
	for (auto i = 0; i < this->_funs.size(); ++i)
	{
		if (this->_funs[i]->fullname() == fullname)
			return this->_funs[i];
	}

	return nullptr;
}

void fay::FunTable::toString(mirror::utils::StringBuilder* sb)
{
	for each(auto it in this->_funs)
	{
		it->toString(sb);
		sb->endl();
	}
}

const std::string &fay::FayVarDef::fullname()
{
	if (!this->_fullname.size())
		this->_fullname = this->_name + ":"+this->_type.lock()->fullname();

	return this->_fullname;
}

void fay::FayVarDef::toString(mirror::utils::StringBuilder* sb)
{
	sb->add(this->fullname());
}
