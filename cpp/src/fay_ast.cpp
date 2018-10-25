#include "fay_ast.h"
#include "fay_ast.h"
#include "fay_ast.h"
#include "fay_ast.h"
#include <fay_ast.h>
#include <typeinfo>

using namespace fay;

std::string fay::AstNode::className()
{
	std::string name = typeid(*this).name();
	if (name.find_first_of("class ") == 0)
		name = name.substr(6);
	return name;
}

bool fay::AstNode::is(const type_info & type)
{
	if (typeid(*this) == type)
		return true;

	return false;
}

void fay::AstNode::addChildNode(PTR(AstNode) node)
{
	//可能会添加空节点用于占位
	if (node != nullptr)
		node->_parent = this->shared_from_this();
	this->_nodes.push_back(node);
}

std::string fay::AstNode::traceInfo()
{
	std::string str;

	str.append(this->className()).append("(").append(this->_text).append(")");
	if (this->_token)
		str.append(" @ ").append(this->_token->toString());

	return str;
}

void fay::AstNode::toString(mirror::utils::StringBuilder *sb)
{
	sb->add(this->className())
	->add("(")->add(this->_text)->add(")");
	sb->increaseIndent();
	for each(auto it in this->_nodes)
	{
		sb->endl();
		it->toString(sb);
	}
	sb->decreaseIndent();
}

void fay::AstNode::dig1(FayBuilder *builder)
{
	for each(auto it in this->_nodes)
		it->dig1(builder);
}

void fay::AstNode::dig2(FayBuilder *builder)
{
	for each(auto it in this->_nodes)
		it->dig2(builder);
}

void fay::AstNode::dig3(FayBuilder *builder)
{
	for each(auto it in this->_nodes)
		it->dig3(builder);
}

void fay::AstClass::dig1(FayBuilder *builder)
{
	this->typeIndex = builder->beginClass(this->_text);
	AstNode::dig1(builder);
}

void fay::AstClass::dig2(FayBuilder *builder)
{
	builder->bindClass(this->typeIndex);
	AstNode::dig2(builder);
}

void fay::AstClass::dig3(FayBuilder *builder)
{
	builder->bindClass(this->typeIndex);
	AstNode::dig3(builder);
}

void fay::AstFun::dig2(FayBuilder *builder)
{
	this->_index = builder->beginFun(this->_text);
	AstNode::dig2(builder);
}

void fay::AstFun::dig3(FayBuilder *builder)
{
	builder->bindFun(this->_index);
	AstNode::dig3(builder);
	builder->optimizeInsts();
}

void fay::AstFile::dig1(FayBuilder *builder)
{
	builder->beginFile(this->_text);
	AstNode::dig1(builder);
	builder->endFile();
}

ValueType fay::AstString::valueType()
{
	return ValueType::String;
}

void fay::AstString::dig3(FayBuilder *builder)
{
	inst::PushString *inst = new inst::PushString(this->text());
	builder->addInst(inst);
}

PTR(FayType) fay::AstParamDefine::getType(FayBuilder *builder)
{
	auto n1=this->childNode<AstType>(0);
	if (n1)
		return n1->toFayType(builder);

	return nullptr;
}

void fay::AstParamDefine::dig2(FayBuilder *builder)
{
	builder->addParamDefine(this->text(), this->_nodes[0]->text());
	AstNode::dig2(builder);
}

void fay::AstPackage::dig1(FayBuilder *builder)
{
	builder->package(this->_text);
}

ValueType fay::AstType::valueType()
{
	auto it = TypeDict::ValueTypeMap.find(this->_text);
	if (it != TypeDict::ValueTypeMap.end())
		return it->second;
	return ValueType::Object;
}

PTR(FayType) fay::AstType::toFayType(FayBuilder * builder)
{
	auto t=builder->domain()->findType(this->text());
	if (!t)
		throw BuildException(this->shared_from_this(), "connt find type : "+this->text());
	return t;
}

void fay::AstCall::dig3(FayBuilder *builder)
{
	AstNode::dig3(builder);

	std::vector<std::string> paramsType;
	paramsType.push_back("string");

	pos_t index = builder->findFun(this->text(), paramsType);
	LOG_DEBUG("Index : " << index);

	PTR(AstParams) n1 = this->childNode<AstParams>(0);
	size_t paramSize = n1->size();

	builder->addInst(new inst::CallStatic(index, paramSize));
}

fay::BuildException::BuildException(PTR(fay::AstNode) ast, const std::string & msg)
	: std::exception::exception((msg+"\n"+ast->traceInfo()).c_str())
{
	this->_trace = mirror::log::SysTrace::TraceInfo();
}

std::vector<PTR(FayType)> fay::AstParamDefineList::getTypeList(FayBuilder *builder)
{
	std::vector<PTR(FayType)> list;

	for (auto i = 0; i < this->childNodesNum(); ++i)
	{
		auto n = this->childNode<AstParamDefine>(i);
		if (!n)
			throw BuildException(this->shared_from_this(), "expect AstParamDefine");

		list.push_back(n->getType(builder));
	}

	return list;
}
