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

std::string fay::FayClass::makeFullname()
{
	return TOSTR((!this->package.empty() ? this->package + "." : "") << this->name);
}

void fay::FayClass::buildVFT()
{
	if(!this->_isRebuild)
	{
		this->_isRebuild = true;

		if(!this->superClass.expired())
		{
			auto superClass = this->superClass.lock();
			superClass->buildVFT();
			this->_vft.rebuild(&superClass->_vft);
		}
	}
}

bool fay::FayClass::match(PTR(FayClass) type)
{
	//TODO ：对接口的处理

	if(type && type.get() == this)
		return true;

	PTR(FayClass) parent = this->superClass.lock();
	if(parent && parent->match(type))
		return true;

	return false;
}

PTR(FayStaticVarDef) fay::FayClass::addStaticVar(const std::string &name, PTR(FayClass) classType)
{
	PTR(FayStaticVarDef) var = MKPTR(FayStaticVarDef)(name, classType);
	this->_staticVarDefs.add(var);
	return var;
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

PTR(FayVarDef) fay::FayClass::addVar(const std::string &name, PTR(FayClass) type)
{
	PTR(FayVarDef) field = MKPTR(FayVarDef)(name, type);
	this->_varDefs.add(field);
	return field;
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

		if(!this->superClass.expired())
			return this->superClass.lock()->canCovertTo(type);
	}

	return false;
}

void fay::FayClass::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add("[Class] ")->add(this->fullname());
	if(!this->superClass.expired())
		sb->add(" : ")->add(this->superClass.lock()->fullname());
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

void fay::FayLib::addClass(PTR(FayClass) clazz)
{
	clazz->onAddToLib(this->shared_from_this());
	this->classes.add(clazz);
}

PTR(FayClass) fay::FayLib::findClass(const std::string &fullname, bool inDeps)
{
	PTR(FayClass) clazz = this->classes.find(fullname);
	if(!clazz && inDeps && this->deps)
		clazz = this->deps->findClass(fullname);

	return clazz;
}

PTR(FayClass) fay::FayLib::findClass(ValueType type, bool inDeps)
{
	if(type >= ValueType::Object)
		return nullptr;

	for(const auto &it : this->classes.list())
	{
		if(it->valueType() == type)
			return it;
	}

	if(inDeps && this->deps)
		return this->deps->findClass(type);

	return nullptr;
}

LIST(PTR(FayClass)) fay::FayLib::findClassWithName(const std::string &name, bool inDeps)
{
	LIST(PTR(FayClass)) list;

	for(auto it : this->classes.list())
	{
		if(it->name == name)
			list.push_back(it);
	}

	if(inDeps && this->deps)
	{
		auto list2 = this->deps->findClassWithName(name);
		for(auto it : list2)
			list.push_back(it);
	}

	return list;
}

PTR(std::vector<PTR(FayFun)>) fay::FayLib::findMainFun()
{
	PTR(std::vector<PTR(FayFun)>) funs = MKPTR(std::vector<PTR(FayFun)>)();

	for(const auto &clazz : this->classes.list())
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
	for each(auto it in this->classes.list())
		it->buildString(sb);
	sb->decreaseIndent();
	sb->decreaseIndent();
}

const std::string fay::FayLib::indexKey()
{
	return this->_indexKey;
}

