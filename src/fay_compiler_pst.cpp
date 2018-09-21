#include "fay_compiler_pst.h"

ValueType fay::compiler::pst::FixedValue::valueType()
{
	return this->value.type;
}

ValueType fay::compiler::pst::Expr2::valueType()
{
	//这里需要根据子节点的类型，来判断一下
	return item1->valueType();
}


void fay::compiler::pst::Params::addVar(const string &name)
{
	this->items.push_back(name);
}

void fay::compiler::pst::Call::addParam(ExprNode *param)
{
	this->params.push_back(param);
}

void fay::compiler::pst::Call::addParam(const string &id)
{

}

ValueType fay::compiler::pst::IDValue::valueType()
{
	return ValueType();
}

std::string fay::compiler::PSTNode::typeName()
{
	string str;

	for (auto it = FayConst::PSTTypeName.begin(); it != FayConst::PSTTypeName.end(); ++it)
	{
		if (this->_sign & it->first)
		{
			if (str.size())
				str.append(",");

			str.append(it->second);
		}
	}

	return str;
}

void fay::compiler::PSTNode::dump(TextBuilder *tb)
{
	tb->add("[")->add(this->typeName())->add("] ")->add(this->info())->endl();
	tb->indentAdd();
	for (int i = 0; i < this->_childNodes.size(); ++i)
		this->_childNodes[i]->dump(tb);
	tb->indentSub();
}

std::string fay::compiler::PSTNode::info()
{
	return "";
}
