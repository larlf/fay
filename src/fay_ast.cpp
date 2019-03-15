#include "fay_ast.h"
#include "fay_ast.h"
#include "fay_ast.h"
#include <fay_ast.h>
#include <typeinfo>
#include <mirror_utils_log.h>

using namespace fay;

std::string fay::AstNode::className()
{
	std::string name = typeid(*this).name();
	if(name.find_first_of("class ") == 0)
		name = name.substr(6);
	return name;
}

void fay::AstNode::addChildNode(PTR(AstNode) node)
{
	//可能会添加空节点用于占位
	if(node != nullptr)
		node->_parent = this->shared_from_this();
	this->_nodes.push_back(node);
}

void fay::AstNode::insertChldNode(size_t index, PTR(AstNode) node)
{
	PTR(AstNode) oldNode;
	if(index >= 0 && index < this->_nodes.size())
	{
		oldNode = this->_nodes[index];
		this->_nodes[index] = node;
	}
	else
		this->_nodes.push_back(node);

	node->_parent = this->shared_from_this();

	if(oldNode)
		node->addChildNode(oldNode);
}

ValueType fay::AstNode::valueType()
{
	PTR(FayClass) clazz = this->classType();
	if(clazz)
		return clazz->valueType();

	return ValueType::Void;
}

void fay::AstNode::buildString(mirror::utils::StringBuilder* sb)
{
	sb->add("[" + this->className() + "]")
	->add(" \"")->add(this->_text)->add("\"");

	auto type = this->valueType();
	if(type != ValueType::Void)
	{
		sb->add(" ")->add(TypeDict::ToName(type));
		if(type == ValueType::Object)
			sb->add(this->_classType.expired() ? "=?" : ("=" + this->_classType.lock()->fullname()));
	}

	sb->increaseIndent();
	for each(auto it in this->_nodes)
	{
		sb->endl();
		if(it)
			it->buildString(sb);
		else
			sb->add("<null>");
	}
	sb->decreaseIndent();
}

void fay::AstNode::dig1(FayBuilder* builder)
{
	for each(auto it in this->_nodes)
		if(it)
			it->dig1(builder);
}

void fay::AstNode::dig2(FayBuilder* builder)
{
	for each(auto it in this->_nodes)
		if(it)
			it->dig2(builder);
}

void fay::AstNode::dig3(FayBuilder* builder)
{
	for each(auto it in this->_nodes)
		if(it)
			it->dig3(builder);
}

void fay::AstNode::dig4(FayBuilder* builder)
{
	for each(auto it in this->_nodes)
		if(it)
			it->dig4(builder);
}

void fay::AstClass::dig1(FayBuilder* builder)
{
	this->_clazz = MKPTR(FayInstClass)(builder->lib(), builder->package(), this->_text);

	//检查是否重复
	std::string fullname = this->_clazz->fullname();
	if(builder->deps()->findClass(fullname) != nullptr)
		this->throwError(builder->file(), this->token(), I18n::Err_RepeatedClass, fullname);

	//添加到domain
	builder->addClass(this->_clazz);

	AstNode::dig1(builder);
}

void fay::AstClass::dig2(FayBuilder* builder)
{
	builder->bindClass(this->_clazz);

	//处理父类
	if(!this->superClassText.empty())
	{
		auto types = builder->deps()->findClassWithName(this->superClassText);
		if(types.empty())
			this->throwError(builder->file(), this->token(), I18n::Err_NoClass, this->superClassText);
		if(types.size() > 1)
			this->throwError(builder->file(), this->token(), I18n::Err_RepeatedClass, this->superClassText);
		builder->clazz()->superClass(types[0]);
	}
	else
	{
		//默认用Object当父类
		builder->clazz()->superClass(builder->deps()->findClass("fay.system.Object"));
	}

	//添加特殊函数

	this->_initFun = MKPTR(FayInstFun)(FUN_STATIC_INIT, true, FunAccessType::Private, nullptr);
	builder->addFun(this->_initFun);

	this->_createFun = MKPTR(FayInstFun)(FUN_CREATE, false, FunAccessType::Private, nullptr);
	this->_createFun->addVar("this", builder->clazz());
	builder->addFun(this->_createFun);

	//fun = MKPTR(FayInstFun)(FUN_DESTORY, false, FunAccessType::Private, nullptr);
	//fun->addVar("this", builder->clazz());
	//this->destoryFunIndex = builder->addFun(fun);

	AstNode::dig2(builder);
}

void fay::AstClass::dig3(FayBuilder* builder)
{
	builder->bindClass(this->_clazz);
	AstNode::dig3(builder);
}

void fay::AstClass::dig4(FayBuilder* builder)
{
	builder->bindClass(this->_clazz);

	//先处理静态字段
	builder->bindFun(this->_initFun);
	for each(auto it in this->_nodes)
		if(it && it->is<AstField>() && TOPTR(AstField, it)->isStatic())
			it->dig4(builder);
	builder->optimizeInsts();

	//处理非静态字段
	builder->bindFun(this->_createFun);
	for each(auto it in this->_nodes)
		if(it && it->is<AstField>() && !TOPTR(AstField, it)->isStatic())
			it->dig4(builder);
	builder->optimizeInsts();

	//再处理其它的内容
	for each(auto it in this->_nodes)
		if(it && !it->is<AstField>())
			it->dig4(builder);
}

