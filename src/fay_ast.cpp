#include "fay_ast.h"
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

//bool fay::AstNode::is(const type_info &type)
//{
//	if(typeid(*this) == type)
//		return true;
//
//	return false;
//}

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

std::string fay::AstNode::traceInfo()
{
	std::string str;

	str.append(this->className()).append("(").append(this->_text).append(")");
	if(this->_token)
		str.append(" @ ").append(this->_token->toString());

	return str;
}

PTR(FayClass) fay::AstNode::classType(FayBuilder* builder)
{
	ValueType vtype = this->valueType();
	return builder->domain()->findType(vtype);
}

void fay::AstNode::toString(mirror::utils::StringBuilder* sb)
{
	sb->add(this->className())
	->add(" T=")->add(this->_text)
	->add(" V=")->add(TypeDict::ToName(this->valueType()));
	sb->increaseIndent();
	for each(auto it in this->_nodes)
	{
		sb->endl();
		if(it)
			it->toString(sb);
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
	this->typeIndex = builder->beginClass(this->_text);
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
	this->_index = builder->beginFun(this->_text);
	AstNode::dig2(builder);
}

void fay::AstFun::dig3(FayBuilder* builder)
{
	builder->bindFun(this->_index);
	AstNode::dig3(builder);
}

void fay::AstFun::dig4(FayBuilder* builder)
{
	builder->bindFun(this->_index);
	AstNode::dig4(builder);
	builder->optimizeInsts();
}

void fay::AstFile::dig1(FayBuilder* builder)
{
	builder->beginFile(this->_text);
	AstNode::dig1(builder);
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

ValueType fay::AstString::valueType()
{
	return ValueType::String;
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
		return n1->toFayType(builder);

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

PTR(FayClass) fay::AstType::toFayType(FayBuilder* builder)
{
	auto t = builder->domain()->findType(this->text());
	if(!t)
		throw BuildException(this->shared_from_this(), "connt find type : " + this->text());
	return t;
}

fay::ValueType fay::AstType::valueType()
{
	//如果还没有处理
	if(this->_valueType == ValueType::Void)
	{
		this->_valueType = TypeDict::ToValueType(this->_text);

		//如果不是简单类型就是对象
		if(this->_valueType == ValueType::Void)
			this->_valueType = ValueType::Object;
	}

	return this->_valueType;
}

void fay::AstCall::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	PTR(AstParams) n1 = this->childNode<AstParams>(0);
	size_t paramSize = n1->size();
	std::vector<PTR(FayClass)> paramsType = n1->paramsType(builder);

	pos_t index = builder->findFun(this->text(), paramsType);
	builder->addInst(new inst::CallStatic(index, paramSize));
}

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
	PTR(FayClass) varType = builder->domain()->findType(this->_nodes[0]->text());
	if(!varType)
		throw BuildException(this->_nodes[0], "cannot find type : " + this->_nodes[0]->text());

	pos_t varIndex = builder->fun()->addVar(varName, varType);

	AstNode::dig3(builder);

	//如果有赋值，检查是否要进行类型转换
	if(this->_nodes.size() > 1)
	{
		ValueType leftType = this->_nodes[0]->valueType();
		ValueType rightType = this->_nodes[1]->valueType();
		if(leftType != rightType)
			this->insertChldNode(1, MKPTR(AstTypeConvert)(this->_token, rightType, leftType));
	}
}

void fay::AstVar::dig4(FayBuilder* builder)
{
	//新添加了变量
	std::string varName = this->text();
	PTR(FayClass) varType = builder->domain()->findType(this->_nodes[0]->text());
	if(!varType)
		throw BuildException(this->_nodes[0], "cannot find type : " + this->_nodes[0]->text());

	pos_t varIndex = builder->fun()->addVar(varName, varType);

	AstNode::dig4(builder);

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
			LOG_ERROR("Unknow value type : " << (int)this->_class);
			break;
	}
}

fay::ValueType fay::AstNumber::valueType()
{
	return this->_val.type();
}

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

