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

bool fay::AstNode::is(const type_info &type)
{
	if(typeid(*this) == type)
		return true;

	return false;
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
	if (index >= 0 && index < this->_nodes.size())
	{
		oldNode = this->_nodes[index];
		this->_nodes[index] = node;
	}
	else
		this->_nodes.push_back(node);

	node->_parent = this->shared_from_this();

	if (oldNode)
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

PTR(FayClass) fay::AstNode::classType(FayBuilder * builder)
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
		it->toString(sb);
	}
	sb->decreaseIndent();
}

void fay::AstNode::dig1(FayBuilder* builder)
{
	for each(auto it in this->_nodes)
		it->dig1(builder);
}

void fay::AstNode::dig2(FayBuilder* builder)
{
	for each(auto it in this->_nodes)
		it->dig2(builder);
}

void fay::AstNode::dig3(FayBuilder* builder)
{
	for each(auto it in this->_nodes)
		it->dig3(builder);
}

void fay::AstNode::dig4(FayBuilder* builder)
{
	for each(auto it in this->_nodes)
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

void fay::AstClass::dig3(FayBuilder * builder)
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

void fay::AstFun::dig3(FayBuilder * builder)
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

ValueType fay::AstString::valueType()
{
	return ValueType::String;
}

void fay::AstString::dig4(FayBuilder* builder)
{
	inst::PushString* inst = new inst::PushString(this->text());
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
	if (this->_valueType == ValueType::Void)
	{
		this->_valueType = TypeDict::ToValueType(this->_text);

		//如果不是简单类型就是对象
		if (this->_valueType == ValueType::Void)
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

fay::BuildException::BuildException(PTR(fay::AstNode) ast, const std::string &msg)
	: std::exception::exception((msg + "\n" + ast->traceInfo()).c_str())
{
	this->_trace = mirror::log::SysTrace::TraceInfo();
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

void fay::AstVar::dig3(FayBuilder * builder)
{
	//新添加了变量
	std::string varName = this->text();
	PTR(FayClass) varType = builder->domain()->findType(this->_nodes[0]->text());
	if (!varType)
		throw BuildException(this->_nodes[0], "cannot find type : " + this->_nodes[0]->text());

	pos_t varIndex = builder->fun()->addVar(varName, varType);

	AstNode::dig3(builder);

	//如果有赋值，检查是否要进行类型转换
	if (this->_nodes.size() > 1)
	{
		ValueType leftType = this->_nodes[0]->valueType();
		ValueType rightType = this->_nodes[1]->valueType();
		if (leftType != rightType)
			this->insertChldNode(1, MKPTR(AstTypeConvert)(rightType, leftType));
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

fay::AstNumber::AstNumber(const PTR(Token)& token)
	: AstNode(token)
{
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

void fay::AstNumber::dig4(FayBuilder* builder)
{
	FayInst* inst = nullptr;

	switch(this->_val.type())
	{
		case ValueType::Int:
			builder->addInst(new inst::PushInt(this->_val.intVal()));
			break;
		case ValueType::Long:
			builder->addInst(new inst::PushLong(this->_val.longValue()));
			break;
		case ValueType::Float:
			builder->addInst(new inst::PushFloat(this->_val.floatValue()));
			break;
		case ValueType::Double:
			builder->addInst(new inst::PushDouble(this->_val.doubleValue()));
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

void fay::AstID::dig4(FayBuilder* builder)
{
	auto index = builder->findVarIndex(this->_text);
	if(index < 0)
		throw BuildException(this->shared_from_this(), "connt find var : " + this->text());

	builder->addInst(new inst::LoadLocal(index));
}

void fay::AstLeftRightOP::dig3(FayBuilder *builder)
{
	AstNode::dig3(builder);

	//先算出目标类型
	ValueType t1 = this->_nodes[0]->valueType();
	ValueType t2 = this->_nodes[1]->valueType();
	this->_valueType = FayLangUtils::WeightValueType(t1, t2);

	//如果和目标类型不一致，就转换一下
	if (t1 != this->_valueType)
		this->insertChldNode(0, MKPTR(AstTypeConvert)(t1, this->_valueType));
	if (t2 != this->_valueType)
		this->insertChldNode(1, MKPTR(AstTypeConvert)(t2, this->_valueType));
}

void fay::AstLeftRightOP::dig4(FayBuilder* builder)
{
	AstNode::dig4(builder);

	ValueType leftType = this->_nodes[0]->valueType();
	//ValueType rightType = this->_nodes[1]->valueType();

	switch (this->_valueType)
	{
	case fay::ValueType::Void:
		break;
	case fay::ValueType::Bool:
		break;
	case fay::ValueType::Byte:
		break;
	case fay::ValueType::Int:
		builder->addInst(new inst::AddInt());
		break;
	case fay::ValueType::Long:
		break;
	case fay::ValueType::Float:
		builder->addInst(new inst::AddFloat());
		break;
	case fay::ValueType::Double:
		break;
	case fay::ValueType::String:
		break;
	case fay::ValueType::Object:
		break;
	case fay::ValueType::Function:
		break;
	default:
		throw BuildException(this->shared_from_this(), "unknow add type : " + TypeDict::ToName(this->_valueType));
		break;
	}
}

fay::ValueType fay::AstLeftRightOP::valueType()
{
	return this->_valueType;
}

void fay::AstTypeConvert::dig4(FayBuilder *builder)
{
	AstNode::dig4(builder);
	builder->addInst(FayLangUtils::ConvertInst(this->_srcType, this->_destType));
}

fay::ValueType fay::AstTypeConvert::valueType()
{
	return this->_destType;
}
