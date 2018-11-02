#include <fay_parser.h>
#include <mirror_utils_string.h>
#include <algorithm>

using namespace fay;
using namespace mirror;

PTR(AstNode) fay::Parser::_MakeLeftRightOPNode(std::function<PTR(AstNode)(TokenStack*)> subExpr, const std::vector<std::string> &ops, TokenStack* stack)
{
	auto leftNode = subExpr(stack);
	while(stack->now()->is(TokenType::OP)
		&& (std::find(ops.begin(), ops.end(), stack->now()->text()) != ops.end()))
	{
		//生成节点
		auto node = MKPTR(AstLeftRightOP)(stack->now());
		node->addChildNode(leftNode);

		//取右值
		stack->next();
		auto rightNode = subExpr(stack);
		node->addChildNode(rightNode);

		//返回新生成的节点
		leftNode = node;
	}
	return leftNode;
}

PTR(AstNode) fay::Parser::_MakeBoolOPNode(std::function<PTR(AstNode)(TokenStack*)> subExpr, std::vector<std::string> ops, TokenStack* stack)
{
	auto leftNode = subExpr(stack);
	while(stack->now()->is(TokenType::OP)
		&& (std::find(ops.begin(), ops.end(), stack->now()->text()) != ops.end()))
	{
		//生成节点
		auto node = MKPTR(AstBoolOP)(stack->now());
		node->addChildNode(leftNode);

		//取右值
		stack->next();
		auto rightNode = subExpr(stack);
		node->addChildNode(rightNode);

		//返回新生成的节点
		leftNode = node;
	}
	return leftNode;
}

PTR(AstNode) fay::Parser::_Using(TokenStack* stack)
{
	PTR(AstNode) node;

	if(stack->now()->is(TokenType::Using))
	{
		if(!stack->next()->is(TokenType::ID))
			throw ParseException(stack, "using format error");

		node = MKPTR(AstUsing)(stack->now());

		if(!stack->next()->is(TokenType::Semicolon))
			throw ParseException(stack, "using not stop with ;");

		stack->next();
	}

	return node;
}

PTR(AstNode) fay::Parser::_Package(TokenStack* stack)
{
	PTR(AstPackage) node;

	if(stack->now()->is(TokenType::Package))
	{
		if(!stack->next()->is(TokenType::ID))
			throw ParseException(stack, "package format error");

		node = MKPTR(AstPackage)(stack->now());

		if(!stack->next()->is(TokenType::Semicolon))
			throw ParseException(stack, "package not stop with ;");

		stack->next();
	}

	return node;
}

PTR(AstNode) fay::Parser::_Class(TokenStack* stack)
{
	std::vector<std::string> descWords;
	while(stack->now()->is(TokenType::DescSymbol))
		descWords.push_back(stack->move()->text());

	if(!stack->now()->is(TokenType::Class))
		throw ParseException(stack, "cannot find keyword : class");
	stack->next();

	//Class name
	if(!stack->now()->is(TokenType::ID))
		throw ParseException(stack, "bad class name");
	PTR(AstClass) node = MKPTR(AstClass)(stack->now(), descWords);
	stack->next();

	//开始的括号
	if(!stack->now()->is("{"))
		throw ParseException(stack, "cannot find start brace for class");
	stack->next();

	while(true)
	{
		auto nextToken = stack->findNextToken(
		{
			TokenType::Var,
			TokenType::Fun,
			TokenType::Class,
			TokenType::Interface
		});

		//如果什么也没找到，退出
		if(!nextToken || nextToken->is(TokenType::None))
			break;

		//如果下面是一个新的Class，退出
		if(nextToken->is(TokenType::Class) || nextToken->is(TokenType::Interface))
			break;

		//处理函数和字段
		switch(nextToken->type())
		{
			case TokenType::Var:
			{
				auto subNode = _Field(stack);
				if(subNode)
					node->addChildNode(subNode);
				break;
			}
			case TokenType::Fun:
			{
				auto subNode = _Fun(stack);
				if(subNode)
					node->addChildNode(subNode);
				break;
			}
			default:
				throw ParseException(stack, "unknow token type : " + TypeDict::ToName(nextToken->type()));
		}
	}

	if(!stack->now()->is("}"))
		throw ParseException(stack, "cannot find end brace for class");
	stack->next();

	return node;
}

