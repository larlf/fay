#include <fay_ast.h>
#include <typeinfo> 

std::string fay::AstNode::className()
{
	std::string name = typeid(*this).name();
	if (name.find_first_of("class ") == 0)
		name = name.substr(6);
	return name;
}

void fay::AstNode::destory()
{
	//清除对子节点的引用
	for each(auto it in this->_nodes)
	{
		if (it)
		{
			it->destory();
			it.reset();
		}
	}
	this->_nodes.clear();

	//清除对父节点的引用
	if (this->_parent)
		this->_parent.reset();
}

void fay::AstNode::addChildNode(PTR(AstNode) node)
{
	//可能会添加空节点用于占位
	if (node != nullptr)
		node->_parent = this->shared_from_this();

	this->_nodes.push_back(node);
}

void fay::AstNode::toString(mirror::utils::StringBuilder & sb)
{
	sb.add(this->className())
		->add("(")->add(this->_text)->add(")");

	sb.indentAdd();
	for each(auto it in this->_nodes)
	{
		sb.endl();
		it->toString(sb);
	}
	sb.indentSub();
}
