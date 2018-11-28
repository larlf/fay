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
	sb->add(this->className())
	->add(" T=")->add(this->_text)
	->add(" V=")->add(TypeDict::ToName(this->valueType()));
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
	this->typeIndex = builder->addClass(this->_text);
	AstNode::dig1(builder);
}

void fay::AstClass::dig2(FayBuilder* builder)
{
	builder->bindClass(this->typeIndex);
	AstNode::dig2(builder);
}

void fay::AstClass::dig3(FayBuilder* builder)
{
	builder->bindClass(this->typeIndex);
	AstNode::dig3(builder);
}

void fay::AstClass::dig4(FayBuilder* builder)
{
	builder->bindClass(this->typeIndex);
	AstNode::dig4(builder);
}

void fay::AstFun::dig2(FayBuilder* builder)
{
	AstNode::dig2(builder);

	FunAccessType _accessType = FunAccessType::Public;
	for(auto it : this->_descWords)
	{
		if(it == "static")
			this->isStatic = true;
		else if(it == "private")
			_accessType = FunAccessType::Private;
		else if(it == "protected")
			_accessType = FunAccessType::Protected;
	}

	//创建这个函数定义
	PTR(FayInstFun) fun = MKPTR(FayInstFun)(builder->domain(), this->_text, this->isStatic, _accessType);

	//处理参数
	if (this->_nodes[1])
	{
		this->_classType = this->_nodes[1]->classType();
		fun->returnValue(this->_nodes[1]->classType());
	}

	this->_index = builder->addFun(fun);
}

void fay::AstFun::dig3(FayBuilder* builder)
{
	builder->bindFun(this->_index, this->isStatic);
	if(!builder->fun())
		throw BuildException(this->shared_from_this(), "err.cannot_find_fun", this->_text);

	AstNode::dig3(builder);
}

void fay::AstFun::dig4(FayBuilder* builder)
{
	builder->bindFun(this->_index, this->isStatic);
	if(!builder->fun())
		throw BuildException(this->shared_from_this(), "err.cannot_find_fun", this->_text);

	AstNode::dig4(builder);
	builder->optimizeInsts();
}

void fay::AstFile::dig1(FayBuilder* builder)
{
	builder->beginFile(this->_text);
	AstNode::dig1(builder);
	builder->endFile();
}

void fay::AstFile::dig2(FayBuilder* builder)
{
	builder->beginFile(this->_text);
	AstNode::dig2(builder);
	builder->endFile();
}

void fay::AstFile::dig3(FayBuilder* builder)
{
	builder->beginFile(this->_text);
	AstNode::dig3(builder);
	builder->endFile();
}

void fay::AstFile::dig4(FayBuilder* builder)
{
	builder->beginFile(this->_text);
	AstNode::dig4(builder);
	builder->endFile();
}

fay::AstString::AstString(const PTR(Token)& token)
	: AstNode(token)
{
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
}

void fay::AstString::dig3(FayBuilder* builder)
{
	this->_classType = (*builder->domain())[ValueType::String];

	AstNode::dig3(builder);
}

void fay::AstString::dig4(FayBuilder* builder)
{
	inst::PushString* inst = new inst::PushString(this->_value);
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
	builder->addParamDefine(this->text(), this->_nodes[0]->text());
	AstNode::dig2(builder);
}

void fay::AstPackage::dig1(FayBuilder* builder)
{
	builder->package(this->_text);
}

void fay::AstCall::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	//取得参数的长度和类型
	PTR(AstParams) paramsNode = this->childNode<AstParams>(0);
	size_t paramSize = paramsNode->size();
	std::vector<PTR(FayClass)> paramsType = paramsNode->paramsType(builder);

	//判断是内部函数还是外部函数
	std::string className;
	std::string funName;
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

	if(className.size() > 0)
	{
		auto classes = builder->domain()->findClass(builder->usings(), className);
		if(classes.size() < 1)
			throw BuildException(this->shared_from_this(), "err.no_class", className);
		else if(classes.size() > 1)
			throw BuildException(this->shared_from_this(), "err.too_many_class", className);

		auto funs = classes[0]->matchFun(funName, paramsType, true);
		if(funs.size() < 1)
			throw BuildException(this->shared_from_this(), "err.no_fun", className, funName);
		else if(funs.size() > 1)
			throw BuildException(this->shared_from_this(), "err.too_many_fun", className, funName);

		PTR(FayFun) fun = funs[0];
		this->_fun = fun;

		//以第一个参数的返回值为准
		if (fun->returnValue())
			this->_classType = fun->returnValue();
	}
	else
	{
		//TODO ：内部函数调用的实现
	}
}

