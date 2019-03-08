#include "fay_lang.h"
#include "fay_lang.h"
#include "fay_lang.h"
#include <fay_lang.h>
#include <mirror_utils_log.h>
#include <mirror_utils_file.h>
#include <fay_internal_fun.h>
#include <fay_internal_object.h>
#include <fay_i18n.h>
#include <fay_vm.h>

using namespace fay;
using namespace fay::internal;
using namespace mirror;

fay::FayClass::FayClass(const std::string &package, const std::string &name)
	: _package(package), _name(name)
{
	this->_fullname = (!package.empty() ? package + "." : "") + name;
}

void fay::FayClass::initClass()
{
	if(!this->_isInited)
	{
		this->_isInited = true;

		//执行初始化方法
		auto funs = this->findFunByName(FUN_STATIC_INIT, true);
		for(const auto &fun : funs)
			FayVM::Run(fun);
	}
}

void fay::FayClass::rebuild()
{
	if(!this->_isRebuild)
	{
		this->_isRebuild = true;

		auto superClass = this->_superClass.lock();
		if(superClass)
		{
			superClass->rebuild();
			this->_vft.rebuild(&superClass->_vft);
		}
	}
}

bool fay::FayClass::match(PTR(FayClass) type)
{
	//TODO ：对接口的处理

	if(type && type.get() == this)
		return true;

	PTR(FayClass) parent = this->_superClass.lock();
	if(parent && parent->match(type))
		return true;

	return false;
}

pos_t fay::FayClass::addStaticVar(const std::string &name, PTR(FayClass) classType)
{
	PTR(FayStaticVarDef) var = MKPTR(FayStaticVarDef)(name, classType);
	return this->_staticVarDefs.add(var);
}

PTR(FayStaticVarDef) fay::FayClass::findStaticVar(const std::string &name)
{
	return this->_staticVarDefs.find(name);
}

PTR(FayStaticVarDef) fay::FayClass::findStaticVar(pos_t index)
{
	return this->_staticVarDefs[index];
}

FayValue &fay::FayClass::staticVar(pos_t index)
{
	return this->_staticVarDefs[index]->value;
}

pos_t fay::FayClass::addVar(const std::string &name, PTR(FayClass) type)
{
	PTR(FayVarDef) field = MKPTR(FayVarDef)(name, type);
	return this->_varDefs.add(field);
}

PTR(FayVarDef) fay::FayClass::findVar(const std::string &name)
{
	return this->_varDefs.find(name);
}

PTR(FayVarDef) fay::FayClass::findVar(pos_t index)
{
	return this->_varDefs[index];
}

std::vector<PTR(FayFun)> fay::FayClass::findFun(const std::string &funName, const std::vector<PTR(FayClass)> &paramsType, bool isStatic)
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

std::vector<PTR(FayFun)> fay::FayClass::findFunByName(const std::string &name, bool isStatic)
{
	std::vector<PTR(FayFun)> r;
	if(isStatic)
		this->_sft.findFunByName(name, r);
	else
		this->_vft.findFunByName(name, r);
	return r;
}

bool fay::FayClass::canCovertTo(PTR(FayClass) type)
{
	if(type)
	{
		if(type.get() == this)
			return true;

		if(!this->_superClass.expired())
			return this->_superClass.lock()->canCovertTo(type);
	}

	return false;
}

void fay::FayClass::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add("[Class] ")->add(this->fullname());
	if(!this->_superClass.expired())
		sb->add(" : ")->add(this->_superClass.lock()->fullname());
	sb->endl();

	sb->increaseIndent();
	{
		if(this->_sft.size() > 0)
		{
			sb->add("[SFT] ")->endl();
			sb->increaseIndent();
			this->_sft.buildString(sb);
			sb->decreaseIndent();
		}

		if(this->_vft.size() > 0)
		{
			sb->add("[VFT] ")->endl();
			sb->increaseIndent();
			this->_vft.buildString(sb);
			sb->decreaseIndent();
		}
	}
	sb->decreaseIndent();
}

pos_t fay::FayLib::addClass(PTR(FayClass) clazz)
{
	clazz->lib(this->shared_from_this());
	this->classes.push_back(clazz);

	return FayDomain::AddClass(clazz);
}