void fay::AstFun::dig2(FayBuilder* builder)
{
	builder->params.clear();

	AstNode::dig2(builder);

	FunAccessType _accessType = FunAccessType::Public;
	bool isStatic = false;
	for(const auto &it : this->_descWords)
	{
		if(it == "static")
			isStatic = true;
		else if(it == "private")
			_accessType = FunAccessType::Private;
		else if(it == "protected")
			_accessType = FunAccessType::Protected;
	}

	//返回值
	if(this->_nodes[1])
		this->_classType = this->_nodes[1]->classType();
	else
		this->_classType.reset();

	//创建这个函数定义
	this->_fun = MKPTR(FayInstFun)(this->_text, isStatic, _accessType, builder->params, this->_classType.lock());

	//添加初始化的变量
	if(!isStatic)
		this->_fun->addVar("this", builder->clazz());

	//添加参数变量
	for(const auto &param : builder->params)
		this->_fun->addVar(param->name(), param->type());

	builder->addFun(this->_fun);
}

void fay::AstFun::dig3(FayBuilder* builder)
{
	builder->bindFun(this->_fun);
	AstNode::dig3(builder);
}

void fay::AstFun::dig4(FayBuilder* builder)
{
	builder->bindFun(this->_fun);
	AstNode::dig4(builder);
	builder->optimizeInsts();
}

void fay::AstFile::dig1(FayBuilder* builder)
{
	builder->bindFile(this->_file);
	AstNode::dig1(builder);
	builder->unbindFile();
}

void fay::AstFile::dig2(FayBuilder* builder)
{
	builder->bindFile(this->_file);
	AstNode::dig2(builder);
	builder->unbindFile();
}

void fay::AstFile::dig3(FayBuilder* builder)
{
	builder->bindFile(this->_file);
	AstNode::dig3(builder);
	builder->unbindFile();
}

void fay::AstFile::dig4(FayBuilder* builder)
{
	builder->bindFile(this->_file);
	AstNode::dig4(builder);
	builder->unbindFile();
}

void fay::AstString::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	for(auto i = 0; i < this->_text.size(); ++i)
	{
		char c = this->_text[i];

		//字符串开始标志
		if(i == 0 && c == '\"')
			continue;

		//字符串结束标志
		if(i == this->_text.size() - 1 && c == '\"')
			continue;

		//TODO： 处理转义字符

		this->_value.push_back(c);
	}

	this->_classType = builder->deps()->findClass(ValueType::String);
}

void fay::AstString::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	auto* inst = new inst::PushString(this->_value);
	builder->addInst(inst);
}

PTR(FayClass) fay::AstParamDefine::getType(FayBuilder* builder)
{
	auto n1 = this->childNode<AstType>(0);
	if(n1)
		return n1->classType();

	return nullptr;
}

void fay::AstParamDefine::dig2(FayBuilder* builder)
{
	this->_classType = builder->deps()->findClass(this->_nodes[0]->text());
	if(this->_classType.expired())
		this->throwError(builder->file(), this->token(), I18n::Err_NoClass, this->_nodes[0]->text());

	PTR(FayParamDef) def = MKPTR(FayParamDef)(this->text(), this->_classType.lock());
	builder->params.push_back(def);

	AstNode::dig2(builder);
}

void fay::AstPackage::dig1(FayBuilder* builder)
{
	builder->package(this->_text);
	builder->addUsing(this->_text);
}

void fay::AstPackage::dig2(FayBuilder* builder)
{
	builder->addUsing(this->_text);
}

void fay::AstPackage::dig3(FayBuilder* builder)
{
	builder->addUsing(this->_text);
}

void fay::AstPackage::dig4(FayBuilder* builder)
{
	builder->addUsing(this->_text);
}

void fay::AstCall::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	//取得参数的长度和类型
	PTR(AstParams) paramsNode = this->childNode<AstParams>(0);
	size_t paramSize = paramsNode->size();
	std::vector<PTR(FayClass)> paramsType = paramsNode->paramsType(builder);

	std::string className;
	std::string funName;

	//判断是内部函数还是外部函数
	size_t p = this->_text.find_last_of('.');
	if(p != std::string::npos)
	{
		className = this->_text.substr(0, p);
		funName = this->_text.substr(p + 1);

		//内部函数
		if(className == "this")
			className = "";
	}
	else
	{
		//内部函数
		funName = this->_text;
	}

	if(!className.empty())
	{
		PTR(FayFun) fun;
		auto varDef = builder->fun()->findVar(className);
		if(varDef)
		{
			this->varIndex = varDef->indexValue();

			auto clazz = varDef->classType();
			if(!clazz)
				this->throwError(builder->file(), this->token(), I18n::Err_NoClass, className);

			auto funs = clazz->findFun(funName, paramsType, false);
			if(funs.empty())
				this->throwError(builder->file(), this->token(), I18n::Err_NoFun, clazz->fullname(), funName);

			fun = funs[0];
		}
		else
		{
			auto classes = builder->deps()->findClassWithName(className);
			if(classes.empty())
				this->throwError(builder->file(), this->token(), I18n::Err_NoClass, className);
			else if(classes.size() > 1)
				this->throwError(builder->file(), this->token(), I18n::Err_RepeatedClass, className);

			auto funs = classes[0]->findFun(funName, paramsType, true);
			if(funs.empty())
				this->throwError(builder->file(), this->token(), I18n::Err_NoClass, className, funName);
			else if(funs.size() > 1)
				this->throwError(builder->file(), this->token(), I18n::Err_RepeatedClass, className, funName);

			fun = funs[0];
		}

		this->_fun = fun;

		//以第一个参数的返回值为准
		if(fun->returnValue())
			this->_classType = fun->returnValue();
	}
	else
	{
		//TODO ：内部函数调用的实现
	}
}

void fay::AstCall::dig4(FayBuilder* builder)
{
	PTR(FayFun) fun = this->_fun.lock();
	if(!fun)
		this->throwError(builder->file(), this->token(), I18n::Err_NoFun, this->_text);

	if(fun->isStatic())
	{
		AstNode::dig4(builder);
		builder->addInst(new inst::CallStatic(fun->clazz()->fullname(), fun->fullname()));
	}
	else
	{
		builder->addInst(new inst::LoadLocal(this->varIndex));
		AstNode::dig4(builder);
		builder->addInst(new inst::CallVirtual(fun->clazz()->fullname(), fun->fullname()));
	}
}