void fay::AstCall::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	PTR(FayFun) fun = this->_fun.lock();
	if(!fun)
		throw BuildException(this->shared_from_this(), "err.unknow_fun", this->_text);

	pos_t index = builder->lib()->findOutsideFun(fun);
	if(fun->isStatic())
		builder->addInst(new inst::CallStatic(index, fun->paramsCount()));
	//TODO : 处理非静态方法
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
			throw BuildException(this->shared_from_this(), "expect AstParamDefine");

		list.push_back(n->getType(builder));
	}

	return list;
}

void fay::AstVar::dig3(FayBuilder* builder)
{
	//新添加了变量
	std::string varName = this->text();
	PTR(FayClass) varType = builder->domain()->findClass(this->_nodes[0]->text());
	if(!varType)
		throw BuildException(this->_nodes[0], "cannot find type : " + this->_nodes[0]->text());

	pos_t varIndex = builder->fun()->addVar(varName, varType);

	AstNode::dig3(builder);

	//如果有赋值，检查是否要进行类型转换
	if(this->_nodes.size() > 1 && this->_nodes[1])
	{
		auto leftType = this->_nodes[0]->classType();
		auto rightType = this->_nodes[1]->classType();
		if(leftType != rightType)
			this->insertChldNode(1, MKPTR(AstTypeConvert)(this->_token, rightType, leftType));
	}
}

void fay::AstVar::dig4(FayBuilder* builder)
{
	//新添加了变量
	std::string varName = this->text();
	PTR(FayClass) varType = builder->domain()->findClass(this->_nodes[0]->text());
	if(!varType)
		throw BuildException(this->_nodes[0], "cannot find type : " + this->_nodes[0]->text());

	pos_t varIndex = builder->fun()->addVar(varName, varType);

	if(this->_nodes.size() > 1 && this->_nodes[1])
		this->_nodes[1]->dig4(builder);
	else
	{
		FayInst* inst = FayLangUtils::PushDefault(this->_nodes[0]->valueType());
		if(inst == nullptr)
			throw BuildException(this->shared_from_this(), "err.no_default_value", this->_nodes[0]->classType()->fullname());
		builder->addInst(inst);
	}

	//存入本地变量
	builder->addInst(new inst::SetLocal(varIndex));
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
//	return (*builder->domain())[this->_val.type()];
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
	this->_classType = (*builder->domain())[this->_val.type()];

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
	auto var = builder->findVar(this->_text);
	if(!var)
		throw BuildException(this->shared_from_this(), "connt find var : " + this->text());

	this->_classType = var->type();
}

void fay::AstID::dig4(FayBuilder* builder)
{
	auto index = builder->findVarIndex(this->_text);
	if(index < 0)
		throw BuildException(this->shared_from_this(), "connt find var : " + this->text());

	//不同模式下的操作不一样
	if(builder->exprMode == BuildExprMode::Assign)
		builder->addInst(new inst::CopyLocal(index));
	else
		builder->addInst(new inst::LoadLocal(index));
}

void fay::AstLeftRightOP::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	//先算出目标类型
	auto t1 = this->_nodes[0]->classType();
	auto t2 = this->_nodes[1]->classType();
	auto t3 = FayLangUtils::WeightValueType(t1, t2);
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

	if(inst)
		builder->addInst(inst);
	else
		throw BuildException(this->shared_from_this(), "unknow op inst : " + this->_text + " " + this->_classType.lock()->fullname());
}

void fay::AstTypeConvert::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);
	builder->addInst(FayLangUtils::ConvertInst(this->_srcType.lock()->valueType(), this->classType()->valueType()));
}

fay::AstBool::AstBool(const PTR(Token)& token)
	: AstNode(token)
{
	if(this->_text == "true")
		this->_value = true;
	else
		this->_value = false;
}

