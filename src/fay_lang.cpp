#include <fay_lang.h>
#include <mirror_utils_log.h>
#include <fay_internal_fun.h>
#include <fay_internal_object.h>

using namespace fay;

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

std::vector<PTR(FayFun)> fay::FayClass::matchFun(const std::string &funName, const std::vector<PTR(FayClass)> &paramsType, bool isStatic)
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

std::vector<PTR(FayFun)> fay::FayClass::findFunByName(const std::string &name)
{
	std::vector<PTR(FayFun)> r;
	this->_sft.findFunByName(name, r);
	this->_vft.findFunByName(name, r);
	return r;
}

pos_t fay::FayClass::getFunIndex(const std::string &fullname, bool isStatic)
{
	if(isStatic)
		return this->_sft.getFunIndex(fullname);
	else
		return this->_vft.getFunIndex(fullname);
}

const std::string &fay::FayClass::fullname()
{
	return this->_fullname;
}

void fay::FayClass::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add(this->fullname());
	sb->increaseIndent();

	sb->add("SFT : ")->endl();
	sb->increaseIndent();
	this->_sft.buildString(sb);
	sb->decreaseIndent();

	sb->add("VFT : ")->endl();
	sb->increaseIndent();
	this->_vft.buildString(sb);
	sb->decreaseIndent();

	sb->decreaseIndent();
}

pos_t fay::FayLib::addClass(PTR(FayClass) clazz)
{
	clazz->lib(this->shared_from_this());
	this->classes.push_back(clazz);

	auto domain = this->domain();
	if(domain)
		return domain->addType(clazz);

	return -1;
}

pos_t fay::FayLib::findOutsideFun(PTR(FayFun) fun)
{
	//查找当前是否已经有了
	std::string fullname = fun->clazz()->fullname() + "." + fun->fullname();
	pos_t index = this->_outsideFuns.findIndex(fullname);
	if(index >= 0) return index;

	//检查domain是否正常
	auto domain = this->domain();
	if(!domain)
	{
		LOG_ERROR("Cannot find domain");
		return -1;
	}

	pos_t classIndex;
	pos_t funIndex;
	domain->getFunIndex(fun, classIndex, funIndex);

	if(classIndex < 0)
	{
		LOG_ERROR("Cannot find class index : " << fullname);
		return -1;
	}

	if(funIndex < 0)
	{
		LOG_ERROR("Cannot find fun index : " << fullname);
		return -1;
	}

	//添加外部函数
	PTR(OutsideFun) ofun = MKPTR(OutsideFun)(
			fun->clazz()->fullname(), classIndex,
			fun->fullname(), funIndex);
	return this->_outsideFuns.add(fullname, ofun);
}