PTR(std::vector<PTR(FayFun)>) fay::FayLib::findMainFun()
{
	PTR(std::vector<PTR(FayFun)>) funs = MKPTR(std::vector<PTR(FayFun)>)();

	for(const auto &clazz : this->classes)
	{
		auto fs = clazz->findFun("Main():void", true);
		if(fs)
			funs->push_back(fs);
	}

	return funs;
}

void fay::FayLib::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayLib]")->add(this->name)->endl();
	sb->increaseIndent();
	//显示Class的内容
	sb->add("[Classes]")->endl();
	sb->increaseIndent();
	for each(auto it in this->classes)
		it->buildString(sb);
	sb->decreaseIndent();
	sb->decreaseIndent();
}

void fay::FayInstFun::prepareInsts()
{
	for(FayInst* inst : this->_insts)
	{
		switch(inst->type())
		{
			case InstType::CallStatic:
			{
				//取出调用方法的索引值
				auto* cmd = static_cast<inst::CallStatic*>(inst);
				auto clazz = FayDomain::FindClass(cmd->className);
				if(!clazz)
				{
					LOG_ERROR("Cannot find class : " << cmd->className);
					break;
				}

				auto fun = clazz->findFun(cmd->funName, true);
				if(!fun)
				{
					LOG_ERROR("Cannot find static fun : " << cmd->className << "." << cmd->funName);
					break;
				}

				cmd->classIndex = clazz->indexValue();
				cmd->funIndex = fun->indexValue();
				break;
			}
			case InstType::CallVirtual:
			{
				//取出调用方法的索引值
				auto* cmd = static_cast<inst::CallVirtual*>(inst);
				auto clazz = FayDomain::FindClass(cmd->className);
				if(!clazz)
				{
					LOG_ERROR("Cannot find class : " << cmd->className);
					break;
				}

				auto fun = clazz->findFun(cmd->funName, false);
				if(!fun)
				{
					LOG_ERROR("Cannot find static fun : " << cmd->className << "." << cmd->funName);
					break;
				}

				cmd->classIndex = clazz->indexValue();
				cmd->funIndex = fun->indexValue();
				break;
			}
			case InstType::SetStatic:
			{
				auto* cmd = static_cast<inst::SetStatic*>(inst);
				auto clazz = FayDomain::FindClass(cmd->className);
				if(!clazz)
				{
					LOG_ERROR("Cannot find class : " << cmd->className);
					break;
				}
				cmd->classIndex = clazz->indexValue();

				auto var = clazz->findStaticVar(cmd->varName);
				if(!var)
				{
					LOG_ERROR("Cannot find static var : " << cmd->className << "." << cmd->varName);
					break;
				}
				cmd->varIndex = var->indexValue();

				break;
			}
			case InstType::LoadStatic:
			{
				auto* cmd = static_cast<inst::LoadStatic*>(inst);
				auto clazz = FayDomain::FindClass(cmd->className);
				if(!clazz)
				{
					LOG_ERROR("Cannot find class : " << cmd->className);
					break;
				}
				cmd->classIndex = clazz->indexValue();

				auto var = clazz->findStaticVar(cmd->varName);
				if(!var)
				{
					LOG_ERROR("Cannot find static var : " << cmd->className << "." << cmd->varName);
					break;
				}
				cmd->varIndex = var->indexValue();

				break;
			}
			case InstType::NewObject:
			{
				auto* cmd = static_cast<inst::NewObject*>(inst);
				auto clazz = FayDomain::FindClass(cmd->className);
				if(clazz == nullptr)
					throw FayLangException("Cannot find class : " + cmd->className);

				cmd->classIndex = clazz->indexValue();
				break;
			}

		}
	}
}

fay::FayInstFun::~FayInstFun()
{
	for(auto it : this->_insts)
		delete it;

	for(auto it : this->_handlers)
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
		if(it->classType() != type)
		{
			LOG_ERROR("Same var name, diff type : " << it->classType() << ", " << type);
			return -1;
		}

		return this->_vars.findIndex(name);
	}

	//创建并加入变量定义
	PTR(FayVarDef) def = MKPTR(FayVarDef)(name, type);
	return this->_vars.add(def);
}

PTR(FayVarDef) fay::FayInstFun::findVar(const std::string &name)
{
	return this->_vars.find(name);
}

pos_t fay::FayInstFun::getVarIndex(const std::string &name)
{
	return this->_vars.findIndex(name);
}

TryHandler* fay::FayInstFun::findHandler(int ip)
{
	TryHandler* handler = nullptr;

	for(auto it : this->_handlers)
	{
		if(ip >= it->start && ip <= it->end)
			handler = it;
	}

	return handler;
}