std::vector<PTR(FayClass)> fay::AstParams::paramsType(FayBuilder* builder)
{
	std::vector<PTR(FayClass)> ts;
	for each(auto it in this->_nodes)
		ts.push_back(it->classType(builder));

	return ts;
}

PTR(FayClass) fay::AstID::classType(FayBuilder* builder)
{
	auto var = builder->findVar(this->_text);
	if(!var)
		throw BuildException(this->shared_from_this(), "connt find var : " + this->text());

	return var->type();
}

fay::ValueType fay::AstID::valueType()
{
	return this->_valueType;
}

void fay::AstID::dig3(FayBuilder* builder)
{
	auto var = builder->findVar(this->_text);
	if(!var)
		throw BuildException(this->shared_from_this(), "connt find var : " + this->text());

	this->_valueType = FayLangUtils::ClassToValueType(var->type());
}

void fay::AstID::dig4(FayBuilder* builder)
{
	auto index = builder->findVarIndex(this->_text);
	if(index < 0)
		throw BuildException(this->shared_from_this(), "connt find var : " + this->text());

	builder->addInst(new inst::LoadLocal(index));
}

void fay::AstLeftRightOP::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	//先算出目标类型
	ValueType t1 = this->_nodes[0]->valueType();
	ValueType t2 = this->_nodes[1]->valueType();
	this->_valueType = FayLangUtils::WeightValueType(t1, t2);

	//如果和目标类型不一致，就转换一下
	if(t1 != this->_valueType)
		this->insertChldNode(0, MKPTR(AstTypeConvert)(this->_token, t1, this->_valueType));
	if(t2 != this->_valueType)
		this->insertChldNode(1, MKPTR(AstTypeConvert)(this->_token, t2, this->_valueType));
}

void fay::AstLeftRightOP::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	FayInst* inst = nullptr;

	//生成操作代码
	if(this->_text == "+")
		inst = FayLangUtils::OPInst(InstGroupType::Add, this->_valueType);
	else if(this->_text == "-")
		inst = FayLangUtils::OPInst(InstGroupType::Sub, this->_valueType);
	else if(this->_text == "*")
		inst = FayLangUtils::OPInst(InstGroupType::Mul, this->_valueType);
	else if(this->_text == "/")
		inst = FayLangUtils::OPInst(InstGroupType::Div, this->_valueType);

	if(inst)
		builder->addInst(inst);
	else
		throw BuildException(this->shared_from_this(), "unknow op inst : " + this->_text + " " + TypeDict::ToName(this->_valueType));
}

fay::ValueType fay::AstLeftRightOP::valueType()
{
	return this->_valueType;
}

void fay::AstTypeConvert::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);
	builder->addInst(FayLangUtils::ConvertInst(this->_srcType, this->_destType));
}

fay::ValueType fay::AstTypeConvert::valueType()
{
	return this->_destType;
}

fay::AstBool::AstBool(const PTR(Token)& token)
	: AstNode(token)
{
	if(this->_text == "true")
		this->_value = true;
	else
		this->_value = false;
}