//PTR(FayClass) fay::AstCall::classType(FayBuilder * builder)
//{
//	PTR(FayFun) fun = this->_fun.lock();
//	if (!fun)
//		return nullptr;
//
//	if (fun->returnsCount() <= 0)
//		return nullptr;
//
//	PTR(FayClass) type = fun->returns()[0].lock();
//	if (!type)
//		return nullptr;
//
//	return type;
//}

std::vector<PTR(FayClass)> fay::AstParamDefineList::getTypeList(FayBuilder* builder)
{
	std::vector<PTR(FayClass)> list;

	for(auto i = 0; i < this->childNodesNum(); ++i)
	{
		auto n = this->childNode<AstParamDefine>(i);
		if(!n)
			this->throwError(builder->file(), this->token(), I18n::Err_Expect, "AstParamDefine");

		list.push_back(n->getType(builder));
	}

	return list;
}

void fay::AstVarItem::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	//需要进行类型推断
	if(this->_nodes[0] == nullptr)
	{
		if(this->_nodes[1] == nullptr)
			this->throwError(builder->file(), this->token(), I18n::Err_UnknowType, "");

		this->_classType = this->_nodes[1]->classType();
	}
	else
	{
		//跟据定义查找类型
		this->_classType = this->_nodes[0]->classType();
	}

	//检查类型定义是否正确
	if(this->_classType.expired())
		this->throwError(builder->file(), this->_nodes[0]->token(), I18n::Err_UnknowType, this->_nodes[0]->text());

	//新添加变量
	std::string varName = this->text();
	this->_index = builder->fun()->addVar(varName, this->_classType.lock());

	//如果有赋值，检查是否要进行类型转换
	if(this->_nodes.size() > 1 && this->_nodes[1])
	{
		auto leftType = this->_classType.lock();
		auto rightType = this->_nodes[1]->classType();
		if(leftType != rightType)
			this->insertChldNode(1, MKPTR(AstTypeConvert)(this->_token, rightType, leftType));
	}
}

void fay::AstVarItem::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	if(this->_nodes[1] != nullptr)   //如果给定了值
	{
		//存入本地变量
		builder->addInst(new inst::SetLocal(this->_index));
	}
	else if(this->classType()->valueType() != ValueType::Object)  //非对象设置一个默认值
	{
		//添加默认值
		if(this->_nodes[1] == nullptr)
		{
			FayInst* inst = FayLangUtils::PushDefault(this->_nodes[0]->classType());
			if(inst == nullptr)
				this->throwError(builder->file(), this->token(), I18n::Err_NoDefaultValue, this->_nodes[0]->classType()->fullname());
			builder->addInst(inst);
		}

		//存入本地变量
		builder->addInst(new inst::SetLocal(this->_index));
	}
}

void fay::AstNumber::dig4(FayBuilder* builder)
{
	FayInst* inst = nullptr;

	switch(this->_val.type())
	{
		case ValueType::Int:
			builder->addInst(new inst::PushInt(this->_val.intVal()));
			break;
		case ValueType::Long:
			builder->addInst(new inst::PushLong(this->_val.longVal()));
			break;
		case ValueType::Float:
			builder->addInst(new inst::PushFloat(this->_val.floatVal()));
			break;
		case ValueType::Double:
			builder->addInst(new inst::PushDouble(this->_val.doubleVal()));
			break;
		default:
			LOG_ERROR("Unknow value type : " << (int)this->_val.type());
			break;
	}
}

//PTR(FayClass) fay::AstNumber::classType(FayBuilder * builder)
//{
//	return (*FayDomain)[this->_val.type()];
//}

fay::AstNumber::AstNumber(const PTR(Token)& token, const std::string &text)
	: AstNode::AstNode(token)
{
	this->_text = text;

	if(this->_text.find('.') != std::string::npos)
	{
		if(this->_text[this->_text.size() - 1] == 'D' || this->_text[this->_text.size() - 1] == 'd')
		{
			double v = std::stod(this->_text);
			this->_val = FayValue(v);
		}
		else
		{
			float v = std::stof(this->_text);
			this->_val = FayValue(v);
		}
	}
	else
	{
		if(this->_text[this->_text.size() - 1] == 'L' || this->_text[this->_text.size() - 1] == 'l')
		{
			int64_t v = std::stoll(this->_text);
			this->_val = FayValue(v);
		}
		else
		{
			int32_t v = std::stoi(this->_text);
			this->_val = FayValue(v);
		}
	}
}

void fay::AstNumber::dig3(FayBuilder* builder)
{
	this->_classType = builder->deps()->findClass(this->_val.type());

	AstNode::dig3(builder);
}

std::vector<PTR(FayClass)> fay::AstParams::paramsType(FayBuilder* builder)
{
	std::vector<PTR(FayClass)> ts;
	for each(auto it in this->_nodes)
		ts.push_back(it->classType());

	return ts;
}