void fay::AstBool::dig3(FayBuilder* builder)
{
	this->_classType = (*builder->domain())[ValueType::Bool];
	AstNode::dig3(builder);
}

void fay::AstBool::dig4(FayBuilder* builder)
{
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
	inst::Jump* inst = new inst::Jump(-1);
	builder->fun()->labels()->addTarget(this->_text, &inst->target);
	builder->addInst(inst);
}

void fay::AstIf::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);



	this->_endLabel = builder->makeLabelName();
	builder->fun()->labels()->addLabel(this->_endLabel);
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
				throw BuildException(this->_nodes[i + 2], "bad branch");

			std::string nextBranchLabel = TOPTR(AstCondition, this->_nodes[i + 2])->label();
			inst::JumpFalse* inst = new inst::JumpFalse(-1);
			builder->fun()->labels()->addTarget(nextBranchLabel, &inst->target);
			builder->addInst(inst);
		}
		else
		{
			//没有的话，条件为false的时候跳到结束
			inst::JumpFalse* inst = new inst::JumpFalse(-1);
			builder->fun()->labels()->addTarget(this->_endLabel, &inst->target);
			builder->addInst(inst);
		}

		this->_nodes[i + 1]->dig4(builder);

		//跳转到结束
		inst::Jump* inst = new inst::Jump(-1);
		builder->fun()->labels()->addTarget(this->_endLabel, &inst->target);
		builder->addInst(inst);
	}

	//设置结束的位置
	builder->fun()->labels()->setPos(this->_endLabel, builder->instsSize());
}

void fay::AstBoolOP::dig3(FayBuilder* builder)
{
	this->_classType = (*builder->domain())[ValueType::Bool];

	AstNode::dig3(builder);

	//先算出目标类型
	auto t1 = this->_nodes[0]->classType();
	auto t2 = this->_nodes[1]->classType();
	auto t3 = FayLangUtils::WeightValueType(t1, t2);
	this->_itemType = t3;

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
		throw BuildException(this->shared_from_this(), "unknow bool inst : " + this->_text + " " + this->_itemType.lock()->fullname());
}

void fay::AstCondition::dig3(FayBuilder* builder)
{
	this->_label = builder->makeLabelName();
	builder->fun()->labels()->addLabel(this->_label);

	AstNode::dig3(builder);

	if(this->_nodes.size() > 0)
	{
		//如果不是Bool，这里进行一下转换
		auto type = this->_nodes[0]->classType();
		if(type->valueType() != ValueType::Bool)
			this->insertChldNode(0, MKPTR(AstTypeConvert)(this->_token, type, (*builder->domain())[ValueType::Bool]));
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

	this->expr2Label = builder->makeLabelName();
	builder->fun()->labels()->addLabel(this->expr2Label);
	this->endLabel = builder->makeLabelName();
	builder->fun()->labels()->addLabel(this->endLabel);

	//如果expr2不是Bool，这里进行一下转换
	auto type = this->_nodes[1]->classType();
	if(type->valueType() != ValueType::Bool)
		this->insertChldNode(1, MKPTR(AstTypeConvert)(this->_token, type, (*builder->domain())[ValueType::Bool]));
}

void fay::AstFor::dig4(FayBuilder* builder)
{
	//expr1
	if(this->_nodes[0])
		this->_nodes[0]->dig4(builder);

	//再生成expr2的代码，用于对结果进行判断
	builder->fun()->labels()->setPos(this->expr2Label, builder->instsSize());
	this->_nodes[1]->dig4(builder);

	//如果不成立，就跳向结束
	inst::JumpFalse* inst2 = new inst::JumpFalse(-1);
	builder->fun()->labels()->addTarget(this->endLabel, &inst2->target);
	builder->addInst(inst2);

	//循环体
	if(this->_nodes[3])
		this->_nodes[3]->dig4(builder);

	//expr3
	if(this->_nodes[2])
		this->_nodes[2]->dig4(builder);

	//跳回到expr2进行判断
	inst::Jump* inst = new inst::Jump(-1);
	builder->fun()->labels()->addTarget(this->expr2Label, &inst->target);
	builder->addInst(inst);

	//结束的位置
	builder->fun()->labels()->setPos(this->endLabel, builder->instsSize());
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
		throw BuildException(this->shared_from_this(), "not a number type : " + TypeDict::ToName(subNode->valueType()));

	//根据类型生成右值
	this->_classType = subNode->classType();
}

void fay::AstPreOP::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	//添加值
	builder->addInst(FayLangUtils::PushNumber(this->valueType(), 1));

	if (this->_text == "++")
		builder->addInst(FayLangUtils::OPInst(InstGroupType::Add, this->valueType()));
	else if (this->_text == "--")
		builder->addInst(FayLangUtils::OPInst(InstGroupType::Sub, this->valueType()));
	else
		throw BuildException(this->shared_from_this(), "err.unknow_pre_op : " + this->_text);

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
	this->_classType = (*builder->domain())[this->_type];

	AstNode::dig3(builder);
}