ValueType fay::AstBool::valueType()
{
	return ValueType::Bool;
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

fay::ValueType fay::AstBoolOP::valueType()
{
	return ValueType::Bool;
}

void fay::AstBoolOP::dig3(FayBuilder* builder)
{
	AstNode::dig3(builder);

	//先算出目标类型
	ValueType t1 = this->_nodes[0]->valueType();
	ValueType t2 = this->_nodes[1]->valueType();
	this->_itemType = FayLangUtils::WeightValueType(t1, t2);

	//如果和目标类型不一致，就转换一下
	if(t1 != this->_itemType)
		this->insertChldNode(0, MKPTR(AstTypeConvert)(this->_token, t1, this->_itemType));
	if(t2 != this->_itemType)
		this->insertChldNode(1, MKPTR(AstTypeConvert)(this->_token, t2, this->_itemType));
}

void fay::AstBoolOP::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	FayInst* inst = nullptr;

	//生成操作代码
	if(this->_text == "==")
		inst = FayLangUtils::OPInst(InstGroupType::Equal, this->_itemType);
	else if(this->_text == ">")
		inst = FayLangUtils::OPInst(InstGroupType::Greater, this->_itemType);
	else if(this->_text == "<")
		inst = FayLangUtils::OPInst(InstGroupType::Less, this->_itemType);
	else if(this->_text == ">=")
		inst = FayLangUtils::OPInst(InstGroupType::GreaterEqual, this->_itemType);
	else if(this->_text == "<=")
		inst = FayLangUtils::OPInst(InstGroupType::LessEqual, this->_itemType);
	else if(this->_text == "!=")
		inst = FayLangUtils::OPInst(InstGroupType::NotEqual, this->_itemType);

	if(inst)
		builder->addInst(inst);
	else
		throw BuildException(this->shared_from_this(), "unknow bool inst : " + this->_text + " " + TypeDict::ToName(this->_itemType));
}

void fay::AstCondition::dig3(FayBuilder* builder)
{
	this->_label = builder->makeLabelName();
	builder->fun()->labels()->addLabel(this->_label);

	AstNode::dig3(builder);

	if(this->_nodes.size() > 0)
	{
		//如果不是Bool，这里进行一下转换
		ValueType type = this->_nodes[0]->valueType();
		if(type != ValueType::Bool)
			this->insertChldNode(0, MKPTR(AstTypeConvert)(this->_token, type, ValueType::Bool));
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
	ValueType type = this->_nodes[1]->valueType();
	if(type != ValueType::Bool)
		this->insertChldNode(1, MKPTR(AstTypeConvert)(this->_token, type, ValueType::Bool));
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

ValueType fay::AstPreOP::valueType()
{
	return this->_valueType;
}

void fay::AstPreOP::dig3(FayBuilder* builder)
{
	//要先执行一下才能确定子节点的类型
	AstNode::dig3(builder);

	PTR(AstNode) leftNode = this->_nodes[0];

	//如果是ID，记录一下ID的名字
	if(leftNode->is<AstID>())
		this->idName = leftNode->text();

	//必需要是数值
	if(!FayLangUtils::IsNumberType(leftNode->valueType()))
		throw BuildException(this->shared_from_this(), "not a number type : " + TypeDict::ToName(leftNode->valueType()));

	//根据类型生成右值
	this->_valueType = leftNode->valueType();
	PTR(AstFixedNumber) rightNode = MKPTR(AstFixedNumber)(this->_token, this->_valueType, 1);

	//生成转换后的操作符
	std::string opStr;
	if(this->_text == "++")
		opStr = "+";
	else if(this->_text == "--")
		opStr = "-";
	else
		throw BuildException(this->shared_from_this(), "unknow pre op : " + this->_text);

	//生成操作并取代现有的节点
	PTR(AstLeftRightOP) op = MKPTR(AstLeftRightOP)(this->_token, opStr);
	this->insertChldNode(0, op);
	op->addChildNode(rightNode);

	//需要再执行一下这个，确定操作的类型
	op->dig3(builder);
}

void fay::AstPreOP::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	//如果是ID，需要把值给复制过去
	if(!this->idName.empty())
	{
		pos_t index = builder->fun()->getVarIndex(this->idName);
		builder->addInst(new inst::CopyLocal(index));
	}

	//根据上层节点判断是否要把值留在堆栈中一份
	//如果上层的值是void，说明不会用于计算，那就清除
	if (this->_parent.lock()->valueType() == ValueType::Void)
		builder->addInst(new inst::Pop());
}

ValueType fay::AstFixedNumber::valueType()
{
	return this->_type;
}

void fay::AstFixedNumber::dig4(FayBuilder* builder)
{
	FayInst* inst = FayLangUtils::PushNumber(this->_type, this->_value);
	if(inst == nullptr)
		throw BuildException(this->shared_from_this(), "make push number error : " + TypeDict::ToName(this->_type));

	builder->addInst(inst);
}