void fay::FayInstFun::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayInstFun] ")->add(this->fullname())->endl();

	sb->increaseIndent();

	if(!this->_params.empty())
	{
		sb->add("[Params] ")->endl();
		sb->increaseIndent();
		for(auto i = 0; i < this->_params.size(); ++i)
		{
			sb->add(i)->add("> ");
			this->_params[i]->buildString(sb);
		}
		sb->decreaseIndent();
	}

	if(!this->_vars.list().empty())
	{
		sb->add("[Vars] ")->endl();
		sb->increaseIndent();
		for(auto i = 0; i < this->_vars.list().size(); ++i)
		{
			auto it = this->_vars.list()[i];
			sb->add(i)->add("> ")->add(it->name())->add(":")->add(it->classType()->fullname())->endl();
		}
		sb->decreaseIndent();
	}

	if(!this->_handlers.empty())
	{
		sb->add("[Handler] ")->endl();
		sb->increaseIndent();
		for(auto it : this->_handlers)
		{
			it->buildString(sb);
			sb->endl();
		}
		sb->decreaseIndent();
	}

	if(!this->_insts.empty())
	{
		sb->add("[Inst] ")->endl();
		sb->increaseIndent();
		for(auto i = 0; i < this->_insts.size(); ++i)
		{
			auto it = this->_insts[i];
			sb->add(i)->add("> ");
			if(it)
				it->buildString(sb);
			else
				sb->add("<nullptr>");
		}
		sb->decreaseIndent();
	}

	sb->decreaseIndent();
}

fay::FayFun::FayFun(const std::string &name, FunType type, bool isStatic, FunAccessType accessType, std::vector<PTR(FayParamDef)> &params, PTR(FayClass) returnValue)
	: _name(name), _type(type), _isStatic(isStatic), _accessType(accessType), _params(params), _returnValue(returnValue)
{
	std::string str;
	for each(auto it in this->_params)
	{
		if(str.size() > 0)
			str += ",";
		str += it->fullname();
	}

	_fullname += this->_name;
	_fullname += "(" + str + "):";

	if(returnValue == nullptr)
		_fullname += "void";
	else
		_fullname += returnValue->fullname();

	//LOG_DEBUG("Fun : " << _fullname);
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

void fay::FayFun::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayFun]")->add(this->fullname())->endl();
}

pos_t fay::FayClass::addFun(PTR(FayFun) fun)
{
	fun->clazz(this->shared_from_this());
	if(fun->isStatic())
		return this->_sft.addFun(fun);
	else
		return this->_vft.addFun(fun);
}

std::vector<PTR(FayLib)> fay::FayDomain::_libs;

fay::IndexMap<fay::FayClass> fay::FayDomain::_classes;


void fay::FayDomain::InitSysLib()
{
	//添加系统库。因为存在循环依赖的问题，要分两次进行初始化
	PTR(SystemLib) lib = MKPTR(SystemLib)("system", 1, 1);
	lib->preInit();
	FayDomain::AddLib(lib);
	lib->postInit();
}

void fay::FayDomain::AddLib(PTR(FayLib) lib)
{
	//先生成相互的引用关系
	FayDomain::_libs.push_back(lib);

	//然后对Lib里的东西进行一下排序
	for each(auto it in lib->classes)
		FayDomain::_classes.add(it);
}

void fay::FayDomain::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayDomain]")->endl();
	sb->increaseIndent();
	for(auto i = 0; i < FayDomain::_classes.list().size(); ++i)
	{
		auto t = FayDomain::_classes.list()[i];
		sb->add(i)->add(" : ")->add(t->fullname())->endl();
	}

	for each(auto it in FayDomain::_libs)
		it->buildString(sb);
	sb->decreaseIndent();
}

std::string fay::FayDomain::ToString()
{
	utils::StringBuilder sb;
	FayDomain::buildString(&sb);
	return sb.toString();
}

pos_t fay::FayDomain::AddClass(PTR(FayClass) t)
{
	std::string fullname = t->fullname();

	//如果已经有了，就返回现有的位置
	pos_t index = FayDomain::_classes.findIndex(fullname);
	if(index >= 0)
		return index;

	return FayDomain::_classes.add(t);
}

PTR(FayClass) fay::FayDomain::FindClass(const std::string &fullname)
{
	auto type = FayDomain::_classes.find(fullname);
	return type;
}

