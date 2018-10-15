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

void fay::AstNode::addChildNode(PTR(AstNode) node)
{
	//可能会添加空节点用于占位
	if (node != nullptr)
		node->_parent = this->shared_from_this();

	this->_nodes.push_back(node);
}

void fay::AstNode::toString(mirror::utils::StringBuilder* sb)
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

void fay::AstClass::dig1(FayBuilder* builder)
{
	this->typeIndex = builder->beginClass(this->_text);
	AstNode::dig1(builder);
}

void fay::AstFun::dig1(FayBuilder* builder)
{
	builder->beginFun(this->_text);
	AstNode::dig1(builder);
	builder->endFun();
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

void fay::AstParamDefine::dig1(FayBuilder* builder)
{
	PTR(FayParamDef) p = MKPTR(FayParamDef)(this->_text, this->_nodes[0]->text());
	builder->fun()->addParam(p);
}

void fay::AstPackage::dig1(FayBuilder* builder)
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