void fay::AstID::dig3(FayBuilder* builder)
{
	//先在函数中找
	if(builder->fun())
	{
		auto var = builder->fun()->findVar(this->_text);
		if(var)
		{
			this->_type = VarType::Local;
			this->_varIndex = var->indexValue();
			this->_classType = var->classType();
			return;
		}
	}

	//如果中有.，就应该是成员变量
	auto pos = this->_text.find_last_of(".");
	if(pos != std::string::npos)
	{
		std::string className = this->_text.substr(0, pos);
		std::string varName = this->_text.substr(pos + 1);

		if(className == "err")
		{
			PTR(FayClass) errType = builder->deps()->findClass("fay.system.Error");
			PTR(FayVarDef) varDef = errType->findVar(varName);
			if(!varDef)
				this->throwError(builder->file(), this->token(), I18n::Err_NoField, className, varName);

			this->_type = VarType::Error;
			this->_classIndex = -1;
			this->_varIndex = varDef->indexValue();
			this->_classType = varDef->classType();
		}
		else
		{
			auto localVar = builder->fun()->findVar(className);
			if(localVar)
			{
				auto varDef = localVar->classType()->findVar(varName);
				if(!varDef)
					this->throwError(builder->file(), this->token(), I18n::Err_NoField, className, varName);

				this->_type = VarType::Field;
				this->_classIndex = localVar->indexValue();
				this->_varIndex = varDef->indexValue();
				this->_classType = varDef->classType();
			}
			else
			{
				auto classes = builder->deps()->findClassWithName(className);
				if(classes.empty())
					this->throwError(builder->file(), this->token(), I18n::Err_NoClass, className);
				if(classes.size() > 1)
					this->throwError(builder->file(), this->token(), I18n::Err_RepeatedClass, className);

				PTR(FayStaticVarDef) var = classes[0]->findStaticVar(varName);
				if(var == nullptr)
					this->throwError(builder->file(), this->token(), I18n::Err_NoStaticField, className, varName);

				this->_type = VarType::Static;
				this->_classIndex = classes[0]->indexValue();
				this->_varIndex = var->indexValue();
				this->_classType = var->classType();
			}
		}
	}
}

void fay::AstID::dig4(FayBuilder* builder)
{
	//不同模式下的操作不一样
	if(builder->exprMode == BuildExprMode::LeftValue)
		builder->addInst(new inst::CopyLocal(this->_varIndex));
	else
	{

		switch(this->_type)
		{
			case VarType::Static:
			{
				PTR(FayClass) clazz = FayDomain::FindClass(this->_libIndex, this->_classIndex);
				PTR(FayStaticVarDef) var = clazz->findStaticVar(this->_varIndex);
				builder->addInst(new inst::LoadStatic(clazz->fullname(), var->name()));
				break;
			}
			case VarType::Field:
			{
				builder->addInst(new inst::LoadLocal(this->_classIndex));
				builder->addInst(new inst::LoadField(this->_varIndex));
				break;
			}
			case VarType::Local:
				builder->addInst(new inst::LoadLocal(this->_varIndex));
				break;
			case VarType::Error:
				builder->addInst(new inst::LoadError());
				builder->addInst(new inst::LoadField(this->_varIndex));
				break;
			default:
				this->throwError(builder->file(), this->token(), I18n::Err_UnknowType, TypeDict::ToName(this->_type));
				break;
		}
	}
}

void fay::AstLeftRightOP::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	if(this->_nodes[0] == nullptr)
		this->throwError(builder, I18n::Err_NoLeftValue);
	if(this->_nodes[1] == nullptr)
		this->throwError(builder, I18n::Err_NoRightValue);

	//先算出目标类型
	auto t1 = this->_nodes[0]->classType();
	auto t2 = this->_nodes[1]->classType();
	auto t3 = FayLangUtils::WeightValueType(builder->deps(), t1, t2);
	this->_classType = t3;

	//如果和目标类型不一致，就转换一下
	if(t1 != t3)
		this->insertChldNode(0, MKPTR(AstTypeConvert)(this->_token, t1, t3));
	if(t2 != t3)
		this->insertChldNode(1, MKPTR(AstTypeConvert)(this->_token, t2, t3));
}

void fay::AstLeftRightOP::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	FayInst* inst = nullptr;

	//生成操作代码
	if(this->_text == "+")
		inst = FayLangUtils::OPInst(InstGroupType::Add, this->_classType.lock()->valueType());
	else if(this->_text == "-")
		inst = FayLangUtils::OPInst(InstGroupType::Sub, this->_classType.lock()->valueType());
	else if(this->_text == "*")
		inst = FayLangUtils::OPInst(InstGroupType::Mul, this->_classType.lock()->valueType());
	else if(this->_text == "/")
		inst = FayLangUtils::OPInst(InstGroupType::Div, this->_classType.lock()->valueType());
	else if(this->_text == "%")
		inst = FayLangUtils::OPInst(InstGroupType::Mod, this->valueType());
	else if(this->_text == "<<")
		inst = FayLangUtils::OPInst(InstGroupType::LShift, this->valueType());
	else if(this->_text == ">>")
		inst = FayLangUtils::OPInst(InstGroupType::RShift, this->valueType());

	if(inst)
		builder->addInst(inst);
	else
		this->throwError(builder->file(), this->token(), I18n::Err_NotSupportOP, this->_text, this->_classType.lock()->fullname());
}

fay::AstTypeConvert::AstTypeConvert(const PTR(Token)& token, PTR(FayClass) srcType, PTR(FayClass) destType)
	: AstNode(token), _srcType(srcType), _dstType(destType)
{
	this->_classType = this->_dstType;
}

void fay::AstTypeConvert::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	PTR(FayClass) srcType = this->_srcType.lock();
	if(!srcType)
		this->throwError(builder->file(), this->token(), I18n::Err_CannotConvertFromVoid);

	PTR(FayClass) dstType = this->_dstType.lock();
	if(!dstType)
		this->throwError(builder->file(), this->token(), I18n::Err_CannotConvertToVoid);

	if(srcType->valueType() == ValueType::Object && dstType->valueType() == ValueType::Object)
	{
		//如果不能转换，就提示错误，能转换的话，不进行任何处理
		if(!srcType->canCovertTo(dstType))
			this->throwError(builder->file(), this->token(), I18n::Err_CannotConvert, srcType->fullname(), dstType->fullname());
	}
	else
	{
		FayInst* inst = FayLangUtils::ConvertInst(srcType->valueType(), dstType->valueType());
		if(inst == nullptr)
			this->throwError(builder->file(), this->token(), I18n::Err_CannotConvert, srcType->fullname(), dstType->fullname());

		builder->addInst(inst);
	}
}