PTR(FayClass) fay::FayDomain::FindClass(pos_t index)
{
	auto type = FayDomain::_classes.find(index);
	if(!type)
		LOG_ERROR("Cannot find type by index : " << index);

	return type;
}

PTR(FayClass) fay::FayDomain::FindClass(ValueType type)
{
	return FayDomain::FindClass(TypeDict::ToName(type));
}

std::vector<PTR(FayClass)> fay::FayDomain::FindClass(std::vector<std::string> &imports, const std::string &typeName)
{
	std::vector<PTR(FayClass)> types;

	//直接根据名称查找
	auto type = FayDomain::FindClass(typeName);
	if(type)
		types.push_back(type);

	//加上import的前缀查找
	for each(auto it in imports)
	{
		std::string typeFullname = it + "." + typeName;
		auto type = FayDomain::FindClass(typeFullname);
		if(type)
			types.push_back(type);
	}

	return types;
}

PTR(FayClass) fay::FayDomain::UseClass(pos_t index)
{
	PTR(FayClass) clazz = FayDomain::_classes[index];
	if(!clazz)
	{
		LOG_ERROR("Cannot find fun : " << index);
		return nullptr;
	}

	//运行初始化方法
	if(!clazz->isInited())
		clazz->initClass();

	return clazz;
}

fay::FayParamDef::FayParamDef(const std::string &name, PTR(FayClass) type)
	: _name(name), _class(type)
{
	this->_fullname = this->_name;
	this->_fullname += ":";
	this->_fullname += this->_class.expired() ? "?" : this->_class.lock()->fullname();
}

void fay::FayParamDef::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add(this->fullname())->endl();
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

PTR(FayClass) fay::FayLangUtils::WeightValueType(PTR(FayClass) t1, PTR(FayClass) t2)
{
	if(t1->valueType() >= ValueType::String || t2->valueType() >= ValueType::String)
		return FayDomain::FindClass(ValueType::String);

	return (t1->valueType() >= t2->valueType()) ? t1 : t2;
}

ValueType fay::FayLangUtils::ClassToValueType(PTR(FayClass) clazz)
{
	return clazz->valueType();
}

fay::FayInst* fay::FayLangUtils::ConvertInst(ValueType src, ValueType dest)
{
	//ConvertInstStart
	switch(src)
	{
		case ValueType::Void:
			switch(dest)
			{
				case ValueType::Void:
					return new inst::VoidToVoid();
				case ValueType::Bool:
					return new inst::VoidToBool();
				case ValueType::Byte:
					return new inst::VoidToByte();
				case ValueType::Int:
					return new inst::VoidToInt();
				case ValueType::Long:
					return new inst::VoidToLong();
				case ValueType::Float:
					return new inst::VoidToFloat();
				case ValueType::Double:
					return new inst::VoidToDouble();
				case ValueType::String:
					return new inst::VoidToString();
			}
			break;
		case ValueType::Bool:
			switch(dest)
			{
			}
			break;
		case ValueType::Byte:
			switch(dest)
			{
				case ValueType::String:
					return new inst::ByteToString();
			}
			break;
		case ValueType::Int:
			switch(dest)
			{
				case ValueType::Bool:
					return new inst::IntToBool();
				case ValueType::Byte:
					return new inst::IntToByte();
				case ValueType::Int:
					return new inst::IntToInt();
				case ValueType::Long:
					return new inst::IntToLong();
				case ValueType::Float:
					return new inst::IntToFloat();
				case ValueType::Double:
					return new inst::IntToDouble();
				case ValueType::String:
					return new inst::IntToString();
			}
			break;
		case ValueType::Long:
			switch(dest)
			{
				case ValueType::Bool:
					return new inst::LongToBool();
				case ValueType::Byte:
					return new inst::LongToByte();
				case ValueType::Int:
					return new inst::LongToInt();
				case ValueType::Long:
					return new inst::LongToLong();
				case ValueType::Float:
					return new inst::LongToFloat();
				case ValueType::Double:
					return new inst::LongToDouble();
				case ValueType::String:
					return new inst::LongToString();
			}
			break;
		case ValueType::Float:
			switch(dest)
			{
				case ValueType::Bool:
					return new inst::FloatToBool();
				case ValueType::Byte:
					return new inst::FloatToByte();
				case ValueType::Int:
					return new inst::FloatToInt();
				case ValueType::Long:
					return new inst::FloatToLong();
				case ValueType::Float:
					return new inst::FloatToFloat();
				case ValueType::Double:
					return new inst::FloatToDouble();
				case ValueType::String:
					return new inst::FloatToString();
			}
			break;
		case ValueType::Double:
			switch(dest)
			{
				case ValueType::Bool:
					return new inst::DoubleToBool();
				case ValueType::Byte:
					return new inst::DoubleToByte();
				case ValueType::Int:
					return new inst::DoubleToInt();
				case ValueType::Long:
					return new inst::DoubleToLong();
				case ValueType::Float:
					return new inst::DoubleToFloat();
				case ValueType::Double:
					return new inst::DoubleToDouble();
				case ValueType::String:
					return new inst::DoubleToString();
			}
			break;
		case ValueType::String:
			switch(dest)
			{
				case ValueType::Bool:
					return new inst::StringToBool();
				case ValueType::Byte:
					return new inst::StringToByte();
				case ValueType::Int:
					return new inst::StringToInt();
				case ValueType::Long:
					return new inst::StringToLong();
				case ValueType::Float:
					return new inst::StringToFloat();
				case ValueType::Double:
					return new inst::StringToDouble();
				case ValueType::String:
					return new inst::StringToString();
			}
			break;
		case ValueType::Object:
			switch(dest)
			{
			}
			break;
		case ValueType::Function:
			switch(dest)
			{
			}
			break;
		case ValueType::Any:
			switch(dest)
			{
			}
			break;
	}
	//ConvertInstEnd

	return nullptr;
}

