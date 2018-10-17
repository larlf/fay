#include <fay_lang.h>
#include <mirror_utils_log.h>

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
	if (type && type.get() == this)
		return true;

	return false;
}

std::vector<PTR(FayFun)> fay::FayType::findFun(const std::string &funName, const std::vector<PTR(FayType)> &paramsType)
{
	std::vector<PTR(FayFun)> funs;

	for each(auto it in this->_funs.list())
	{
		if (it->name() == funName && it->matchParams(paramsType))
			funs.push_back(it);
	}

	return funs;
}

PTR(FayFun) fay::FayType::findFun(pos_t index)
{
	return this->_funs.find(index);
}

pos_t fay::FayType::getFunIndex(const std::string & funname)
{
	return this->_funs.findIndex(funname);
}

pos_t fay::FayType::getFunIndex(const PTR(FayFun)& fun)
{
	return FayType::getFunIndex(fun->fullname());
}

pos_t fay::FayLib::addClass(PTR(FayClass) clazz)
{
	this->classes.push_back(clazz);

	auto domain = this->domain();
	if (domain)
		return domain->addType(clazz);

	return -1;
}

PTR(OutsideFun) fay::FayLib::findOutsideFun(const std::string &className, const std::string &funName, const std::vector<PTR(FayType)> &paramsType)
{
	//检查domain是否正常
	auto domain = this->domain();
	if (!domain)
	{
		LOG_ERROR("Cannot find domain");
		return nullptr;
	}

	auto clazz=domain->findType(className);
	if (!clazz)
	{
		LOG_ERROR("Cannot find type : " << className);
		return nullptr;
	}

	auto funs=clazz->findFun(funName, paramsType);
	if (funs.size() <= 0)
	{
		LOG_ERROR("Cannot find fun "<<funName<<" in class "<<className);
		return nullptr;
	}
	else if (funs.size() > 1)
	{
		LOG_ERROR("Too many fun " << funName << " in class " << className);
		return nullptr;
	}

	//添加外部函数
	PTR(OutsideFun) ofun = MKPTR(OutsideFun)(
		this->_outsideFuns.size(), 
		className, domain->getTypeIndex(clazz),
		funName, clazz->getFunIndex(funs[0]));
	this->_outsideFuns.add(ofun->fullname(), ofun);

	return ofun;
}

void fay::FayLib::toString(mirror::utils::StringBuilder *sb)
{
	sb->add("[FayLib]")->add(this->name)->endl();
	sb->increaseIndent();
	for each(auto it in this->classes)
		it->toString(sb);
	sb->decreaseIndent();
}

fay::FayFun::~FayFun()
{
	for each(auto it in this->insts)
		delete it;

	this->insts.clear();
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

bool fay::FayFun::matchParams(const std::vector<PTR(FayType)> &paramsType)
{
	//参数不一致
	if (paramsType.size() != this->_params.size())
		return false;

	for (auto i = 0; i < this->_params.size(); ++i)
	{
		PTR(FayParamDef) p = this->_params[i];
		if (p->type.expired() || !p->type.lock()->match(paramsType[i]))
			return false;
	}

	return true;
}

void fay::FayFun::toString(mirror::utils::StringBuilder *sb)
{
	sb->add("[FayFun]")->add(this->fullname())->endl();

	sb->increaseIndent();

	for each(auto it in this->_params)
		it->toString(sb);

	for (auto i = 0; i < this->insts.size(); ++i)
	{
		auto it = this->insts[i];
		sb->add(i)->add(" : ");
		it->toString(sb);
	}

	sb->decreaseIndent();
}

pos_t fay::FayClass::addFun(PTR(FayFun) fun)
{
	pos_t index=this->_funs.add(fun->fullname(), fun);
	return index;
}



void fay::FayClass::toString(mirror::utils::StringBuilder *sb)
{
	sb->add("[FayClass]")->add(this->_fullname)->endl();
	sb->increaseIndent();
	for each(auto it in this->_funs.list())
		it->toString(sb);
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

void fay::FayDomain::addLib(PTR(FayLib) lib)
{
	//先生成相互的引用关系
	this->_libs.push_back(lib);

	//然后对Lib里的东西进行一下排序
	for each(auto it in lib->classes)
		this->_types.add(it->fullname(), it);
}

void fay::FayDomain::toString(mirror::utils::StringBuilder *sb)
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
	pos_t index=this->_types.findIndex(fullname);
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
		LOG_ERROR("Cannot find type : "<<typeFullname);

	return type;
}

PTR(FayType) fay::FayDomain::findType(pos_t index)
{
	auto type= this->_types.find(index);
	if (!type)
		LOG_ERROR("Cannot find type by index : " << index);

	return type;
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

std::vector<PTR(FayFun)> fay::FayDomain::findFun(const std::string &className, const std::string &funName, const std::vector<PTR(FayType)> &paramsType)
{
	auto clazz=this->_types.find(className);
	if (clazz)
		return clazz->findFun(funName, paramsType);

	return std::vector<PTR(FayFun)>();
}

const std::string &fay::FayParamDef::fullname()
{
	if (this->_fullname.size() <= 0)
	{
		this->_fullname += this->name;
		this->_fullname += ":";
		this->_fullname += this->type.expired()?"?":this->type.lock()->fullname();
	}

	return this->_fullname;
}

void fay::FayParamDef::toString(mirror::utils::StringBuilder *sb)
{
	sb->add("[FayParamDef] ")->add(this->fullname())->endl();
}

void fay::FaySystemClass::init()
{
	PTR(FayFun) fun = MKPTR(FayFun)(this->domain(), "Print");
	fun->addParam(MKPTR(FayParamDef)("str", SimpleType::Get(ValueType::String)));
	this->addFun(fun);
}

void fay::FaySystemLib::init()
{
	PTR(FaySystemClass) c = MKPTR(FaySystemClass)(this->domain());
	this->addClass(c);
	c->init();
}

const std::string & fay::OutsideFun::fullname()
{
	if (this->_fullname.empty())
	{
		this->_fullname.append(this->_typeFullname);
		this->_fullname.append(".");
		this->_fullname.append(this->_funFullname);
	}

	return this->_fullname;
}
