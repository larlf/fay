#include "fay_lang.h"
#include "fay_lang.h"
#include <fay_lang.h>
#include <mirror_utils_log.h>
#include <fay_internal_fun.h>

using namespace fay;

MAP<fay::ValueType, PTR(FayClass)> fay::FaySimpleClass::_Types;

PTR(FayClass) fay::FaySimpleClass::Get(ValueType classType)
{
	auto it = _Types.find(classType);
	if(it != _Types.end())
		return it->second;

	//没有这个类型，新建一个
	PTR(FaySimpleClass) type(new FaySimpleClass(classType));
	_Types[classType] = type;
	return type;
}

const std::string &fay::FaySimpleClass::fullname()
{
	return TypeDict::ToName(this->_valueType);
}

bool fay::FayClass::match(PTR(FayClass) type)
{
	//TODO ：对接口的处理

	if(type && type.get() == this)
		return true;

	PTR(FayClass) parent = this->_parent.lock();
	if(parent && parent->match(type))
		return true;

	return false;
}

std::vector<pos_t> fay::FayClass::matchFun(const std::string &funName, const std::vector<PTR(FayClass)> &paramsType, bool isStatic)
{
	if(isStatic)
		return this->_sft.matchFun(funName, paramsType);
	else
		return this->_vft.matchFun(funName, paramsType);
}

PTR(FayFun) fay::FayClass::findFun(pos_t index, bool isStatic)
{
	if(isStatic)
		return this->_sft.getFun(index);
	else
		return this->_vft.getFun(index);
}

PTR(FayFun) fay::FayClass::findFun(const std::string &fullname, bool isStatic)
{
	if(isStatic)
		return this->_sft.findFun(fullname);
	else
		return this->_vft.findFun(fullname);
}

pos_t fay::FayClass::getFunIndex(const std::string &fullname, bool isStatic)
{
	if(isStatic)
		return this->_sft.findFunIndex(fullname);
	else
		return this->_vft.findFunIndex(fullname);
}

pos_t fay::FayLib::addClass(PTR(FayInstClass) clazz)
{
	clazz->lib(this->shared_from_this());
	this->classes.push_back(clazz);

	auto domain = this->domain();
	if(domain)
		return domain->addType(clazz);

	return -1;
}