FayInst* fay::FayLangUtils::PushDefault(PTR(FayClass) type)
{
	switch(type->valueType())
	{
		case fay::ValueType::Void:
			break;
		case fay::ValueType::Bool:
			return new inst::PushBool(false);
			break;
		case fay::ValueType::Byte:
			return new inst::PushByte((byte)0);
			break;
		case fay::ValueType::Int:
			return new inst::PushInt((int32_t)0);
			break;
		case fay::ValueType::Long:
			return new inst::PushLong((int64_t)0);
			break;
		case fay::ValueType::Float:
			return new inst::PushFloat((float)0);
			break;
		case fay::ValueType::Double:
			return new inst::PushDouble((double)0);
			break;
		case fay::ValueType::String:
			return new inst::PushString("");
			break;
		case fay::ValueType::Object:
			return new inst::NullObject(type->fullname());
			break;
		case fay::ValueType::Function:
			break;
		default:
			break;
	}
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

bool fay::FayLangUtils::IsIntegerNumberType(ValueType type)
{
	switch(type)
	{
		case fay::ValueType::Byte:
		case fay::ValueType::Int:
		case fay::ValueType::Long:
			return true;
	}

	return false;
}


FayInst* fay::FayLangUtils::OPInst(InstGroupType op, ValueType type)
{
	//OPInstStart
	switch(op)
	{
		case InstGroupType::VoidTo:
			switch(type)
			{
				case ValueType::Void:
					return new inst::VoidToVoid();
				case ValueType::Bool:
					return new inst::VoidToBool();
				case ValueType::Byte:
					return new inst::VoidToByte();
				case ValueType::Int:
					return new inst::VoidToInt();
				case ValueType::Long:
					return new inst::VoidToLong();
				case ValueType::Float:
					return new inst::VoidToFloat();
				case ValueType::Double:
					return new inst::VoidToDouble();
				case ValueType::String:
					return new inst::VoidToString();
			}
			break;
		case InstGroupType::ByteTo:
			switch(type)
			{
				case ValueType::String:
					return new inst::ByteToString();
			}
			break;
		case InstGroupType::IntTo:
			switch(type)
			{
				case ValueType::Bool:
					return new inst::IntToBool();
				case ValueType::Byte:
					return new inst::IntToByte();
				case ValueType::Int:
					return new inst::IntToInt();
				case ValueType::Long:
					return new inst::IntToLong();
				case ValueType::Float:
					return new inst::IntToFloat();
				case ValueType::Double:
					return new inst::IntToDouble();
				case ValueType::String:
					return new inst::IntToString();
			}
			break;
		case InstGroupType::LongTo:
			switch(type)
			{
				case ValueType::Bool:
					return new inst::LongToBool();
				case ValueType::Byte:
					return new inst::LongToByte();
				case ValueType::Int:
					return new inst::LongToInt();
				case ValueType::Long:
					return new inst::LongToLong();
				case ValueType::Float:
					return new inst::LongToFloat();
				case ValueType::Double:
					return new inst::LongToDouble();
				case ValueType::String:
					return new inst::LongToString();
			}
			break;
		case InstGroupType::FloatTo:
			switch(type)
			{
				case ValueType::Bool:
					return new inst::FloatToBool();
				case ValueType::Byte:
					return new inst::FloatToByte();
				case ValueType::Int:
					return new inst::FloatToInt();
				case ValueType::Long:
					return new inst::FloatToLong();
				case ValueType::Float:
					return new inst::FloatToFloat();
				case ValueType::Double:
					return new inst::FloatToDouble();
				case ValueType::String:
					return new inst::FloatToString();
			}
			break;
		case InstGroupType::DoubleTo:
			switch(type)
			{
				case ValueType::Bool:
					return new inst::DoubleToBool();
				case ValueType::Byte:
					return new inst::DoubleToByte();
				case ValueType::Int:
					return new inst::DoubleToInt();
				case ValueType::Long:
					return new inst::DoubleToLong();
				case ValueType::Float:
					return new inst::DoubleToFloat();
				case ValueType::Double:
					return new inst::DoubleToDouble();
				case ValueType::String:
					return new inst::DoubleToString();
			}
			break;
		case InstGroupType::StringTo:
			switch(type)
			{
				case ValueType::Bool:
					return new inst::StringToBool();
				case ValueType::Byte:
					return new inst::StringToByte();
				case ValueType::Int:
					return new inst::StringToInt();
				case ValueType::Long:
					return new inst::StringToLong();
				case ValueType::Float:
					return new inst::StringToFloat();
				case ValueType::Double:
					return new inst::StringToDouble();
				case ValueType::String:
					return new inst::StringToString();
			}
			break;
		case InstGroupType::Minus:
			switch(type)
			{
				case ValueType::Byte:
					return new inst::MinusByte();
				case ValueType::Int:
					return new inst::MinusInt();
				case ValueType::Long:
					return new inst::MinusLong();
				case ValueType::Float:
					return new inst::MinusFloat();
				case ValueType::Double:
					return new inst::MinusDouble();
			}
			break;
		case InstGroupType::Add:
			switch(type)
			{
				case ValueType::Byte:
					return new inst::AddByte();
				case ValueType::Int:
					return new inst::AddInt();
				case ValueType::Long:
					return new inst::AddLong();
				case ValueType::Float:
					return new inst::AddFloat();
				case ValueType::Double:
					return new inst::AddDouble();
				case ValueType::String:
					return new inst::AddString();
			}
			break;
		case InstGroupType::Sub:
			switch(type)
			{
				case ValueType::Byte:
					return new inst::SubByte();
				case ValueType::Int:
					return new inst::SubInt();
				case ValueType::Long:
					return new inst::SubLong();
				case ValueType::Float:
					return new inst::SubFloat();
				case ValueType::Double:
					return new inst::SubDouble();
			}
			break;
		case InstGroupType::Mul:
			switch(type)
			{
				case ValueType::Int:
					return new inst::MulInt();
				case ValueType::Long:
					return new inst::MulLong();
				case ValueType::Float:
					return new inst::MulFloat();
				case ValueType::Double:
					return new inst::MulDouble();
			}
			break;
		case InstGroupType::Div:
			switch(type)
			{
				case ValueType::Int:
					return new inst::DivInt();
				case ValueType::Long:
					return new inst::DivLong();
				case ValueType::Float:
					return new inst::DivFloat();
				case ValueType::Double:
					return new inst::DivDouble();
			}
			break;
		case InstGroupType::Mod:
			switch(type)
			{
				case ValueType::Byte:
					return new inst::ModByte();
				case ValueType::Int:
					return new inst::ModInt();
				case ValueType::Long:
					return new inst::ModLong();
			}
			break;
		case InstGroupType::Equal:
			switch(type)
			{
				case ValueType::Bool:
					return new inst::EqualBool();
				case ValueType::Byte:
					return new inst::EqualByte();
				case ValueType::Int:
					return new inst::EqualInt();
				case ValueType::Long:
					return new inst::EqualLong();
				case ValueType::Float:
					return new inst::EqualFloat();
				case ValueType::Double:
					return new inst::EqualDouble();
				case ValueType::String:
					return new inst::EqualString();
			}
			break;
		case InstGroupType::NotEqual:
			switch(type)
			{
				case ValueType::Bool:
					return new inst::NotEqualBool();
				case ValueType::Byte:
					return new inst::NotEqualByte();
				case ValueType::Int:
					return new inst::NotEqualInt();
				case ValueType::Long:
					return new inst::NotEqualLong();
				case ValueType::Float:
					return new inst::NotEqualFloat();
				case ValueType::Double:
					return new inst::NotEqualDouble();
			}
			break;
		case InstGroupType::Greater:
			switch(type)
			{
				case ValueType::Bool:
					return new inst::GreaterBool();
				case ValueType::Byte:
					return new inst::GreaterByte();
				case ValueType::Int:
					return new inst::GreaterInt();
				case ValueType::Long:
					return new inst::GreaterLong();
				case ValueType::Float:
					return new inst::GreaterFloat();
				case ValueType::Double:
					return new inst::GreaterDouble();
			}
			break;
		case InstGroupType::GreaterEqual:
			switch(type)
			{
				case ValueType::Bool:
					return new inst::GreaterEqualBool();
				case ValueType::Byte:
					return new inst::GreaterEqualByte();
				case ValueType::Int:
					return new inst::GreaterEqualInt();
				case ValueType::Long:
					return new inst::GreaterEqualLong();
				case ValueType::Float:
					return new inst::GreaterEqualFloat();
				case ValueType::Double:
					return new inst::GreaterEqualDouble();
			}
			break;
		case InstGroupType::Less:
			switch(type)
			{
				case ValueType::Bool:
					return new inst::LessBool();
				case ValueType::Byte:
					return new inst::LessByte();
				case ValueType::Int:
					return new inst::LessInt();
				case ValueType::Long:
					return new inst::LessLong();
				case ValueType::Float:
					return new inst::LessFloat();
				case ValueType::Double:
					return new inst::LessDouble();
			}
			break;
		case InstGroupType::LessEqual:
			switch(type)
			{
				case ValueType::Bool:
					return new inst::LessEqualBool();
				case ValueType::Byte:
					return new inst::LessEqualByte();
				case ValueType::Int:
					return new inst::LessEqualInt();
				case ValueType::Long:
					return new inst::LessEqualLong();
				case ValueType::Float:
					return new inst::LessEqualFloat();
				case ValueType::Double:
					return new inst::LessEqualDouble();
			}
			break;
		case InstGroupType::BitNot:
			switch(type)
			{
				case ValueType::Byte:
					return new inst::BitNotByte();
				case ValueType::Int:
					return new inst::BitNotInt();
				case ValueType::Long:
					return new inst::BitNotLong();
			}
			break;
		case InstGroupType::BitAnd:
			switch(type)
			{
				case ValueType::Byte:
					return new inst::BitAndByte();
				case ValueType::Int:
					return new inst::BitAndInt();
				case ValueType::Long:
					return new inst::BitAndLong();
			}
			break;
		case InstGroupType::BitOr:
			switch(type)
			{
				case ValueType::Byte:
					return new inst::BitOrByte();
				case ValueType::Int:
					return new inst::BitOrInt();
				case ValueType::Long:
					return new inst::BitOrLong();
			}
			break;
		case InstGroupType::BitXor:
			switch(type)
			{
				case ValueType::Byte:
					return new inst::BitXorByte();
				case ValueType::Int:
					return new inst::BitXorInt();
				case ValueType::Long:
					return new inst::BitXorLong();
			}
			break;
		case InstGroupType::LShift:
			switch(type)
			{
				case ValueType::Byte:
					return new inst::LShiftByte();
				case ValueType::Int:
					return new inst::LShiftInt();
				case ValueType::Long:
					return new inst::LShiftLong();
			}
			break;
		case InstGroupType::RShift:
			switch(type)
			{
				case ValueType::Byte:
					return new inst::RShiftByte();
				case ValueType::Int:
					return new inst::RShiftInt();
				case ValueType::Long:
					return new inst::RShiftLong();
			}
	}
	//OPInstEnd

	return nullptr;
}

std::string fay::FayLangUtils::Fullname(const std::string &className, const std::string &funName, const std::vector<PTR(FayClass)> &params)
{
	return className + "." + FayLangUtils::Fullname(funName, params);
}

fay::StaticFunRef::StaticFunRef(const std::string &fullname, PTR(FayFun) fun)
	: _fullname(fullname)
{
	this->classIndex = fun->clazz()->indexValue();
	this->funIndex = fun->indexValue();
}

void fay::StaticFunRef::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add(this->_fullname)->add(" @ ")->add(this->classIndex)->add(",")->add(this->funIndex)->endl();
}