void fay::FayLib::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayLib]")->add(this->name)->endl();
	sb->increaseIndent();
	//显示外部函数
	sb->add("[OutSideFun]")->endl();
	sb->increaseIndent();
	for(auto i = 0; i < this->_outsideFuns.list().size(); ++i)
	{
		sb->add(i)->add(" : ");
		this->_outsideFuns.list()[i]->buildString(sb);
	}
	sb->decreaseIndent();
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
	for(auto i = 0; i < this->_insts.size(); ++i)
	{
		FayInst* inst = this->_insts[i];
		switch(inst->type())
		{
			case InstType::CallStatic:
			{
				//取出调用方法的索引值
				inst::CallStatic* inst1 = static_cast<inst::CallStatic*>(inst);
				PTR(OutsideFun) fun = this->clazz()->lib()->findOutsideFun(inst1->outsideFunIndex);
				if(fun)
				{
					inst1->typeIndex = fun->classIndex();
					inst1->funIndex = fun->funIndex();
				}
				else
				{
					inst1->typeIndex = -1;
					inst1->funIndex = -1;
				}
				break;
			}
			case InstType::New:
			{
				inst::New* inst1 = static_cast<inst::New*>(inst);
				auto clazz = this->domain()->findClass(inst1->className);
				if(clazz == NULL)
					throw FayLangException("Cannot find class : " + inst1->className);

				inst1->classIndex = this->domain()->getClassIndex(clazz);
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

void fay::FayInstFun::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayInstFun]")->add(this->fullname())->endl();

	sb->increaseIndent();

	for each(auto it in this->_params)
		it->buildString(sb);

	for(auto i = 0; i < this->_insts.size(); ++i)
	{
		auto it = this->_insts[i];
		sb->add(i)->add(" : ");
		if(it)
			it->buildString(sb);
		else
			sb->add("<nullptr>");
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

void fay::FayFun::addParam(std::vector<PTR(FayParamDef)> defs)
{
	for(auto it : defs)
		this->addParam(it);
}

void fay::FayFun::returnValue(PTR(FayClass) type)
{
	if(!this->_returnValue.expired())
		LOG_ERROR("Reset return value");

	this->_returnValue = type;
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

fay::FayDomain::FayDomain()
{
}

PTR(FayDomain) fay::FayDomain::init()
{
	//添加系统库。因为存在循环依赖的问题，要分两次进行初始化
	PTR(SystemLib) lib = MKPTR(SystemLib)(this->shared_from_this(), "system", 1, 1);
	lib->preInit();
	this->addLib(lib);
	lib->postInit();

	return this->shared_from_this();
}

void fay::FayDomain::addLib(PTR(FayLib) lib)
{
	//先生成相互的引用关系
	this->_libs.push_back(lib);

	//然后对Lib里的东西进行一下排序
	for each(auto it in lib->classes)
		this->_types.add(it->fullname(), it);
}

void fay::FayDomain::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add("[FayDomain]")->endl();
	sb->increaseIndent();
	for(auto i = 0; i < this->_types.list().size(); ++i)
	{
		auto t = this->_types.list()[i];
		sb->add(i)->add(" : ")->add(t->fullname())->endl();
	}

	for each(auto it in this->_libs)
		it->buildString(sb);
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

pos_t fay::FayDomain::getClassIndex(PTR(FayClass) t)
{
	return this->_types.findIndex(t->fullname());
}

PTR(FayClass) fay::FayDomain::findClass(const std::string &typeFullname)
{
	auto type = this->_types.find(typeFullname);
	return type;
}

PTR(FayClass) fay::FayDomain::findClass(pos_t index)
{
	auto type = this->_types.find(index);
	if(!type)
		LOG_ERROR("Cannot find type by index : " << index);

	return type;
}

PTR(FayClass) fay::FayDomain::findClass(ValueType type)
{
	return this->findClass(TypeDict::ToName(type));
}

std::vector<PTR(FayClass)> fay::FayDomain::findClass(std::vector<std::string> &imports, const std::string &typeName)
{
	std::vector<PTR(FayClass)> types;

	//直接根据名称查找
	auto type = this->findClass(typeName);
	if(type)
		types.push_back(type);

	//加上import的前缀查找
	for each(auto it in imports)
	{
		std::string typeFullname = it + "." + typeName;
		auto type = this->findClass(typeFullname);
		if(type)
			types.push_back(type);
	}

	return types;
}

PTR(FayFun) fay::FayDomain::findFun(const std::string &typeFullname, const std::string &funFullname, bool isStatic)
{
	auto type = this->findClass(typeFullname);
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

bool fay::FayDomain::getFunIndex(const std::string &typeFullname, const std::string &funFullname, bool isStatic, pos_t &typeIndex, pos_t &funIndex)
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

bool fay::FayDomain::getFunIndex(PTR(FayFun) fun, pos_t &typeIndex, pos_t &funIndex)
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

void fay::FayParamDef::buildString(mirror::utils::StringBuilder* sb)
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

PTR(FayClass) fay::FayLangUtils::WeightValueType(PTR(FayClass) t1, PTR(FayClass) t2)
{
	if(t1->valueType() >= ValueType::String || t2->valueType() >= ValueType::String)
		return (*t1->domain())[ValueType::String];

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
	}
	//ConvertInstEnd

	return nullptr;
}

FayInst* fay::FayLangUtils::PushDefault(ValueType type)
{
	switch(type)
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

void fay::OutsideFun::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add(this->_classFullname)->add(":")->add(this->_classIndex)->add(" ");
	sb->add(this->_funFullname)->add(":")->add(this->_funIndex)->endl();
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

std::vector<PTR(FayFun)> fay::FunTable::matchFun(const std::string &funName, const std::vector<PTR(FayClass)> &paramsType)
{
	std::vector<PTR(FayFun)> funs;

	for(auto i = 0; i < this->_funs.size(); ++i)
	{
		auto it = this->_funs[i];
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
	for(auto i = 0; i < this->_funs.size(); ++i)
	{
		if(this->_funs[i]->fullname() == fullname)
			return this->_funs[i];
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
	for(auto fun : this->_funs)
	{
		if(fun->name() == name)
			list.push_back(fun);
	}
}

void fay::FunTable::buildString(mirror::utils::StringBuilder* sb)
{
	for(auto i = 0; i < this->_funs.size(); ++i)
	{
		sb->add(i)->add(" : ");
		this->_funs[i]->buildString(sb);
	}
}

const std::string &fay::FayVarDef::fullname()
{
	if(!this->_fullname.size())
		this->_fullname = this->_name + ":" + this->_type.lock()->fullname();

	return this->_fullname;
}

void fay::FayVarDef::buildString(mirror::utils::StringBuilder* sb)
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

fay::ValueType fay::FayInstClass::valueType()
{
	return ValueType::Object;
}