pos_t fay::FayLib::findOutsideFun(const std::string &className, const std::string &funName, const std::vector<PTR(FayClass)> &paramsType)
{
	//查找当前是否已经有了
	std::string fullname = FayLangUtils::Fullname(className, funName, paramsType);
	pos_t index = this->_outsideFuns.findIndex(fullname);
	if(index >= 0) return index;

	//检查domain是否正常
	auto domain = this->domain();
	if(!domain)
	{
		LOG_ERROR("Cannot find domain");
		return -1;
	}

	auto clazz = domain->findType(className);
	if(!clazz)
	{
		LOG_ERROR("Cannot find type : " << className);
		return -1;
	}

	auto funs = clazz->matchFun(funName, paramsType, true);
	if(funs.size() <= 0)
	{
		LOG_ERROR("Cannot find fun : " << fullname);
		return -1;
	}
	else if(funs.size() > 1)
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
	for(auto i = 0; i < this->_outsideFuns.list().size(); ++i)
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
	for(auto i = 0; i < this->_insts.size(); ++i)
	{
		FayInst* inst = this->_insts[i];
		switch(inst->type())
		{
			case InstType::CallStatic:
			{
				//取出调用方法的索引值
				inst::CallStatic* i = static_cast<inst::CallStatic*>(inst);
				PTR(OutsideFun) fun = this->clazz()->lib()->findOutsideFun(i->outsideFunIndex);
				if(fun)
				{
					i->typeIndex = fun->typeIndex();
					i->funIndex = fun->funIndex();
				}
				else
				{
					i->typeIndex = -1;
					i->funIndex = -1;
				}
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
	if(!this->isPrepared)
	{
		this->prepareInsts();
		this->isPrepared = true;
	}

	return &this->_insts;
}

pos_t fay::FayInstFun::addVar(const std::string &name, PTR(FayClass) type)
{
	auto it = this->_vars.find(name);
	if(it)
	{
		if(it->type() != type)
		{
			LOG_ERROR("Same var name, diff type : " << it->type() << ", " << type);
			return -1;
		}

		return this->_vars.findIndex(name);
	}

	//创建并加入变量定义
	PTR(FayVarDef) def = MKPTR(FayVarDef)(this->domain(), name, type);
	return this->_vars.add(name, def);
}

PTR(FayVarDef) fay::FayInstFun::findVar(const std::string &name)
{
	return this->_vars.find(name);
}

pos_t fay::FayInstFun::getVarIndex(const std::string &name)
{
	return this->_vars.findIndex(name);
}

void fay::FayInstFun::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayInstFun]")->add(this->fullname())->endl();

	sb->increaseIndent();

	for each(auto it in this->_params)
		it->toString(sb);

	for(auto i = 0; i < this->_insts.size(); ++i)
	{
		auto it = this->_insts[i];
		sb->add(i)->add(" : ");
		it->toString(sb);
	}

	sb->decreaseIndent();
}

const std::string &fay::FayFun::fullname()
{
	if(this->_fullname.size() <= 0)
	{
		std::string str;
		for each(auto it in this->_params)
		{
			if(str.size() > 0)
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

void fay::FayFun::addReturn(PTR(FayClass) type)
{
	this->_returns.push_back(type);
}

bool fay::FayFun::match(const std::vector<PTR(FayClass)> &paramsType)
{
	//参数不一致
	if(paramsType.size() != this->_params.size())
		return false;

	for(auto i = 0; i < this->_params.size(); ++i)
	{
		PTR(FayParamDef) p = this->_params[i];
		if(!p->type()->match(paramsType[i]))
			return false;
	}

	return true;
}

void fay::FayFun::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayFun]")->add(this->fullname())->endl();

	//sb->increaseIndent();

	//for each(auto it in this->_params)
	//	it->toString(sb);

	//sb->decreaseIndent();
}

pos_t fay::FayClass::addFun(PTR(FayFun) fun)
{
	fun->clazz(this->shared_from_this());
	if(fun->isStatic())
		return this->_sft.addFun(fun);
	else
		return this->_vft.addFun(fun);
}

void fay::FayInstClass::toString(mirror::utils::StringBuilder* sb)
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

const std::string &fay::FayInstClass::fullname()
{
	return this->_fullname;
}

fay::FayDomain::FayDomain()
{
}

void fay::FayDomain::initSysLib()
{
	//内置类型在这里创建
	this->_types.add("bool", FaySimpleClass::Get(ValueType::Bool));
	this->_types.add("byte", FaySimpleClass::Get(ValueType::Byte));
	this->_types.add("int", FaySimpleClass::Get(ValueType::Int));
	this->_types.add("long", FaySimpleClass::Get(ValueType::Long));
	this->_types.add("float", FaySimpleClass::Get(ValueType::Float));
	this->_types.add("double", FaySimpleClass::Get(ValueType::Double));
	this->_types.add("string", FaySimpleClass::Get(ValueType::String));

	PTR(FayLib) lib(new FayLib(MYPTR, "System"));
	PTR(FayInstClass) clazz(new FayInstClass(MYPTR, "fay", "System"));
	clazz->addFun(MKPTR(FayInternalFun)(MYPTR, "Print", InternalFun::Print_Bool, std::vector<std::string>({ "bool" })));
	clazz->addFun(MKPTR(FayInternalFun)(MYPTR, "Print", InternalFun::Print_Byte, std::vector<std::string>({ "byte" })));
	clazz->addFun(MKPTR(FayInternalFun)(MYPTR, "Print", InternalFun::Print_Int, std::vector<std::string>({ "int" })));
	clazz->addFun(MKPTR(FayInternalFun)(MYPTR, "Print", InternalFun::Print_Long, std::vector<std::string>({ "long" })));
	clazz->addFun(MKPTR(FayInternalFun)(MYPTR, "Print", InternalFun::Print_Float, std::vector<std::string>({ "float" })));
	clazz->addFun(MKPTR(FayInternalFun)(MYPTR, "Print", InternalFun::Print_Double, std::vector<std::string>({ "double" })));
	clazz->addFun(MKPTR(FayInternalFun)(MYPTR, "Print", InternalFun::Print_String, std::vector<std::string>({ "string" })));

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
	for(auto i = 0; i < this->_types.list().size(); ++i)
	{
		auto t = this->_types.list()[i];
		sb->add(i)->add(" : ")->add(t->fullname())->endl();
	}

	for each(auto it in this->_libs)
		it->toString(sb);
	sb->decreaseIndent();
}

pos_t fay::FayDomain::addType(PTR(FayClass) t)
{
	std::string fullname = t->fullname();

	//如果已经有了，就返回现有的位置
	pos_t index = this->_types.findIndex(fullname);
	if(index >= 0)
		return index;

	return this->_types.add(fullname, t);
}

pos_t fay::FayDomain::getTypeIndex(PTR(FayClass) t)
{
	return this->_types.findIndex(t->fullname());
}

PTR(FayClass) fay::FayDomain::findType(const std::string &typeFullname)
{
	auto type = this->_types.find(typeFullname);
	if(!type)
		LOG_ERROR("Cannot find type : " << typeFullname);

	return type;
}

PTR(FayClass) fay::FayDomain::findType(pos_t index)
{
	auto type = this->_types.find(index);
	if(!type)
		LOG_ERROR("Cannot find type by index : " << index);

	return type;
}

PTR(FayClass) fay::FayDomain::findType(ValueType type)
{
	return FaySimpleClass::Get(type);
}

std::vector<PTR(FayClass)> fay::FayDomain::findType(std::vector<std::string> &imports, const std::string &typeName)
{
	std::vector<PTR(FayClass)> types;

	for each(auto it in imports)
	{
		std::string typeFullname = it + "." + typeName;
		auto type = this->findType(typeFullname);
		if(type)
			types.push_back(type);
	}

	return types;
}

PTR(FayFun) fay::FayDomain::findFun(const std::string &typeFullname, const std::string &funFullname, bool isStatic)
{
	auto type = this->findType(typeFullname);
	if(type)
		return type->findFun(funFullname, isStatic);

	return nullptr;
}

PTR(FayFun) fay::FayDomain::findFun(pos_t typeIndex, pos_t funIndex, bool isStatic)
{
	if(typeIndex < 0 || funIndex < 0)
	{
		LOG_ERROR("Bad fun index : " << typeIndex << " " << funIndex);
		return nullptr;
	}

	auto type = this->_types[typeIndex];
	if(!type)
	{
		LOG_ERROR("Cannot find type : " << typeIndex);
		return nullptr;
	}

	auto fun = type->findFun(funIndex, isStatic);
	if(!fun)
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
	if(typeIndex < 0)
	{
		LOG_ERROR("Cannot find type : " << typeIndex);
		return false;
	}

	funIndex = this->_types[typeIndex]->getFunIndex(funFullname, funIndex);
	if(funIndex < 0)
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

	if(!fun)
	{
		LOG_ERROR("Fun is null");
		return false;
	}

	typeIndex = this->_types.findIndex(fun->clazz()->fullname());
	if(typeIndex < 0)
	{
		LOG_ERROR("Cannot find type : " << fun->clazz()->fullname());
		return false;
	}

	funIndex = this->_types[typeIndex]->getFunIndex(fun->fullname(), fun->isStatic());
	if(funIndex < 0)
	{
		LOG_ERROR("Cannot find fun : " << fun->fullname());
		return false;
	}

	return true;
}

const std::string &fay::FayParamDef::fullname()
{
	if(this->_fullname.size() <= 0)
	{
		this->_fullname += this->_name;
		this->_fullname += ":";
		this->_fullname += this->_class.expired() ? "?" : this->_class.lock()->fullname();
	}

	return this->_fullname;
}

void fay::FayParamDef::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayParamDef] ")->add(this->fullname())->endl();
}