pos_t fay::FunTable::addFun(PTR(FayFun) fun)
{
	for(auto i = 0; i < this->_funs.size(); ++i)
	{
		//如果是同一个函数，用新的替换旧的
		if(this->_funs[i]->fullname() == fun->fullname())
		{
			this->_funs[i] = fun;
			fun->_indexValue = i;
			return fun->_indexValue;
		}
	}

	//如果当前没有这个函数，就加到最后
	this->_funs.push_back(fun);
	fun->_indexValue = this->_funs.size() - 1;
	return fun->_indexValue;
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

	for(const auto &fun : funs)
		this->addFun(fun);
}

void fay::FunTable::rebuild(FunTable* parent)
{
	std::vector<PTR(FayFun)> funs = this->_funs;
	this->_funs = parent->_funs;

	for(const auto &fun : funs)
		this->addFun(fun);
}

std::vector<PTR(FayFun)> fay::FunTable::matchFun(const std::string &funName, const std::vector<PTR(FayClass)> &paramsType)
{
	std::vector<PTR(FayFun)> funs;

	for(const auto &it : this->_funs)
	{
		if(it->name() == funName && it->match(paramsType))
			funs.push_back(it);
	}

	return funs;
}

pos_t fay::FunTable::getFunIndex(const std::string &fullname)
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
	for(auto &fun : this->_funs)
	{
		if(fun->fullname() == fullname)
			return fun;
	}

	return nullptr;
}