PTR(AstNode) fay::Parser::_Field(TokenStack* stack)
{
	std::vector<std::string> descWords;
	while(stack->now()->is(TokenType::DescSymbol))
		descWords.push_back(stack->move()->text());

	if(!stack->now()->is(TokenType::Var))
		throw ParseException(stack, "unknow desc symbol");
	stack->next();

	if(!stack->now()->is(TokenType::ID))
		throw ParseException(stack, "connot find var name");
	PTR(AstField) node = MKPTR(AstField)(stack->now());
	stack->next();

	//处理类型，也可能没有，这样的话需要由数据进行推断
	if(stack->now()->is(":"))
	{
		stack->next();
		auto typeNode = _Type(stack);
		node->addChildNode(typeNode);
	}
	else
		node->addChildNode(nullptr);

	if(stack->now()->is("="))
	{
		stack->next();
		auto valueNode = _Expr(stack);
		node->addChildNode(valueNode);
	}

	if(!stack->now()->is(";"))
		throw ParseException(stack, "expert ;");

	return node;
}

PTR(AstNode) fay::Parser::_Fun(TokenStack* stack)
{
	std::vector<std::string> descWords;
	while(stack->now()->is(TokenType::DescSymbol))
		descWords.push_back(stack->move()->text());

	if(!stack->now()->is(TokenType::Fun))
		throw ParseException(stack, "unknow desc word");
	stack->next();

	if(!stack->now()->is(TokenType::ID) && !stack->now()->is(TokenType::SystemName))
		throw ParseException(stack, "bad function name");

	PTR(AstFun) node = MKPTR(AstFun)(stack->move());

	//参数列表
	if(!stack->now()->is("("))
		throw ParseException(stack, "expect (");
	stack->next();
	auto pds = _ParamDefList(stack);
	if(pds)
		node->addChildNode(pds);
	else
		node->addChildNode(nullptr);
	if(!stack->now()->is(")"))
		throw ParseException(stack, "expect )");
	stack->next();

	//返回值
	if(stack->now()->is(":"))
	{
		stack->next();
		auto typeNode = _Type(stack);
		node->addChildNode(typeNode);
	}
	else
		node->addChildNode(nullptr);

	//函数体
	auto bodyNode = _StmtBlock(stack);
	if(!bodyNode)
		throw ParseException(stack, "Cannot find function body");
	node->addChildNode(bodyNode);

	return node;
}

PTR(AstNode) fay::Parser::_Call(TokenStack* stack)
{
	if(stack->now()->is(TokenType::ID))
	{
		PTR(AstCall) node = MKPTR(AstCall)(stack->now());
		stack->next();

		if(!stack->now()->is("("))
			throw ParseException(stack, "expect ( with call");
		stack->next();

		node->addChildNode(_ParamList((stack)));

		if(!stack->now()->is(")"))
			throw ParseException(stack, "expect ) with call");
		stack->next();

		return node;
	}

	return nullptr;
}

PTR(AstNode) fay::Parser::_Stmt(TokenStack* stack)
{
	if (stack->now()->is(TokenType::LeftBrace))
		return _StmtBlock(stack);
	else if (stack->now()->is(TokenType::Var))
		return _StmtVar(stack);
	else if (stack->now()->is(TokenType::Return))
		return _StmtReturn(stack);
	else if (stack->now()->is(TokenType::If))
		return _StmtIf(stack);
	else if (stack->now()->is(TokenType::For))
		return _StmtFor(stack);
	else if (stack->now()->is(TokenType::Label))
		return _StmtLabel(stack);
	else if (stack->now()->is(TokenType::Goto))
		return _StmtGoto(stack);

	auto nextToken = stack->findNextToken({ TokenType::Assign, TokenType::Semicolon });
	switch(nextToken->type())
	{
		case TokenType::Assign:
			return _StmtAssign(stack);
		default:
			return _Expr(stack);
	}

	//throw ParseException(stack, "unknow statement");
}