std::string fay::FayLangUtils::Fullname(const std::string &funName, const std::vector<PTR(FayClass)> &params)
{
	std::string str;
	for each(auto it in params)
	{
		if(str.size() > 0)
			str.append(",");

		if(it)
			str.append(it->fullname());
		else
			str.append("?");
	}

	str = funName + "(" + str + ")";
	return str;
}

fay::ValueType fay::FayLangUtils::WeightValueType(ValueType t1, ValueType t2)
{
	//有字符串的话，都转换成字符串
	if(t1 >= ValueType::String || t2 >= ValueType::String)
		return ValueType::String;

	return (t1 >= t2) ? t1 : t2;
}

ValueType fay::FayLangUtils::ClassToValueType(PTR(FayClass) clazz)
{
	if(clazz->type() == ClassType::Simple)
		return TOPTR(FaySimpleClass, clazz)->valueType();

	return ValueType::Object;
}

fay::FayInst* fay::FayLangUtils::ConvertInst(ValueType src, ValueType dest)
{
	//ConvertInstStart
	if (src == ValueType::Void && dest == ValueType::Void)
		return new inst::VoidToVoid();
	else if (src == ValueType::Void && dest == ValueType::Bool)
		return new inst::VoidToBool();
	else if (src == ValueType::Void && dest == ValueType::Byte)
		return new inst::VoidToByte();
	else if (src == ValueType::Void && dest == ValueType::Int)
		return new inst::VoidToInt();
	else if (src == ValueType::Void && dest == ValueType::Long)
		return new inst::VoidToLong();
	else if (src == ValueType::Void && dest == ValueType::Float)
		return new inst::VoidToFloat();
	else if (src == ValueType::Void && dest == ValueType::Double)
		return new inst::VoidToDouble();
	else if (src == ValueType::Void && dest == ValueType::String)
		return new inst::VoidToString();
	else if (src == ValueType::Void && dest == ValueType::Object)
		return new inst::VoidToObject();
	else if (src == ValueType::Void && dest == ValueType::Function)
		return new inst::VoidToFunction();
	else if (src == ValueType::Bool && dest == ValueType::Void)
		return new inst::BoolToVoid();
	else if (src == ValueType::Bool && dest == ValueType::Bool)
		return new inst::BoolToBool();
	else if (src == ValueType::Bool && dest == ValueType::Byte)
		return new inst::BoolToByte();
	else if (src == ValueType::Bool && dest == ValueType::Int)
		return new inst::BoolToInt();
	else if (src == ValueType::Bool && dest == ValueType::Long)
		return new inst::BoolToLong();
	else if (src == ValueType::Bool && dest == ValueType::Float)
		return new inst::BoolToFloat();
	else if (src == ValueType::Bool && dest == ValueType::Double)
		return new inst::BoolToDouble();
	else if (src == ValueType::Bool && dest == ValueType::String)
		return new inst::BoolToString();
	else if (src == ValueType::Bool && dest == ValueType::Object)
		return new inst::BoolToObject();
	else if (src == ValueType::Bool && dest == ValueType::Function)
		return new inst::BoolToFunction();
	else if (src == ValueType::Byte && dest == ValueType::Void)
		return new inst::ByteToVoid();
	else if (src == ValueType::Byte && dest == ValueType::Bool)
		return new inst::ByteToBool();
	else if (src == ValueType::Byte && dest == ValueType::Byte)
		return new inst::ByteToByte();
	else if (src == ValueType::Byte && dest == ValueType::Int)
		return new inst::ByteToInt();
	else if (src == ValueType::Byte && dest == ValueType::Long)
		return new inst::ByteToLong();
	else if (src == ValueType::Byte && dest == ValueType::Float)
		return new inst::ByteToFloat();
	else if (src == ValueType::Byte && dest == ValueType::Double)
		return new inst::ByteToDouble();
	else if (src == ValueType::Byte && dest == ValueType::String)
		return new inst::ByteToString();
	else if (src == ValueType::Byte && dest == ValueType::Object)
		return new inst::ByteToObject();
	else if (src == ValueType::Byte && dest == ValueType::Function)
		return new inst::ByteToFunction();
	else if (src == ValueType::Int && dest == ValueType::Void)
		return new inst::IntToVoid();
	else if (src == ValueType::Int && dest == ValueType::Bool)
		return new inst::IntToBool();
	else if (src == ValueType::Int && dest == ValueType::Byte)
		return new inst::IntToByte();
	else if (src == ValueType::Int && dest == ValueType::Int)
		return new inst::IntToInt();
	else if (src == ValueType::Int && dest == ValueType::Long)
		return new inst::IntToLong();
	else if (src == ValueType::Int && dest == ValueType::Float)
		return new inst::IntToFloat();
	else if (src == ValueType::Int && dest == ValueType::Double)
		return new inst::IntToDouble();
	else if (src == ValueType::Int && dest == ValueType::String)
		return new inst::IntToString();
	else if (src == ValueType::Int && dest == ValueType::Object)
		return new inst::IntToObject();
	else if (src == ValueType::Int && dest == ValueType::Function)
		return new inst::IntToFunction();
	else if (src == ValueType::Long && dest == ValueType::Void)
		return new inst::LongToVoid();
	else if (src == ValueType::Long && dest == ValueType::Bool)
		return new inst::LongToBool();
	else if (src == ValueType::Long && dest == ValueType::Byte)
		return new inst::LongToByte();
	else if (src == ValueType::Long && dest == ValueType::Int)
		return new inst::LongToInt();
	else if (src == ValueType::Long && dest == ValueType::Long)
		return new inst::LongToLong();
	else if (src == ValueType::Long && dest == ValueType::Float)
		return new inst::LongToFloat();
	else if (src == ValueType::Long && dest == ValueType::Double)
		return new inst::LongToDouble();
	else if (src == ValueType::Long && dest == ValueType::String)
		return new inst::LongToString();
	else if (src == ValueType::Long && dest == ValueType::Object)
		return new inst::LongToObject();
	else if (src == ValueType::Long && dest == ValueType::Function)
		return new inst::LongToFunction();
	else if (src == ValueType::Float && dest == ValueType::Void)
		return new inst::FloatToVoid();
	else if (src == ValueType::Float && dest == ValueType::Bool)
		return new inst::FloatToBool();
	else if (src == ValueType::Float && dest == ValueType::Byte)
		return new inst::FloatToByte();
	else if (src == ValueType::Float && dest == ValueType::Int)
		return new inst::FloatToInt();
	else if (src == ValueType::Float && dest == ValueType::Long)
		return new inst::FloatToLong();
	else if (src == ValueType::Float && dest == ValueType::Float)
		return new inst::FloatToFloat();
	else if (src == ValueType::Float && dest == ValueType::Double)
		return new inst::FloatToDouble();
	else if (src == ValueType::Float && dest == ValueType::String)
		return new inst::FloatToString();
	else if (src == ValueType::Float && dest == ValueType::Object)
		return new inst::FloatToObject();
	else if (src == ValueType::Float && dest == ValueType::Function)
		return new inst::FloatToFunction();
	else if (src == ValueType::Double && dest == ValueType::Void)
		return new inst::DoubleToVoid();
	else if (src == ValueType::Double && dest == ValueType::Bool)
		return new inst::DoubleToBool();
	else if (src == ValueType::Double && dest == ValueType::Byte)
		return new inst::DoubleToByte();
	else if (src == ValueType::Double && dest == ValueType::Int)
		return new inst::DoubleToInt();
	else if (src == ValueType::Double && dest == ValueType::Long)
		return new inst::DoubleToLong();
	else if (src == ValueType::Double && dest == ValueType::Float)
		return new inst::DoubleToFloat();
	else if (src == ValueType::Double && dest == ValueType::Double)
		return new inst::DoubleToDouble();
	else if (src == ValueType::Double && dest == ValueType::String)
		return new inst::DoubleToString();
	else if (src == ValueType::Double && dest == ValueType::Object)
		return new inst::DoubleToObject();
	else if (src == ValueType::Double && dest == ValueType::Function)
		return new inst::DoubleToFunction();
	else if (src == ValueType::String && dest == ValueType::Void)
		return new inst::StringToVoid();
	else if (src == ValueType::String && dest == ValueType::Bool)
		return new inst::StringToBool();
	else if (src == ValueType::String && dest == ValueType::Byte)
		return new inst::StringToByte();
	else if (src == ValueType::String && dest == ValueType::Int)
		return new inst::StringToInt();
	else if (src == ValueType::String && dest == ValueType::Long)
		return new inst::StringToLong();
	else if (src == ValueType::String && dest == ValueType::Float)
		return new inst::StringToFloat();
	else if (src == ValueType::String && dest == ValueType::Double)
		return new inst::StringToDouble();
	else if (src == ValueType::String && dest == ValueType::String)
		return new inst::StringToString();
	else if (src == ValueType::String && dest == ValueType::Object)
		return new inst::StringToObject();
	else if (src == ValueType::String && dest == ValueType::Function)
		return new inst::StringToFunction();
	else if (src == ValueType::Object && dest == ValueType::Void)
		return new inst::ObjectToVoid();
	else if (src == ValueType::Object && dest == ValueType::Bool)
		return new inst::ObjectToBool();
	else if (src == ValueType::Object && dest == ValueType::Byte)
		return new inst::ObjectToByte();
	else if (src == ValueType::Object && dest == ValueType::Int)
		return new inst::ObjectToInt();
	else if (src == ValueType::Object && dest == ValueType::Long)
		return new inst::ObjectToLong();
	else if (src == ValueType::Object && dest == ValueType::Float)
		return new inst::ObjectToFloat();
	else if (src == ValueType::Object && dest == ValueType::Double)
		return new inst::ObjectToDouble();
	else if (src == ValueType::Object && dest == ValueType::String)
		return new inst::ObjectToString();
	else if (src == ValueType::Object && dest == ValueType::Object)
		return new inst::ObjectToObject();
	else if (src == ValueType::Object && dest == ValueType::Function)
		return new inst::ObjectToFunction();
	else if (src == ValueType::Function && dest == ValueType::Void)
		return new inst::FunctionToVoid();
	else if (src == ValueType::Function && dest == ValueType::Bool)
		return new inst::FunctionToBool();
	else if (src == ValueType::Function && dest == ValueType::Byte)
		return new inst::FunctionToByte();
	else if (src == ValueType::Function && dest == ValueType::Int)
		return new inst::FunctionToInt();
	else if (src == ValueType::Function && dest == ValueType::Long)
		return new inst::FunctionToLong();
	else if (src == ValueType::Function && dest == ValueType::Float)
		return new inst::FunctionToFloat();
	else if (src == ValueType::Function && dest == ValueType::Double)
		return new inst::FunctionToDouble();
	else if (src == ValueType::Function && dest == ValueType::String)
		return new inst::FunctionToString();
	else if (src == ValueType::Function && dest == ValueType::Object)
		return new inst::FunctionToObject();
	else if (src == ValueType::Function && dest == ValueType::Function)
		return new inst::FunctionToFunction();
	//ConvertInstEnd

	return nullptr;
}

