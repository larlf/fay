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

void fay::AstNode::makeOutline(OutlineBuilder* builder)
{
	for each(auto it in this->_nodes)
	{
		it->makeOutline(builder);
	}
}

void fay::AstNode::makeInst(InstBuilder* builder)
{
	for each(auto it in this->_nodes)
	{
		it->makeInst(builder);
	}
}

void fay::AstClass::makeOutline(OutlineBuilder* builder)
{
	builder->beginClass(this->_text);
	AstNode::makeOutline(builder);
	builder->endClass();
}

void fay::AstFun::makeOutline(OutlineBuilder* builder)
{
	PTR(FayAstCode) code = MKPTR(FayAstCode)(this->_nodes[2]);
	builder->beginFun(this->_text, code);
	AstNode::makeOutline(builder);
	builder->endFun();
}

void fay::AstFile::makeOutline(OutlineBuilder* builder)
{
	builder->beginFile(this->_text);
	AstNode::makeOutline(builder);
	builder->endFile();
}

std::vector<PTR(FayInst)> fay::FayAstCode::insts()
{
	PTR(InstBuilder) builder = MKPTR(InstBuilder)();
	this->_ast->makeInst(builder.get());

	return builder->insts();
}

void fay::AstString::makeInst(InstBuilder* builder)
{
	builder->addInst(MKPTR(InstPushString)(this->_text));
}

ValueType fay::AstString::valueType()
{
	return ValueType::String;
}

void fay::AstParamDefine::makeOutline(OutlineBuilder* builder)
{
	PTR(FayParamDef) p = MKPTR(FayParamDef)(this->_text, this->_nodes[0]->text());
	builder->fun()->addParam(p);
}

void fay::AstPackage::makeOutline(OutlineBuilder* builder)
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