PTR(AstNode) fay::Parser::_StmtLabel(TokenStack * stack)
{
	PTR(AstLabel) node;

	if (!stack->now()->is(TokenType::Label))
		throw ParseException(stack, "expect label");

	stack->next();
	if (!stack->now()->is(TokenType::ID))
		throw ParseException(stack, "cannot find label name");

	node = MKPTR(AstLabel)(stack->now());

	stack->next();
	if (!stack->now()->is(TokenType::Semicolon))
		throw ParseException(stack, "expect ;");

	return node;
}

PTR(AstNode) fay::Parser::_StmtGoto(TokenStack * stack)
{
	PTR(AstGoto) node;

	if (!stack->now()->is(TokenType::Goto))
		throw ParseException(stack, "expect goto");

	stack->next();
	if (!stack->now()->is(TokenType::ID))
		throw ParseException(stack, "cannot find label name");

	node=MKPTR(AstGoto)(stack->now());

	stack->next();
	if (!stack->now()->is(TokenType::Semicolon))
		throw ParseException(stack, "expect ;");

	return node;
}

PTR(AstNode) fay::Parser::_StmtBlock(TokenStack* stack)
{
	PTR(AstBlock) node = MKPTR(AstBlock)();

	if(!stack->now()->is(TokenType::LeftBrace))
		throw ParseException(stack, "expect {");
	stack->next();

	while(true)
	{
		if(stack->now()->is(TokenType::RightBrace))
			break;

		//跳过空语句
		while(stack->now()->is(TokenType::Semicolon))
			stack->next();

		auto subNode = _Stmt(stack);
		if(subNode)
			node->addChildNode(subNode);
		else
			break;

		//跳过空语句
		while(stack->now()->is(TokenType::Semicolon))
			stack->next();
	}

	if(!stack->now()->is(TokenType::RightBrace))
		throw ParseException(stack, "expect }");
	stack->next();

	return node;
}

PTR(AstNode) fay::Parser::_StmtVar(TokenStack* stack)
{
	if(!stack->now()->is(TokenType::Var))
		throw ParseException(stack, "expect var");
	stack->next();

	//变量名
	if(!stack->now()->is(TokenType::ID))
		throw ParseException(stack, "cannt find var name");
	auto varName = stack->now();
	stack->next();

	//数据类型
	PTR(AstNode) typeNode;
	if(stack->now()->is(TokenType::Colon))
	{
		stack->next();
		typeNode = _Type(stack);
	}

	//处理赋值
	PTR(AstNode) valueNode;
	if(stack->now()->is(TokenType::Assign))
	{
		stack->next();
		valueNode = _Expr(stack);
	}

	if(!stack->now()->is(TokenType::Semicolon))
		throw ParseException(stack, "expect ;");

	PTR(AstVar) node = MKPTR(AstVar)(varName);
	node->addChildNode(typeNode);
	node->addChildNode(valueNode);
	return node;
}

PTR(AstNode) fay::Parser::_StmtAssign(TokenStack* stack)
{
	return PTR(AstNode)();
}

PTR(AstNode) fay::Parser::_StmtIf(TokenStack* stack)
{
	return PTR(AstNode)();
}

PTR(AstNode) fay::Parser::_StmtFor(TokenStack* stack)
{
	return PTR(AstNode)();
}

PTR(AstNode) fay::Parser::_StmtReturn(TokenStack* stack)
{
	PTR(AstReturn) node = MKPTR(AstReturn)();
	stack->next();

	//是否有参数
	if(stack->now()->is(TokenType::Semicolon))
		stack->next();
	else
	{
		auto valueNode = _Expr(stack);
		if(!valueNode)
			throw ParseException(stack, "bad return value");

		node->addChildNode(valueNode);
	}

	//;
	if(!stack->now()->is(TokenType::Semicolon))
		throw ParseException(stack, "expect ;");
	stack->next();

	return node;
}