FayInst* fay::FayLangUtils::PushNumber(ValueType type, int32_t value)
{
	switch(type)
	{
		case fay::ValueType::Void:
			break;
		case fay::ValueType::Bool:
			return new inst::PushBool((bool)value);
			break;
		case fay::ValueType::Byte:
			return new inst::PushByte((byte)value);
			break;
		case fay::ValueType::Int:
			return new inst::PushInt((int32_t)value);
			break;
		case fay::ValueType::Long:
			return new inst::PushLong((int64_t)value);
			break;
		case fay::ValueType::Float:
			return new inst::PushFloat((float)value);
			break;
		case fay::ValueType::Double:
			return new inst::PushDouble((double)value);
			break;
		case fay::ValueType::String:
			return new inst::PushString(std::to_string(value));
			break;
		case fay::ValueType::Object:
			break;
		case fay::ValueType::Function:
			break;
		default:
			break;
	}
	return nullptr;
}

bool fay::FayLangUtils::IsNumberType(ValueType type)
{
	switch(type)
	{
		case fay::ValueType::Byte:
		case fay::ValueType::Int:
		case fay::ValueType::Long:
		case fay::ValueType::Float:
		case fay::ValueType::Double:
			return true;
	}

	return false;
}

FayInst* fay::FayLangUtils::OPInst(InstGroupType op, ValueType type)
{
	//OPInstStart
	if (op == InstGroupType::Add && type == ValueType::Void)
		return new inst::AddVoid();
	else if (op == InstGroupType::Add && type == ValueType::Bool)
		return new inst::AddBool();
	else if (op == InstGroupType::Add && type == ValueType::Byte)
		return new inst::AddByte();
	else if (op == InstGroupType::Add && type == ValueType::Int)
		return new inst::AddInt();
	else if (op == InstGroupType::Add && type == ValueType::Long)
		return new inst::AddLong();
	else if (op == InstGroupType::Add && type == ValueType::Float)
		return new inst::AddFloat();
	else if (op == InstGroupType::Add && type == ValueType::Double)
		return new inst::AddDouble();
	else if (op == InstGroupType::Add && type == ValueType::String)
		return new inst::AddString();
	else if (op == InstGroupType::Add && type == ValueType::Object)
		return new inst::AddObject();
	else if (op == InstGroupType::Add && type == ValueType::Function)
		return new inst::AddFunction();
	else if (op == InstGroupType::Sub && type == ValueType::Void)
		return new inst::SubVoid();
	else if (op == InstGroupType::Sub && type == ValueType::Bool)
		return new inst::SubBool();
	else if (op == InstGroupType::Sub && type == ValueType::Byte)
		return new inst::SubByte();
	else if (op == InstGroupType::Sub && type == ValueType::Int)
		return new inst::SubInt();
	else if (op == InstGroupType::Sub && type == ValueType::Long)
		return new inst::SubLong();
	else if (op == InstGroupType::Sub && type == ValueType::Float)
		return new inst::SubFloat();
	else if (op == InstGroupType::Sub && type == ValueType::Double)
		return new inst::SubDouble();
	else if (op == InstGroupType::Sub && type == ValueType::String)
		return new inst::SubString();
	else if (op == InstGroupType::Sub && type == ValueType::Object)
		return new inst::SubObject();
	else if (op == InstGroupType::Sub && type == ValueType::Function)
		return new inst::SubFunction();
	else if (op == InstGroupType::Mul && type == ValueType::Void)
		return new inst::MulVoid();
	else if (op == InstGroupType::Mul && type == ValueType::Bool)
		return new inst::MulBool();
	else if (op == InstGroupType::Mul && type == ValueType::Byte)
		return new inst::MulByte();
	else if (op == InstGroupType::Mul && type == ValueType::Int)
		return new inst::MulInt();
	else if (op == InstGroupType::Mul && type == ValueType::Long)
		return new inst::MulLong();
	else if (op == InstGroupType::Mul && type == ValueType::Float)
		return new inst::MulFloat();
	else if (op == InstGroupType::Mul && type == ValueType::Double)
		return new inst::MulDouble();
	else if (op == InstGroupType::Mul && type == ValueType::String)
		return new inst::MulString();
	else if (op == InstGroupType::Mul && type == ValueType::Object)
		return new inst::MulObject();
	else if (op == InstGroupType::Mul && type == ValueType::Function)
		return new inst::MulFunction();
	else if (op == InstGroupType::Div && type == ValueType::Void)
		return new inst::DivVoid();
	else if (op == InstGroupType::Div && type == ValueType::Bool)
		return new inst::DivBool();
	else if (op == InstGroupType::Div && type == ValueType::Byte)
		return new inst::DivByte();
	else if (op == InstGroupType::Div && type == ValueType::Int)
		return new inst::DivInt();
	else if (op == InstGroupType::Div && type == ValueType::Long)
		return new inst::DivLong();
	else if (op == InstGroupType::Div && type == ValueType::Float)
		return new inst::DivFloat();
	else if (op == InstGroupType::Div && type == ValueType::Double)
		return new inst::DivDouble();
	else if (op == InstGroupType::Div && type == ValueType::String)
		return new inst::DivString();
	else if (op == InstGroupType::Div && type == ValueType::Object)
		return new inst::DivObject();
	else if (op == InstGroupType::Div && type == ValueType::Function)
		return new inst::DivFunction();
	else if (op == InstGroupType::Equal && type == ValueType::Void)
		return new inst::EqualVoid();
	else if (op == InstGroupType::Equal && type == ValueType::Bool)
		return new inst::EqualBool();
	else if (op == InstGroupType::Equal && type == ValueType::Byte)
		return new inst::EqualByte();
	else if (op == InstGroupType::Equal && type == ValueType::Int)
		return new inst::EqualInt();
	else if (op == InstGroupType::Equal && type == ValueType::Long)
		return new inst::EqualLong();
	else if (op == InstGroupType::Equal && type == ValueType::Float)
		return new inst::EqualFloat();
	else if (op == InstGroupType::Equal && type == ValueType::Double)
		return new inst::EqualDouble();
	else if (op == InstGroupType::Equal && type == ValueType::String)
		return new inst::EqualString();
	else if (op == InstGroupType::Equal && type == ValueType::Object)
		return new inst::EqualObject();
	else if (op == InstGroupType::Equal && type == ValueType::Function)
		return new inst::EqualFunction();
	else if (op == InstGroupType::Greater && type == ValueType::Void)
		return new inst::GreaterVoid();
	else if (op == InstGroupType::Greater && type == ValueType::Bool)
		return new inst::GreaterBool();
	else if (op == InstGroupType::Greater && type == ValueType::Byte)
		return new inst::GreaterByte();
	else if (op == InstGroupType::Greater && type == ValueType::Int)
		return new inst::GreaterInt();
	else if (op == InstGroupType::Greater && type == ValueType::Long)
		return new inst::GreaterLong();
	else if (op == InstGroupType::Greater && type == ValueType::Float)
		return new inst::GreaterFloat();
	else if (op == InstGroupType::Greater && type == ValueType::Double)
		return new inst::GreaterDouble();
	else if (op == InstGroupType::Greater && type == ValueType::String)
		return new inst::GreaterString();
	else if (op == InstGroupType::Greater && type == ValueType::Object)
		return new inst::GreaterObject();
	else if (op == InstGroupType::Greater && type == ValueType::Function)
		return new inst::GreaterFunction();
	else if (op == InstGroupType::Less && type == ValueType::Void)
		return new inst::LessVoid();
	else if (op == InstGroupType::Less && type == ValueType::Bool)
		return new inst::LessBool();
	else if (op == InstGroupType::Less && type == ValueType::Byte)
		return new inst::LessByte();
	else if (op == InstGroupType::Less && type == ValueType::Int)
		return new inst::LessInt();
	else if (op == InstGroupType::Less && type == ValueType::Long)
		return new inst::LessLong();
	else if (op == InstGroupType::Less && type == ValueType::Float)
		return new inst::LessFloat();
	else if (op == InstGroupType::Less && type == ValueType::Double)
		return new inst::LessDouble();
	else if (op == InstGroupType::Less && type == ValueType::String)
		return new inst::LessString();
	else if (op == InstGroupType::Less && type == ValueType::Object)
		return new inst::LessObject();
	else if (op == InstGroupType::Less && type == ValueType::Function)
		return new inst::LessFunction();
	//OPInstEnd

	return nullptr;
}