void fay::AstBool::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	if(this->_text == "true")
		this->_value = true;
	else
		this->_value = false;

	this->_classType = builder->deps()->findClass(ValueType::Bool);
}

void fay::AstBool::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);
	builder->addInst(new inst::PushBool(this->_value));
}

void fay::AstLabel::dig3(FayBuilder* builder)
{
	builder->fun()->labels()->addLabel(this->_text);
	AstNode::dig3(builder);
}

void fay::AstLabel::dig4(FayBuilder* builder)
{
	builder->fun()->labels()->setPos(this->_text, builder->instsSize());
	AstNode::dig4(builder);
}

void fay::AstGoto::dig4(FayBuilder* builder)
{
	auto* inst = new inst::Jump(-1);
	builder->fun()->labels()->addTarget(this->_text, &inst->target);
	builder->addInst(inst);
}

void fay::AstIf::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	this->_endLabel = builder->makeLabel();
}

void fay::AstIf::dig4(FayBuilder* builder)
{
	//处理结构为<条件><代码><条件><代码>...<代码>
	for(auto i = 0; i < this->_nodes.size(); i += 2)
	{
		//条件入栈
		this->_nodes[i]->dig4(builder);

		//检测条件，如果else或elseif就跳到对应位置
		if(this->_nodes.size() > i + 2)
		{
			if(!this->_nodes[i + 2]->is<AstCondition>())
				this->throwError(builder->file(), this->_nodes[i + 2]->token(), I18n::Err_BadBranch);

			std::string nextBranchLabel = TOPTR(AstCondition, this->_nodes[i + 2])->label();
			auto* inst = new inst::JumpFalse(-1);
			builder->fun()->labels()->addTarget(nextBranchLabel, &inst->target);
			builder->addInst(inst);
		}
		else
		{
			//没有的话，条件为false的时候跳到结束
			auto* inst = new inst::JumpFalse(-1);
			builder->fun()->labels()->addTarget(this->_endLabel, &inst->target);
			builder->addInst(inst);
		}

		this->_nodes[i + 1]->dig4(builder);

		//跳转到结束
		auto* inst = new inst::Jump(-1);
		builder->fun()->labels()->addTarget(this->_endLabel, &inst->target);
		builder->addInst(inst);
	}

	//设置结束的位置
	builder->fun()->labels()->setPos(this->_endLabel, builder->instsSize());
}

void fay::AstEqualityOP::dig3(FayBuilder* builder)
{
	this->_classType = builder->deps()->findClass(ValueType::Bool);

	AstNode::dig3(builder);

	//先算出目标类型
	auto t1 = this->_nodes[0]->classType();
	auto t2 = this->_nodes[1]->classType();
	auto t3 = FayLangUtils::WeightValueType(builder->deps(), t1, t2);
	this->_itemType = t3;

	//如果和目标类型不一致，就转换一下
	if(t1 != t3)
		this->insertChldNode(0, MKPTR(AstTypeConvert)(this->_token, t1, t3));
	if(t2 != t3)
		this->insertChldNode(1, MKPTR(AstTypeConvert)(this->_token, t2, t3));
}

void fay::AstEqualityOP::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	FayInst* inst = nullptr;
	ValueType type = this->_itemType.lock()->valueType();

	//生成操作代码
	if(this->_text == "==")
		inst = FayLangUtils::OPInst(InstGroupType::Equal, type);
	else if(this->_text == ">")
		inst = FayLangUtils::OPInst(InstGroupType::Greater, type);
	else if(this->_text == "<")
		inst = FayLangUtils::OPInst(InstGroupType::Less, type);
	else if(this->_text == ">=")
		inst = FayLangUtils::OPInst(InstGroupType::GreaterEqual, type);
	else if(this->_text == "<=")
		inst = FayLangUtils::OPInst(InstGroupType::LessEqual, type);
	else if(this->_text == "!=")
		inst = FayLangUtils::OPInst(InstGroupType::NotEqual, type);

	if(inst)
		builder->addInst(inst);
	else
		this->throwError(builder->file(), this->token(), I18n::Err_NotSupportOP, this->_text, this->_itemType.lock()->fullname());
}

void fay::AstCondition::dig3(FayBuilder* builder)
{
	//生成label的操作
	this->_label = builder->makeLabel();

	AstNode::dig3(builder);

	if(!this->_nodes.empty())
	{
		//如果不是Bool，这里进行一下转换
		auto type = this->_nodes[0]->classType();
		if(type->valueType() != ValueType::Bool)
			this->insertChldNode(0, MKPTR(AstTypeConvert)(this->_token, type, builder->deps()->findClass(ValueType::Bool)));
	}

}

void fay::AstCondition::dig4(FayBuilder* builder)
{
	builder->fun()->labels()->setPos(this->_label, builder->instsSize());
	AstNode::dig4(builder);
}

void fay::AstFor::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	this->expr2Label = builder->makeLabel();
	this->endLabel = builder->makeLabel();

	//如果expr2不是Bool，这里进行一下转换
	auto type = this->_nodes[1]->classType();
	if(type->valueType() != ValueType::Bool)
		this->insertChldNode(1, MKPTR(AstTypeConvert)(this->_token, type, builder->deps()->findClass(ValueType::Bool)));
}