PTR(AstNode) fay::Parser::_Array(TokenStack* stack)
{
	if(!stack->now()->is("["))
		throw ParseException(stack, "expect array start with [");
	stack->next();

	auto node = MKPTR(AstArray)();
	while(stack->now() && !stack->now()->is("]"))
	{
		auto expr1 = _Expr(stack);
		if(!expr1)
			throw ParseException(stack, "bad array index");

		node->addChildNode(expr1);

		//处理多个下标
		if(stack->now()->is(","))
			stack->next();
	}

	if(!stack->now()->is("]"))
		throw ParseException(stack, "expect array end with ]");

	stack->next();
	return node;
}

PTR(AstNode) fay::Parser::_Type(TokenStack* stack)
{
	if(stack->now()->is(TokenType::BasicType)
		|| stack->now()->is(TokenType::ID))
	{
		PTR(Token) name = stack->now();
		stack->next();

		if(stack->now()->is("[") && stack->after()->is("]"))
		{
			stack->next();
			stack->next();
			return MKPTR(AstArrayType)(name);
		}
		else
			return MKPTR(AstType)(name);
	}

	throw ParseException(stack, "bad type");
}

PTR(AstNode) fay::Parser::_ParamDef(TokenStack* stack)
{
	if(stack->now()->is(TokenType::ID) && stack->after()->is(TokenType::Colon))
	{
		auto node = MKPTR(AstParamDefine)(stack->now());
		stack->next();
		stack->next();

		auto typeNode = _Type(stack);
		if(typeNode)
			node->addChildNode(typeNode);

		return node;
	}

	return nullptr;
}

PTR(AstNode) fay::Parser::_ParamDefList(TokenStack* stack)
{
	PTR(AstParamDefineList) node = MKPTR(AstParamDefineList)();

	while(true)
	{
		auto pd = _ParamDef(stack);
		if(!pd)
			break;

		node->addChildNode(pd);

		if(stack->now()->is(TokenType::Comma))
			stack->next();
		else
			break;
	}

	return node;
}

PTR(AstNode) fay::Parser::_ParamList(TokenStack* stack)
{
	PTR(AstParams) node = MKPTR(AstParams)();

	while(true)
	{
		//没有参数
		if(stack->now()->is(")"))
			return node;

		auto param = _Expr(stack);
		if(!param)
			throw ParseException(stack, "bad param");

		node->addChildNode(param);

		if(stack->now()->is(TokenType::Comma))
			stack->next();
		else
			break;
	}

	return node;
}

PTR(AstNode) fay::Parser::_ExprPrimary(TokenStack* stack)
{
	if(stack->now()->is(TokenType::Number))
		return MKPTR(AstNumber)(stack->move());
	else if(stack->now()->is(TokenType::String))
		return MKPTR(AstString)(stack->move());
	else if(stack->now()->is(TokenType::Char))
		return MKPTR(AstByte)(stack->move());
	else if(stack->now()->is(TokenType::Bool))
		return MKPTR(AstBool)(stack->move());
	else if(stack->now()->is(TokenType::ID))
	{
		//如果下面是括号，应该是个调用
		if(stack->after()->is("("))
			return Parser::_Call(stack);

		//不然就是一个ID
		return MKPTR(AstID)(stack->move());
	}
	else if(stack->now()->is("("))   //处理括号里的内容
	{
		stack->next();
		auto expr1 = Parser::_Expr(stack);
		if(stack->now()->is(")"))
			stack->next();
		else
			throw ParseException(stack, "except )");
	}

	return nullptr;
}

PTR(AstNode) fay::Parser::_ExprBracket(TokenStack* stack)
{
	auto leftNode = Parser::_ExprPrimary(stack);

	while(leftNode && stack->now()->is("["))
	{
		stack->next();

		//取下标
		auto n = _Expr(stack);
		if(!n)
			throw ParseException(stack, "bad index");

		if(!stack->now()->is("]"))
			throw ParseException(stack, "except ]");

		stack->next();
		leftNode = MKPTR(AstBracket)();
		leftNode->addChildNode(leftNode);
		leftNode->addChildNode(n);
	}

	return leftNode;
}