void fay::AstFixedNumber::dig4(FayBuilder* builder)
{
	FayInst* inst = FayLangUtils::PushNumber(this->_type, this->_value);
	if(inst == nullptr)
		throw BuildException(this->shared_from_this(), "make push number error : " + TypeDict::ToName(this->_type));

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
	builder->exprMode = BuildExprMode::Assign;
	this->_nodes[0]->dig4(builder);
	builder->exprMode = BuildExprMode::Count;
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

	auto classes = builder->domain()->findClass(builder->usings(), this->_text);
	if(classes.size() <= 0)
		throw BuildException(this->shared_from_this(), "err.cannot_find_class", this->_text);
	else if(classes.size() > 1)
		throw BuildException(this->shared_from_this(), "err.unsolved_class", this->_text);

	this->_classType = classes[0];
}

void fay::AstReturn::dig3(FayBuilder * builder)
{
	AstNode::dig3(builder);

	if (this->_nodes.size() > 0)
	{
		PTR(FayFun) fun = builder->fun();

		if (fun->returnValue()->valueType() == ValueType::Void)
			throw BuildException(this->shared_from_this(), "err.return_to_void");

		//先算出目标类型
		auto t1 = this->_nodes[0]->classType();
		auto t2 = fun->returnValue();

		//如果和目标类型不一致，就转换一下
		if (t1 != t2)
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
	this->_classType = this->_nodes[0]->classType();
}

void fay::AstMinusOP::dig4(FayBuilder * builder)
{
	AstNode::dig4(builder);
	auto inst = FayLangUtils::OPInst(InstGroupType::Minus, this->valueType());
	builder->addInst(inst);
}

void fay::AstPostOP::dig3(FayBuilder * builder)
{
	AstNode::dig3(builder);

	PTR(AstNode) subNode = this->_nodes[0];

	//如果是ID，记录一下ID的名字
	if (subNode->is<AstID>())
		this->_id = subNode->text();

	//必需要是数值
	if (!FayLangUtils::IsNumberType(subNode->valueType()))
		throw BuildException(this->shared_from_this(), "not a number type : " + TypeDict::ToName(subNode->valueType()));

	//根据类型生成右值
	this->_classType = subNode->classType();
}

void fay::AstPostOP::dig4(FayBuilder * builder)
{
	AstNode::dig4(builder);

	//先把栈顶的值复制一个
	builder->addInst(new inst::Dup());

	//添加值
	builder->addInst(FayLangUtils::PushNumber(this->valueType(), 1));

	if (this->_text == "++")
		builder->addInst(FayLangUtils::OPInst(InstGroupType::Add, this->valueType()));
	else if (this->_text == "--")
		builder->addInst(FayLangUtils::OPInst(InstGroupType::Sub, this->valueType()));
	else
		throw BuildException(this->shared_from_this(), "err.unknow_post_op : ", this->_text);

	//如果是ID，需要把值给复制过去
	if (!this->_id.empty())
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

void fay::AstExprStmt::dig3(FayBuilder * builder)
{
	AstNode::dig3(builder);
	this->_classType = this->_nodes[0]->classType();
}

void fay::AstExprStmt::dig4(FayBuilder * builder)
{
	AstNode::dig4(builder);
	builder->addInst(new inst::Pop());
}