void fay::AstFor::dig4(FayBuilder* builder)
{
	//expr1
	if(this->_nodes[0])
		this->_nodes[0]->dig4(builder);

	//再生成expr2的代码，用于对结果进行判断
	builder->fixedLabel(this->expr2Label);
	this->_nodes[1]->dig4(builder);

	//如果不成立，就跳向结束
	auto* inst2 = new inst::JumpFalse(-1);
	builder->useLabel(this->endLabel, &inst2->target);
	builder->addInst(inst2);

	//循环体
	if(this->_nodes[3])
		this->_nodes[3]->dig4(builder);

	//expr3
	if(this->_nodes[2])
		this->_nodes[2]->dig4(builder);

	//这是个表达式，要把最后的值给pop了
	builder->addInst(new inst::Pop());

	//跳回到expr2进行判断
	auto* inst = new inst::Jump(-1);
	builder->useLabel(this->expr2Label, &inst->target);
	builder->addInst(inst);

	//结束的位置
	builder->fixedLabel(this->endLabel);
}

void fay::AstPreOP::dig3(FayBuilder* builder)
{
	//要先执行一下才能确定子节点的类型
	AstNode::dig3(builder);

	PTR(AstNode) subNode = this->_nodes[0];

	//如果是ID，记录一下ID的名字
	if(subNode->is<AstID>())
		this->_id = subNode->text();

	//必需要是数值
	if(!FayLangUtils::IsNumberType(subNode->valueType()))
		this->throwError(builder->file(), this->token(), I18n::Err_NotNumberTyep, TypeDict::ToName(subNode->valueType()));

	//根据类型生成右值
	this->_classType = subNode->classType();
}

void fay::AstPreOP::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	//添加值
	builder->addInst(FayLangUtils::PushNumber(this->valueType(), 1));

	if(this->_text == "++")
		builder->addInst(FayLangUtils::OPInst(InstGroupType::Add, this->valueType()));
	else if(this->_text == "--")
		builder->addInst(FayLangUtils::OPInst(InstGroupType::Sub, this->valueType()));
	else
		this->throwError(builder->file(), this->token(), I18n::Err_BadPreOP, this->_text);

	//如果是ID，需要把值给复制过去
	if(!this->_id.empty())
	{
		pos_t index = builder->fun()->getVarIndex(this->_id);
		builder->addInst(new inst::CopyLocal(index));
	}

	//根据上层节点判断是否要把值留在堆栈中一份
	//如果上层的值是void，说明不会用于计算，那就清除
	//if (this->_parent.lock()->valueType() == ValueType::Void)
	//	builder->addInst(new inst::Pop());
}

void fay::AstFixedNumber::dig3(FayBuilder* builder)
{
	this->_classType = builder->deps()->findClass(this->_type);

	AstNode::dig3(builder);
}

void fay::AstFixedNumber::dig4(FayBuilder* builder)
{
	FayInst* inst = FayLangUtils::PushNumber(this->_type, this->_value);
	if(inst == nullptr)
		this->throwError(builder->file(), this->token(), I18n::Err_BadOP, TypeDict::ToName(this->_type), "push");

	builder->addInst(inst);
}

void fay::AstAssign::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	//要转换成左值需要的类型
	auto t1 = this->_nodes[0]->classType();
	auto t2 = this->_nodes[1]->classType();
	if(t1 != t2)
		this->insertChldNode(1, MKPTR(AstTypeConvert)(this->_token, t2, t1));

	//表达式的值定义为目标值的类型
	this->_classType = t1;
}

void fay::AstAssign::dig4(FayBuilder* builder)
{
	this->_nodes[1]->dig4(builder);
	builder->exprMode = BuildExprMode::LeftValue;
	this->_nodes[0]->dig4(builder);
	builder->exprMode = BuildExprMode::RightValue;
}

void fay::AstUsing::dig1(FayBuilder* builder)
{
	builder->addUsing(this->_text);
}

void fay::AstUsing::dig2(FayBuilder* builder)
{
	builder->addUsing(this->_text);
}

void fay::AstUsing::dig3(FayBuilder* builder)
{
	builder->addUsing(this->_text);
}

void fay::AstUsing::dig4(FayBuilder* builder)
{
	builder->addUsing(this->_text);
}

void fay::AstType::dig2(FayBuilder* builder)
{
	AstNode::dig2(builder);

	auto classes = builder->deps()->findClassWithName(this->_text);
	if(classes.empty())
		this->throwError(builder->file(), this->token(), I18n::Err_NoClass, this->_text);
	else if(classes.size() > 1)
		this->throwError(builder->file(), this->token(), I18n::Err_RepeatedClass, this->_text);

	this->_classType = classes[0];
}

void fay::AstReturn::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	if(!this->_nodes.empty())
	{
		PTR(FayFun) fun = builder->fun();

		if(fun->returnValue() == nullptr || fun->returnValue()->valueType() == ValueType::Void)
			this->throwError(builder->file(), this->token(), I18n::Err_ReturnValue);

		//先算出目标类型
		auto t1 = this->_nodes[0]->classType();
		auto t2 = fun->returnValue();

		//如果和目标类型不一致，就转换一下
		if(t1 != t2)
			this->insertChldNode(0, MKPTR(AstTypeConvert)(this->_token, t1, t2));
	}
}

void fay::AstReturn::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);
	builder->addInst(new inst::Return());
}

void fay::AstMinusOP::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	if(this->_nodes.size() < 1 || this->_nodes[0] == nullptr)
		this->throwError(builder, I18n::Err_NoRightValue);

	this->_classType = this->_nodes[0]->classType();
}

void fay::AstMinusOP::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);
	auto inst = FayLangUtils::OPInst(InstGroupType::Minus, this->valueType());
	builder->addInst(inst);
}