PTR(AstNode) fay::Parser::_ExprPost(TokenStack* stack)
{
	auto node = _ExprBracket(stack);

	if(stack->now()->is(TokenType::OP) &&
		(stack->now()->is("++") || stack->now()->is("--")))
	{
		node = MKPTR(AstPostOP)(stack->now());
		node->addChildNode(node);
		stack->next();
	}

	return node;
}

PTR(AstNode) fay::Parser::_ExprPre(TokenStack* stack)
{
	if(stack->now()->is(TokenType::OP)
		&& (stack->now()->is("++")
			|| stack->now()->is("--")
			|| stack->now()->is("!")
			|| stack->now()->is("~")))
	{
		auto node = MKPTR(AstPreOP)(stack->move());
		auto rightNode = _ExprPost(stack);
		node->addChildNode(rightNode);
		return node;
	}

	return _ExprPost(stack);
}

PTR(AstNode) fay::Parser::_ExprMulDiv(TokenStack* stack)
{
	return _MakeLeftRightOPNode(_ExprPre, { "*", "/", "%" }, stack);
}

PTR(AstNode) fay::Parser::_ExprAddSub(TokenStack* stack)
{
	return _MakeLeftRightOPNode(_ExprMulDiv, { "+", "-" }, stack);
}

PTR(AstNode) fay::Parser::_ExprLeftRightMove(TokenStack* stack)
{
	return _MakeLeftRightOPNode(_ExprAddSub, { ">>", "<<" }, stack);
}

PTR(AstNode) fay::Parser::_ExprBool(TokenStack* stack)
{
	return _MakeBoolOPNode(_ExprLeftRightMove, { ">", "<", "==", ">=", "<=" }, stack);
}

PTR(AstNode) fay::Parser::_Expr(TokenStack* stack)
{
	return _ExprBool(stack);
}

PTR(AstNode) fay::Parser::_AddrExprItem(TokenStack* stack)
{
	if(stack->now()->is(TokenType::ID))
	{
		//如果下面是括号，那应该是个调用
		if(stack->after()->is("("))
			return _Call(stack);

		//不然就是一个ID
		return MKPTR(AstID)(stack->move());
	}
	else if(stack->now()->is("("))   //处理括号里的内容
	{
		stack->next();
		auto expr1 = _Expr(stack);
		if(!stack->move()->is(")"))
			throw ParseException(stack, "expect )");

		PTR(AstBracket) node = MKPTR(AstBracket)();
		node->addChildNode(expr1);
		return node;
	}

	return nullptr;
}

PTR(AstNode) fay::Parser::_AddrExprBracket(TokenStack* stack)
{
	auto leftNode = _AddrExprItem(stack);
	while(leftNode && stack->now()->is("["))
	{
		stack->next();
		auto index = _Expr(stack);
		if(!index)
			throw ParseException(stack, "bad index");

		if(!stack->move()->is("]"))
			throw ParseException(stack, "expect ]");
		leftNode = MKPTR(AstBracket)();
		leftNode->addChildNode(leftNode);
		leftNode->addChildNode(index);
	}

	return leftNode;
}

PTR(AstNode) fay::Parser::_AddrExpr(TokenStack* stack)
{
	return _AddrExprBracket(stack);
}

PTR(AstNode) fay::Parser::Parse(PTR(std::vector<PTR(Token)>) tokens, const std::string &filename)
{
	TokenStack stack(tokens);
	PTR(AstFile) ast = MKPTR(AstFile)(filename);

	while(true)
	{
		PTR(Token) token = stack.findNextToken(
		{
			TokenType::Class,
			TokenType::Using,
			TokenType::Package
		});

		if(token)
		{
			PTR(AstNode) node;
			switch(token->type())
			{
				case TokenType::Class:
					node = _Class(&stack);
					break;
				case TokenType::Using:
					node = _Using(&stack);
					break;
				case TokenType::Package:
					node = _Package(&stack);
					break;
			}

			if(node)
				ast->addChildNode(node);
			else
				break;
		}
		else
			break;
	}

	return ast;
}

fay::ParseException::ParseException(TokenStack* stack, const std::string &msg)
	: std::exception::exception((msg + "\n" + stack->now()->toString()).c_str())
{
	this->_trace = mirror::log::SysTrace::TraceInfo();
}