std::string fay::FayLangUtils::Fullname(const std::string &className, const std::string &funName, const std::vector<PTR(FayClass)> &params)
{
	return className + "." + FayLangUtils::Fullname(funName, params);
}

void fay::OutsideFun::toString(mirror::utils::StringBuilder* sb)
{
	sb->add(this->_typeFullname)->add(":")->add(this->_typeIndex)->add(" ");
	sb->add(this->_funFullname)->add(":")->add(this->_funIndex)->endl();
}

fay::FayInternalFun::FayInternalFun(PTR(FayDomain) domain, const std::string &name, std::function<void(std::stack<PTR(FayValue)>&)> fun, std::vector<std::string> params)
	: FayFun(domain, name, FunType::Internal, true), _fun(fun)
{
	for(auto i = 0; i < params.size(); ++i)
	{
		auto it = params[i];
		std::string paramName = "p" + std::to_string(i);
		auto t = domain->findType(it);
		PTR(FayParamDef) p = MKPTR(FayParamDef)(domain, paramName, t);
		this->addParam(p);
	}
}

pos_t fay::FunTable::addFun(PTR(FayFun) fun)
{
	for(auto i = 0; i < this->_funs.size(); ++i)
	{
		//如果是同一个函数，用新的替换旧的
		if(this->_funs[i]->fullname() == fun->fullname())
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
	if(index >= 0 && index < this->_funs.size())
		return this->_funs[index];

	return nullptr;
}

void fay::FunTable::rebuild(std::vector<PTR(FayFun)> &parentFuns)
{
	std::vector<PTR(FayFun)> funs = this->_funs;
	this->_funs = parentFuns;

	for(auto i = 0; i < funs.size(); ++i)
		this->addFun(funs[i]);
}

std::vector<pos_t> fay::FunTable::matchFun(const std::string &funName, const std::vector<PTR(FayClass)> &paramsType)
{
	std::vector<pos_t> funs;

	for(auto i = 0; i < this->_funs.size(); ++i)
	{
		auto it = this->_funs[i];
		if(it->name() == funName && it->match(paramsType))
			funs.push_back(i);
	}

	return funs;
}

pos_t fay::FunTable::findFunIndex(const std::string &fullname)
{
	for(auto i = 0; i < this->_funs.size(); ++i)
	{
		if(this->_funs[i]->fullname() == fullname)
			return i;
	}

	return -1;
}

PTR(FayFun) fay::FunTable::findFun(const std::string &fullname)
{
	for(auto i = 0; i < this->_funs.size(); ++i)
	{
		if(this->_funs[i]->fullname() == fullname)
			return this->_funs[i];
	}

	return nullptr;
}

void fay::FunTable::toString(mirror::utils::StringBuilder* sb)
{
	for(auto i = 0; i < this->_funs.size(); ++i)
	{
		sb->add(i)->add(" : ");
		this->_funs[i]->toString(sb);
	}
}

const std::string &fay::FayVarDef::fullname()
{
	if(!this->_fullname.size())
		this->_fullname = this->_name + ":" + this->_class.lock()->fullname();

	return this->_fullname;
}

void fay::FayVarDef::toString(mirror::utils::StringBuilder* sb)
{
	sb->add(this->fullname());
}

void fay::FayLabel::addTarget(int32_t* target)
{
	if(this->_pos >= 0)
		(*target) = this->_pos;
	this->_targets.push_back(target);
}

void fay::FayLabel::setPos(int32_t pos)
{
	this->_pos = pos;
	for each(auto it in this->_targets)
		(*it) = pos;
}

PTR(FayLabel) fay::FayLabelTable::findLabel(const std::string &name)
{
	auto it = this->_labels.find(name);
	if(it == this->_labels.end())
	{
		PTR(FayLabel) label = MKPTR(FayLabel)(name);
		this->_labels[name] = label;
		return label;
	}
	else
		return it->second;
}

void fay::FayLabelTable::addLabel(const std::string &name)
{
	this->findLabel(name);
}

void fay::FayLabelTable::addTarget(const std::string &name, int32_t* target)
{
	this->findLabel(name)->addTarget(target);
}

void fay::FayLabelTable::setPos(const std::string &name, int32_t pos)
{
	this->findLabel(name)->setPos(pos);
}