void fay::AstPostOP::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	PTR(AstNode) subNode = this->_nodes[0];

	//如果是ID，记录一下ID的名字
	if(subNode->is<AstID>())
		this->_id = subNode->text();

	//必需要是数值
	if(!FayLangUtils::IsNumberType(subNode->valueType()))
		this->throwError(builder->file(), this->token(), I18n::Err_NotNumberTyep, TypeDict::ToName(subNode->valueType()));

	//根据类型生成右值
	this->_classType = subNode->classType();
}

void fay::AstPostOP::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	//先把栈顶的值复制一个
	builder->addInst(new inst::Dup());

	//添加值
	builder->addInst(FayLangUtils::PushNumber(this->valueType(), 1));

	if(this->_text == "++")
		builder->addInst(FayLangUtils::OPInst(InstGroupType::Add, this->valueType()));
	else if(this->_text == "--")
		builder->addInst(FayLangUtils::OPInst(InstGroupType::Sub, this->valueType()));
	else
		this->throwError(builder->file(), this->token(), I18n::Err_BadPostOP, this->_text);

	//如果是ID，需要把值给复制过去
	if(!this->_id.empty())
	{
		pos_t index = builder->fun()->getVarIndex(this->_id);
		builder->addInst(new inst::CopyLocal(index));
	}

	//把复制的值给移除了
	builder->addInst(new inst::Pop());

	//根据上层节点判断是否要把值留在堆栈中一份
	//如果上层的值是void，说明不会用于计算，那就清除
	//if (this->_parent.lock()->valueType() == ValueType::Void)
	//	builder->addInst(new inst::Pop());
}

void fay::AstExprStmt::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);
	this->_classType = this->_nodes[0]->classType();
}

void fay::AstExprStmt::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	//如果表达式是有值的，需要把堆栈里的值清一下
	if(!this->_classType.expired() && this->_classType.lock()->valueType() != ValueType::Void)
		builder->addInst(new inst::Pop());
}

void fay::AstBoolNot::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	auto boolType = builder->deps()->findClass(ValueType::Bool);
	this->_classType = boolType;

	auto subType = this->_nodes[0]->classType();
	if(subType != this->_classType.lock())
		this->insertChldNode(0, MKPTR(AstTypeConvert)(this->_token, subType, boolType));
}

void fay::AstBoolNot::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);
	builder->addInst(new inst::BoolNot());
}

void fay::AstBitNot::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	this->_classType = this->_nodes[0]->classType();
	if(!FayLangUtils::IsIntegerNumberType(this->valueType()))
		this->throwError(builder->file(), this->token(), I18n::Err_NotIntType);
}

void fay::AstBitNot::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);
	builder->addInst(FayLangUtils::OPInst(InstGroupType::BitNot, this->valueType()));
}

void fay::AstCast::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);
	this->_classType = builder->deps()->findClass(this->_text);
}

void fay::AstCast::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);
	builder->addInst(FayLangUtils::ConvertInst(this->_nodes[0]->valueType(), this->valueType()));
}

void fay::AstBitOP::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	//先算出目标类型
	auto t1 = this->_nodes[0]->classType();
	auto t2 = this->_nodes[1]->classType();
	auto t3 = FayLangUtils::WeightValueType(builder->deps(), t1, t2);
	this->_classType = t3;

	//限定要是整数
	if(!FayLangUtils::IsIntegerNumberType(t3->valueType()))
		this->throwError(builder->file(), this->token(), I18n::Err_NotIntType, t3->fullname());

	//如果和目标类型不一致，就转换一下
	if(t1 != t3)
		this->insertChldNode(0, MKPTR(AstTypeConvert)(this->_token, t1, t3));
	if(t2 != t3)
		this->insertChldNode(1, MKPTR(AstTypeConvert)(this->_token, t2, t3));
}

void fay::AstBitOP::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	FayInst* inst = nullptr;

	//生成操作代码
	if(this->_text == "&")
		inst = FayLangUtils::OPInst(InstGroupType::BitAnd, this->valueType());
	else if(this->_text == "|")
		inst = FayLangUtils::OPInst(InstGroupType::BitOr, this->valueType());
	else if(this->_text == "^")
		inst = FayLangUtils::OPInst(InstGroupType::BitXor, this->valueType());

	if(inst)
		builder->addInst(inst);
	else
		this->throwError(builder->file(), this->token(), I18n::Err_BadOP, this->_text, this->_classType.lock()->fullname());
}

void fay::AstBoolOP::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	//先算出目标类型
	auto t1 = this->_nodes[0]->classType();
	auto t2 = this->_nodes[1]->classType();
	auto t3 = builder->deps()->findClass(ValueType::Bool);
	this->_classType = t3;

	//如果和目标类型不一致，就转换一下
	if(t1 != t3)
		this->insertChldNode(0, MKPTR(AstTypeConvert)(this->_token, t1, t3));
	if(t2 != t3)
		this->insertChldNode(1, MKPTR(AstTypeConvert)(this->_token, t2, t3));
}

void fay::AstBoolOP::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	FayInst* inst = nullptr;

	//生成操作代码
	if(this->_text == "&&")
		inst = new inst::BoolAnd();
	else if(this->_text == "||")
		inst = new inst::BoolOr();

	if(inst)
		builder->addInst(inst);
	else
		this->throwError(builder->file(), this->token(), I18n::Err_BadOP, this->_text, this->_classType.lock()->fullname());
}