std::vector<PTR(FayFun)> fay::FunTable::findFunByName(const std::string &name)
{
	std::vector<PTR(FayFun)> list;
	this->findFunByName(name, list);
	return list;
}

void fay::FunTable::findFunByName(const std::string &name, std::vector<PTR(FayFun)> &list)
{
	for(const auto &fun : this->_funs)
	{
		if(fun->name() == name)
			list.push_back(fun);
	}
}

void fay::FunTable::buildString(mirror::utils::StringBuilder* sb)
{
	for(auto i = 0; i < this->_funs.size(); ++i)
	{
		sb->add(i)->add("> ");
		this->_funs[i]->buildString(sb);
	}
}

fay::FayVar::FayVar(const std::string &name, PTR(FayClass) clazz)
	: _name(name), _classType(clazz)
{
	this->_fullname = name + ":" + clazz->fullname();
}

void fay::FayVar::buildString(mirror::utils::StringBuilder* sb)
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

void fay::FayLabel::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add(this->_name)->add(" => ")->add(this->_pos);
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

fay::ValueType fay::FayInstClass::valueType()
{
	return ValueType::Object;
}

fay::StaticVarRef::StaticVarRef(const std::string &fullname, PTR(FayClass) clazz, PTR(FayStaticVarDef) var)
	: _fullname(fullname)
{
	this->_className = clazz->fullname();
	this->_classIndex = clazz->indexValue();
	this->_varName = var->name();
	this->_varIndex = var->indexValue();
	this->_fullname = this->_className + "." + this->_varName;
}


fay::FayStaticVarDef::FayStaticVarDef(const std::string &name, PTR(FayClass) clazz)
	: FayVar(name, clazz), value(MKPTR(FayObject)(clazz))
{
}

void fay::FayObject::init()
{
	//调用构造方法
	auto funs = this->_class->findFunByName(FUN_CREATE, false);
	for(auto fun : funs)
		FayVM::Run(fun, FayValue(this->shared_from_this()));
}

void fay::FayObject::setVar(const std::string &name, FayValue &value)
{
	auto varDef = this->_class->findVar(name);
	if(!varDef)
	{
		LOG_ERROR("Cannot find var : " << name << " in " << this->_class->fullname());
		return;
	}

	//TODO 这里最好加一个类型的Check
	this->_vars[varDef->indexValue()] = value;
}

void fay::TryHandler::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add("(")->add(this->start)->add(",")->add(this->end)->add(") => ")->add(this->target);
}

void fay::TypeDesc::buildString(mirror::utils::StringBuilder* sb)
{
	if(this->isAny)
		sb->add("any");
	else if(this->type == ValueType::Object && this->classType)
		this->classType->buildString(sb);
	else
		sb->add(TypeDict::ToName(this->type));
}
