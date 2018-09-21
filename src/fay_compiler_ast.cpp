#include "fay_compiler_ast.h"
#include "fay_compiler.h"

using namespace fay::compiler;

int fay::compiler::ASTBus::LineNum = 1;

fay::compiler::ASTNode *fay::compiler::ASTBus::ASTRootNode;

fay::compiler::ASTNode::ASTNode() : _parentNode(nullptr), _valueType((ValueType)0)
{
	this->_linenum = ASTBus::LineNum;
}

fay::compiler::ASTNode::ASTNode(const char *v)
{
	new (this)ASTNode();
	this->_type = AST_Text;
	this->_text = v;
}

fay::compiler::ASTNode::ASTNode(ASTType type, const char *v)
{
	new (this)ASTNode();
	this->_type = type;
	this->_text = v;
}

fay::compiler::ASTNode::ASTNode(ASTType type, ASTNode *n1)
{
	new (this)ASTNode();
	this->_type = type;
	this->_childNodes.push_back(n1);
}

fay::compiler::ASTNode::ASTNode(ASTType type, ASTNode *n1, ASTNode *n2)
{
	new (this)ASTNode();
	this->_type = type;
	this->_childNodes.push_back(n1);
	this->_childNodes.push_back(n2);
}

fay::compiler::ASTNode::ASTNode(ASTType type, ASTNode *n1, ASTNode *n2, ASTNode *n3)
{
	new (this)ASTNode();
	this->_type = type;
	this->_childNodes.push_back(n1);
	this->_childNodes.push_back(n2);
	this->_childNodes.push_back(n3);
}

ValueType fay::compiler::ASTNode::valueType()
{
	if (this->_valueType > 0)
		return this->_valueType;

	switch (this->_type)
	{
		case AST_IntValue:
			this->_valueType= VAL_Int;
			break;
		case AST_LongValue:
			this->_valueType = VAL_Long;
			break;
		case AST_StringValue:
			this->_valueType = VAL_String;
			break;
	}

	//如果没有生成预期的节类型，就认为是void
	if (this->_valueType <= 0)
		this->_valueType = VAL_Void;

	return this->_valueType;
}

ASTNode *fay::compiler::ASTNode::childNode(int index)
{
	if (index >= 0 && index < this->_childNodes.size())
		return this->_childNodes[index];

	return nullptr;
}

ASTNode *fay::compiler::ASTNode::addChildNode(ASTNode *node)
{
	//如果添加的为空，报个错
	if (node == nullptr)
		LOG_ERROR("Bad child node : " << this->_childNodes.size());

	//添加子节点
	this->_childNodes.push_back(node);
	node->_parentNode = this;
	return this;
}

void fay::compiler::ASTNode::dump(TextBuilder *tb)
{
	tb->add('<')->add(FayConst::ASTTypeName[this->_type])->add('>');
	if (this->_text.length() > 0)
		tb->add(" ")->add(this->_text);
	tb->add("\t\t\t\t... ")->add(this->_linenum);
	tb->endl();

	tb->indentAdd();
	for (auto it : this->_childNodes)
		it->dump(tb);
	tb->indentSub();
}

void fay::compiler::ASTNode::format(string &text)
{
	switch (this->_type)
	{
		case AST_Text:
		case AST_ID:
		case AST_IntValue:
			text.append(this->_text);
			break;
		case AST_VarStmt:
			text.append("var ");
			this->_childNodes[0]->format(text);
			text.append(":");
			this->_childNodes[1]->format(text);
			text.append(";\n");
			break;
		case AST_AssignStmt:
			this->_childNodes[0]->format(text);
			text.append(" = ");
			this->_childNodes[1]->format(text);
			text.append(";\n");
			break;
		case AST_Call:
			this->_childNodes[0]->format(text);
			text.append("(");
			this->_childNodes[1]->format(text);
			text.append(")");
			text.append(";\n");
			break;
		case AST_Expr2:
			this->_childNodes[0]->format(text);
			text.append(" ");
			this->_childNodes[1]->format(text);
			text.append(" ");
			this->_childNodes[2]->format(text);
			break;
		default:
			for (auto it : this->_childNodes)
				it->format(text);
			break;
	}
}

void fay::compiler::ASTNode::compile(Compiler *compiler)
{
	//编译下面所有的节点
	for (auto it : this->_childNodes)
		it->compile(compiler);

	switch (this->_type)
	{
		case AST_VarStmt:
		{
			string id = this->_childNodes[0]->text();
			string type = this->_childNodes[1]->text();
			compiler->addVar(id, type);
			break;
		}
		case AST_AssignStmt:
		{
			string id = this->_childNodes[0]->text();
			break;
		}
		case AST_IntValue:
		{
			LOG_DEBUG("IntValue : " << this->_text);
			int v = stoi(this->_text);
			compiler->addInstruction(new ins::PushInt(v));
			break;
		}
		case AST_Expr2:
		{
			string sign = this->_childNodes[1]->text();
			if (sign == "+")
			{
				//TODO 对类型的判断
				compiler->addInstruction(new ins::AddInt());
			}

			break;
		}
	}

	//LOG_ERROR("Cannot build AST Node : "<<FayUtils::ASTTypeToString(this->_type));
}

void fay::compiler::ASTNode::build(AppDomain *domain, Compiler *compiler)
{
	//编译下面所有的节点
	for (auto it : this->_childNodes)
		it->build(domain, compiler);

	//处理不同类型的节点
	switch (this->_type)
	{
		case AST_VarStmt:
		{
			string name=this->childNode(0)->text();
			string typeName = this->childNode(1)->text();
			//ValueType type=FayUtils::StringToValueType(typeName);
			compiler->addVar(name, typeName);
			break;
		}
	}

	//LOG_ERROR("Cannot build AST Node : "<<FayUtils::ASTTypeToString(this->_type));
}

string fay::compiler::ASTNode::toString()
{
	return "";
}