void fay::FayInstFun::prepareInsts()
{
	PTR(FayClass) clazz = this->clazz.lock();
	PTR(FayLib) lib = clazz->lib.lock();

	for(FayInst* inst : this->_insts)
	{
		switch(inst->type())
		{
			case InstType::CallStatic:
			{
				//取出调用方法的索引值
				auto* cmd = static_cast<inst::CallStatic*>(inst);
				auto clazz = lib->findClass(cmd->className, true);
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

				cmd->libIndex = clazz->lib.lock()->indexValue();
				cmd->classIndex = clazz->indexValue();
				cmd->funIndex = fun->indexValue();
				break;
			}
			case InstType::CallVirtual:
			{
				//取出调用方法的索引值
				auto* cmd = static_cast<inst::CallVirtual*>(inst);
				auto clazz = lib->findClass(cmd->className, true);
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

				cmd->libIndex = clazz->lib.lock()->indexValue();
				cmd->classIndex = clazz->indexValue();
				cmd->funIndex = fun->indexValue();
				break;
			}
			case InstType::SetStatic:
			{
				auto* cmd = static_cast<inst::SetStatic*>(inst);
				auto clazz = lib->findClass(cmd->className, true);
				if(!clazz)
				{
					LOG_ERROR("Cannot find class : " << cmd->className);
					break;
				}
				cmd->libIndex = clazz->lib.lock()->indexValue();
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
				auto clazz = lib->findClass(cmd->className, true);
				if(!clazz)
				{
					LOG_ERROR("Cannot find class : " << cmd->className);
					break;
				}
				cmd->libIndex = clazz->lib.lock()->indexValue();
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
				auto clazz = lib->findClass(cmd->className, true);
				if(clazz == nullptr)
					throw FayLangException("Cannot find class : " + cmd->className);

				cmd->libIndex = clazz->lib.lock()->indexValue();
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
		if(it->classType.lock() != type)
		{
			LOG_ERROR("Same var name, diff type : " << it->classType.lock()->fullname() << ", " << type->fullname());
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

	if(!this->params.empty())
	{
		sb->add("[Params] ")->endl();
		sb->increaseIndent();
		for(auto i = 0; i < this->params.size(); ++i)
		{
			sb->add(i)->add("> ");
			this->params[i]->buildString(sb);
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
			sb->add(i)->add("> ")->add(it->name)->add(":")->add(it->classType.lock()->fullname())->endl();
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
	: name(name), type(type), isStatic(isStatic), accessType(accessType), params(params), returnValue(returnValue)
{
}

bool fay::FayFun::match(const std::vector<PTR(FayClass)> &paramsType)
{
	//参数不一致
	if(paramsType.size() != this->params.size())
		return false;

	for(auto i = 0; i < this->params.size(); ++i)
	{
		PTR(FayParamDef) p = this->params[i];
		if(!p->type.lock()->match(paramsType[i]))
			return false;
	}

	return true;
}

std::string fay::FayFun::makeFullname()
{
	std::string str;
	for each(auto it in this->params)
	{
		if(str.size() > 0)
			str += ",";
		str += it->fullname();
	}

	str = this->name + "(" + str + "):";

	if(returnValue.expired())
		str += "void";
	else
		str += returnValue.lock()->fullname();

	return str;
}

void fay::FayFun::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayFun]")->add(this->fullname())->endl();
}

pos_t fay::FayClass::addFun(PTR(FayFun) fun)
{
	fun->onAddToClass(this->shared_from_this());

	if(fun->isStatic)
		return this->_sft.addFun(fun);
	else
		return this->_vft.addFun(fun);
}

IndexMap<FayLib> fay::FayDomain::Libs;

void fay::FayDomain::AddLib(PTR(FayLib) lib)
{
	lib->onAddToDomain();
	Libs.add(lib);
}

PTR(FayLib) fay::FayDomain::FindLib(const std::string &name)
{
	PTR(FayLib) lib;

	for(const auto &it : FayDomain::Libs.list())
	{
		if(it->name == name)
		{
			if(lib)
			{
				if(it->ver.marjor > lib->ver.marjor || (it->ver.marjor == lib->ver.marjor && it->ver.minjor > lib->ver.minjor))
					lib = it;
			}
			else
				lib = it;
		}
	}

	return lib;
}

PTR(FayClass) fay::FayDomain::UseClass(pos_t libIndex, pos_t classIndex)
{
	auto lib = Libs.find(libIndex);
	if(lib)
	{
		auto clazz = lib->classes.find(classIndex);
		if(clazz)
			return clazz;
	}

	std::string msg = TOSTR("Unknow Class : " << libIndex << "," << classIndex);
	throw std::exception(msg.c_str());
}

void fay::FayDomain::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayDomain]")->endl();
	for each(auto it in FayDomain::Libs.list())
		it->buildString(sb);
	sb->decreaseIndent();
}

std::string fay::FayDomain::ToString()
{
	utils::StringBuilder sb;
	FayDomain::buildString(&sb);
	return sb.toString();
}

//TODO Delete
//PTR(FayClass) fay::FayDomain::FindClass(const std::string &fullname)
//{
//	//auto type = FayDomain::_classes.find(fullname);
//	//return type;
//	return nullptr;
//}

PTR(FayClass) fay::FayDomain::FindClass(pos_t libIndex, pos_t lassIndex)
{
	PTR(FayLib) lib = FayDomain::Libs.find(libIndex);
	if(lib != nullptr)
	{
		PTR(FayClass) type = lib->classes.find(lassIndex);
		return type;
	}

	return nullptr;
}

//PTR(FayClass) fay::FayDomain::FindClass(ValueType type)
//{
//	return FayDomain::FindClass(TypeDict::ToName(type));
//}
//
//std::vector<PTR(FayClass)> fay::FayDomain::FindClass(std::vector<std::string> &imports, const std::string &typeName)
//{
//	std::vector<PTR(FayClass)> types;
//
//	//直接根据名称查找
//	auto type = FayDomain::FindClass(typeName);
//	if(type)
//		types.push_back(type);
//
//	//加上import的前缀查找
//	for each(auto it in imports)
//	{
//		std::string typeFullname = it + "." + typeName;
//		auto type = FayDomain::FindClass(typeFullname);
//		if(type)
//			types.push_back(type);
//	}
//
//	return types;
//}

PTR(FayLib) fay::FayDomain::FindLib(const std::string &name, int marjor)
{
	return FayDomain::FindLib(name, marjor, 0);
}

PTR(FayLib) fay::FayDomain::FindLib(const std::string &name, int marjor, int minjor)
{
	PTR(FayLib) lib;

	for(const auto &it : FayDomain::Libs.list())
	{
		if(it->name == name && it->ver.marjor == marjor)
		{
			if(it->ver.minjor == minjor)
			{
				lib = it;
				break;
			}

			if(lib == nullptr || it->ver.minjor > lib->ver.minjor)
				lib = it;
		}
	}

	return lib;
}

PTR(FayLibSet) fay::FayDomain::AllLibs()
{
	auto libs = MKPTR(FayLibSet)();

	for(const auto &it : FayDomain::Libs.list())
		libs->addLib(it);

	return libs;
}

std::string fay::FayParamDef::makeFullname()
{
	return TOSTR(this->name << ":" << (this->type.expired() ? "?" : this->type.lock()->fullname()));
}

void fay::FayParamDef::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add(this->fullname())->endl();
}

std::string fay::FayLangUtils::ClassFullname(const std::string &package, const std::string &name)
{
	std::string fullname = TOSTR(package << "." << name);
	return fullname;
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

PTR(FayClass) fay::FayLangUtils::WeightValueType(PTR(FayLib) deps, PTR(FayClass) t1, PTR(FayClass) t2)
{
	if(t1->valueType() >= ValueType::String || t2->valueType() >= ValueType::String)
		return deps->findClass(ValueType::String, true);

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
	this->classIndex = fun->clazz.lock()->indexValue();
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
		if(it->name == funName && it->match(paramsType))
			funs.push_back(it);
	}

	return funs;
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
		if(fun->name == name)
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

void fay::FayVar::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add(this->fullname());
}

std::string fay::FayVar::makeFullname()
{
	return TOSTR(this->name << ":" << this->classType.lock()->fullname());
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

fay::StaticVarRef::StaticVarRef(const std::string &fullname, PTR(FayClass) clazz, PTR(FayStaticVarDef) var)
	: _fullname(fullname)
{
	this->_className = clazz->fullname();
	this->_classIndex = clazz->indexValue();
	this->_varName = var->name;
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
	auto funs = this->clazz->findFunByName(FUN_CREATE, false);
	for(auto fun : funs)
		FayVM::Run(fun, FayValue(this->shared_from_this()));
}

void fay::FayObject::setVar(const std::string &name, FayValue &value)
{
	auto varDef = this->clazz->findVar(name);
	if(!varDef)
	{
		LOG_ERROR("Cannot find var : " << name << " in " << this->clazz->fullname());
		return;
	}

	//TODO 这里最好加一个类型的Check
	this->vars[varDef->indexValue()] = value;
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

void fay::FayLibSet::addLib(PTR(FayLib) lib)
{
	auto i = 0;
	for(; i < this->libs.size(); ++i)
	{
		auto it = this->libs[i];

		//如果已经有了，返回
		if(it == lib)
			return;

		if(it->name == lib->name)
		{
			//如果新的比旧的新
			if(lib->ver.marjor > it->ver.marjor || (lib->ver.marjor == it->ver.marjor && lib->ver.minjor > it->ver.minjor))
			{
				this->libs[i] = lib;
				break;
			}
		}
	}

	//如果之前没有，加到最后
	if(i >= this->libs.size())
		this->libs.push_back(lib);
}

void fay::FayLibSet::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add("[LibSet]");
	sb->increaseIndent();

	for(auto it : this->libs)
		sb->add(it->indexKey());

	sb->decreaseIndent();
}

PTR(FayClass) fay::FayLibSet::findClass(const std::string &fullname)
{
	for(auto lib : this->libs)
	{
		auto clazz = lib->findClass(fullname, false);
		if(clazz)
			return clazz;
	}

	return nullptr;
}

PTR(FayClass) fay::FayLibSet::findClass(ValueType type)
{
	for(const auto &it : this->libs)
	{
		auto r = it->findClass(type, false);
		if(r) return r;
	}

	return nullptr;
}

LIST(PTR(FayClass)) fay::FayLibSet::findClassWithName(const std::string &name)
{
	LIST(PTR(FayClass)) list;

	for(auto lib : this->libs)
	{
		auto cs = lib->findClassWithName(name, false);
		if(cs.size() > 0)
		{
			for(auto const &c : cs)
				list.push_back(c);
		}
	}

	return list;
}

void fay::FayInstClass::onStaticInit()
{
	//执行初始化方法
	auto funs = this->findFunByName(FUN_STATIC_INIT, true);
	for(const auto &fun : funs)
		FayVM::Run(fun);
}