void fay::AstCondExpr::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	//如果判断表达式不是Bool类型的，要转换成Boole类型
	auto cond = this->_nodes[0];
	if(cond->valueType() != ValueType::Bool)
		this->insertChldNode(1, MKPTR(AstTypeConvert)(cond->token(), cond->classType(), builder->deps()->findClass(ValueType::Bool)));

	//如果后面的两个项目返回的类型不一致，需要统一下
	auto t1 = this->_nodes[1]->classType();
	auto t2 = this->_nodes[2]->classType();
	auto t3 = FayLangUtils::WeightValueType(builder->deps(), t1, t2);
	this->_classType = t3;

	//如果和目标类型不一致，就转换一下
	if(t1 != t3)
		this->insertChldNode(1, MKPTR(AstTypeConvert)(this->_token, t1, t3));
	if(t2 != t3)
		this->insertChldNode(2, MKPTR(AstTypeConvert)(this->_token, t2, t3));

	//生成所有的标签
	this->_v2Label = builder->makeLabel();
	this->_endLabel = builder->makeLabel();
}

void fay::AstCondExpr::dig4(FayBuilder* builder)
{
	this->_nodes[0]->dig4(builder);
	{
		//加入否定跳转
		auto* inst = new inst::JumpFalse(-1);
		builder->useLabel(this->_v2Label, &inst->target);
		builder->addInst(inst);
	}

	this->_nodes[1]->dig4(builder);
	{
		//跳转到结束
		auto* inst = new inst::Jump(-1);
		builder->useLabel(this->_endLabel, &inst->target);
		builder->addInst(inst);
	}

	builder->fixedLabel(this->_v2Label);
	this->_nodes[2]->dig4(builder);

	builder->fixedLabel(this->_endLabel);
}

void fay::AstWhile::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	//如果不是Bool，这里进行一下转换
	auto type = this->_nodes[0]->classType();
	if(type->valueType() != ValueType::Bool)
		this->insertChldNode(0, MKPTR(AstTypeConvert)(this->_token, type, builder->deps()->findClass(ValueType::Bool)));

	//生成开始和结束的标签
	this->startLabel = builder->makeLabel();
	this->endLabel = builder->makeLabel();
}

void fay::AstWhile::dig4(FayBuilder* builder)
{
	builder->fixedLabel(this->startLabel);

	this->_nodes[0]->dig4(builder);

	auto* inst1 = new inst::JumpFalse(-1);
	builder->useLabel(this->endLabel, &inst1->target);
	builder->addInst(inst1);

	if(this->_nodes[1] != nullptr)
		this->_nodes[1]->dig4(builder);

	auto* inst2 = new inst::Jump(-1);
	builder->useLabel(this->startLabel, &inst2->target);
	builder->addInst(inst2);

	builder->fixedLabel(this->endLabel);
}

void fay::AstDoWhile::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	//如果不是Bool，这里进行一下转换
	auto type = this->_nodes[1]->classType();
	if(type->valueType() != ValueType::Bool)
		this->insertChldNode(1, MKPTR(AstTypeConvert)(this->_token, type, builder->deps()->findClass(ValueType::Bool)));

	//生成开始的标签
	this->startLabel = builder->makeLabel();
}

void fay::AstDoWhile::dig4(FayBuilder* builder)
{
	builder->fixedLabel(this->startLabel);

	if(this->_nodes[0] != nullptr)
		this->_nodes[0]->dig4(builder);

	this->_nodes[1]->dig4(builder);

	auto* inst1 = new inst::JumpTrue(-1);
	builder->useLabel(this->startLabel, &inst1->target);
	builder->addInst(inst1);
}

void fay::AstNew::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	auto classes = builder->deps()->findClassWithName(this->_text);
	if(classes.empty())
		this->throwError(builder->file(), this->token(), I18n::Err_NoClass, this->_text);
	else if(classes.size() > 1)
		this->throwError(builder->file(), this->token(), I18n::Err_RepeatedClass, this->_text);

	this->_classType = classes[0];
}

void fay::AstNew::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	inst::NewObject* inst = new inst::NewObject(this->_classType.lock()->fullname());
	builder->addInst(inst);
}

void fay::AstField::dig2(FayBuilder* builder)
{
	AstNode::dig2(builder);

	if(!this->_nodes[0])
		this->throwError(builder->file(), this->token(), I18n::Err_NoType);

	PTR(FayClass) type = this->_nodes[0]->classType();
	if(!type)
		this->throwError(builder->file(), this->token(), I18n::Err_BadType);

	if(this->isStatic())
		this->varIndex = builder->clazz()->addStaticVar(this->_text, type);
	else
		this->varIndex = builder->clazz()->addVar(this->_text, type);

}

void fay::AstField::dig4(FayBuilder* builder)
{
	//如果不需要初始化直接返回
	if(this->_nodes[1] == nullptr)
		return;

	this->_nodes[1]->dig4(builder);
	if(this->isStatic())
	{
		auto def = builder->clazz()->findStaticVar(this->varIndex);
		builder->addInst(new inst::SetStatic(builder->clazz()->fullname(), this->_text));
	}
	else
	{
		builder->addInst(new inst::LoadLocal(0));
		builder->addInst(new inst::SetField(this->varIndex));
	}
}

void fay::AstTry::dig4(FayBuilder* builder)
{
	std::string startLabel = builder->makeLabel();
	std::string endLabel = builder->makeLabel();
	std::string catchLabel = builder->makeLabel();

	auto* handler = new TryHandler();
	builder->useLabel(startLabel, &handler->start);
	builder->useLabel(endLabel, &handler->end);
	builder->useLabel(catchLabel, &handler->target);
	builder->fun()->addHandler(handler);

	//开始点
	builder->fixedLabel(startLabel);
	this->_nodes[0]->dig4(builder);

	builder->fixedLabel(endLabel);
	builder->addInst(new inst::LeaveTry());

	builder->fixedLabel(catchLabel);
	this->_nodes[1]->dig4(builder);

	//结束Catch
	builder->addInst(new inst::LeaveCatch());
}
