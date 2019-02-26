﻿#include <fay_parser.h>
#include <mirror_utils_string.h>
#include <algorithm>

using namespace fay;
using namespace mirror;

PTR(AstNode) fay::Parser::_MakeLeftRightOPNode(std::function<PTR(AstNode)(TokenStack*)> subExpr, const std::vector<std::string> &ops, TokenStack* stack, std::function<PTR(AstNode)(PTR(Token))> nodeCreater)
{
	auto leftNode = subExpr(stack);
	if(leftNode == nullptr)
		throw ParseException(stack, "err.cannot_find_left_value");

	while(stack->now()->is(TokenType::OP)
		&& (std::find(ops.begin(), ops.end(), stack->now()->text) != ops.end()))
	{
		//生成节点
		auto node = nodeCreater(stack->now());
		node->addChildNode(leftNode);

		//取右值
		stack->next();
		auto rightNode = subExpr(stack);
		if(rightNode == nullptr)
			throw ParseException(stack, "err.cannot_find_right_value");
		node->addChildNode(rightNode);

		//返回新生成的节点
		leftNode = node;
	}
	return leftNode;
}

PTR(AstNode) fay::Parser::_MakeBoolOPNode(std::function<PTR(AstNode)(TokenStack*)> subExpr, std::vector<std::string> ops, TokenStack* stack)
{
	auto leftNode = subExpr(stack);
	if(leftNode == nullptr)
		throw ParseException(stack, "err.cannot_find_left_value");

	while(stack->now()->is(TokenType::OP)
		&& (std::find(ops.begin(), ops.end(), stack->now()->text) != ops.end()))
	{
		//生成节点
		auto node = MKPTR(AstEqualityOP)(stack->now());
		node->addChildNode(leftNode);

		//取右值
		stack->next();
		auto rightNode = subExpr(stack);
		if(rightNode == nullptr)
			throw ParseException(stack, "err.cannot_find_right_value");
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
		descWords.push_back(stack->move()->text);

	if(!stack->now()->is(TokenType::Class))
		throw ParseException(stack, "cannot find keyword : class");
	stack->next();

	//Class name
	if(!stack->now()->is(TokenType::ID))
		throw ParseException(stack, "bad class name");
	PTR(AstClass) node = MKPTR(AstClass)(stack->now(), descWords);
	stack->next();

	//处理父类
	if(stack->now()->is(":"))
	{
		stack->next();

		if(!stack->now()->is(TokenType::ID))
			throw ParseException(stack, "err.bad_class_name");
		node->superClassText = stack->now()->text;
		stack->next();
	}

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
		switch(nextToken->type)
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
				throw ParseException(stack, "unknow token type : " + TypeDict::ToName(nextToken->type));
		}
	}

	if(!stack->now()->is("}"))
		throw ParseException(stack, "cannot find end brace for class");
	stack->next();

	return node;
}

PTR(AstNode) fay::Parser::_Field(TokenStack* stack)
{
	std::vector<std::string> words;
	while(stack->now()->is(TokenType::DescSymbol))
	{
		//node->descWords[stack->now()->text()] = true;
		words.push_back(stack->now()->text);
		stack->next();
	}

	if(!stack->now()->is(TokenType::Var))
		throw ParseException(stack, "unknow desc symbol");
	stack->next();

	if(!stack->now()->is(TokenType::ID))
		throw ParseException(stack, "connot find var name");
	PTR(AstField) node = MKPTR(AstField)(stack->now());
	for(auto it : words)
		node->descWords[it] = true;
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
	stack->next();

	return node;
}

PTR(AstNode) fay::Parser::_Fun(TokenStack* stack)
{
	std::vector<std::string> descWords;
	while(stack->now()->is(TokenType::DescSymbol))
		descWords.push_back(stack->move()->text);

	if(!stack->now()->is(TokenType::Fun))
		throw ParseException(stack, "unknow desc word");
	stack->next();

	if(!stack->now()->is(TokenType::ID) && !stack->now()->is(TokenType::SystemName))
		throw ParseException(stack, "bad function name");

	PTR(AstFun) node = MKPTR(AstFun)(stack->move(), descWords);

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
			throw ParseException(stack, "err.expect", "(");
		stack->next();

		node->addChildNode(_ParamList((stack)));

		if(!stack->now()->is(")"))
			throw ParseException(stack, "err.expect", ")");
		stack->next();

		return node;
	}

	return nullptr;
}

PTR(AstNode) fay::Parser::_New(TokenStack* stack)
{
	if(!stack->now()->is(TokenType::New))
		throw ParseException(stack, "err.expect", "new");
	stack->next();

	if(!stack->now()->is(TokenType::ID))
		throw ParseException(stack, "err.not_class_name");
	PTR(AstNew) node = MKPTR(AstNew)(stack->now());
	stack->next();

	if(!stack->now()->is("("))
		throw ParseException(stack, "err.expect", "(");
	stack->next();

	node->addChildNode(_ParamList((stack)));

	if(!stack->now()->is(")"))
		throw ParseException(stack, "err.expect", ")");
	stack->next();

	return node;
}

PTR(AstNode) fay::Parser::_Stmt(TokenStack* stack)
{
	if(stack->now()->is(TokenType::Semicolon))
		return MKPTR(AstEmptyStmt)(stack->move());
	else if(stack->now()->is(TokenType::LeftBrace))
		return _StmtBlock(stack);
	else if(stack->now()->is(TokenType::Var))
		return _StmtVar(stack);
	else if(stack->now()->is(TokenType::Return))
		return _StmtReturn(stack);
	else if(stack->now()->is(TokenType::If))
		return _StmtIf(stack);
	else if(stack->now()->is(TokenType::For))
		return _StmtFor(stack);
	else if(stack->now()->is(TokenType::While))
		return _StmtWhile(stack);
	else if(stack->now()->is(TokenType::Do))
		return _StmtDoWhile(stack);
	else if(stack->now()->is(TokenType::Label))
		return _StmtLabel(stack);
	else if(stack->now()->is(TokenType::Goto))
		return _StmtGoto(stack);
	else if(stack->now()->is(TokenType::Try))
		return _StmtTry(stack);

	//如果不是语句，尝试当做一个表达式来解析
	PTR(AstNode) expr = _Expr(stack);
	if(expr)
	{
		if(stack->now()->is(TokenType::Semicolon))
		{
			PTR(AstExprStmt) node = MKPTR(AstExprStmt)(stack->now());
			node->addChildNode(expr);
			stack->next();
			return node;
		}
		else
			return expr;
	}

	throw ParseException(stack, "unknow statement");
}

PTR(AstNode) fay::Parser::_StmtLabel(TokenStack* stack)
{
	PTR(AstLabel) node;

	if(!stack->now()->is(TokenType::Label))
		throw ParseException(stack, "expect label");
	stack->next();

	if(!stack->now()->is(TokenType::ID))
		throw ParseException(stack, "cannot find label name");
	node = MKPTR(AstLabel)(stack->now());
	stack->next();

	if(!stack->now()->is(TokenType::Semicolon))
		throw ParseException(stack, "expect ;");
	stack->next();

	return node;
}

PTR(AstNode) fay::Parser::_StmtGoto(TokenStack* stack)
{
	PTR(AstGoto) node;

	if(!stack->now()->is(TokenType::Goto))
		throw ParseException(stack, "expect goto");
	stack->next();

	if(!stack->now()->is(TokenType::ID))
		throw ParseException(stack, "cannot find label name");
	node = MKPTR(AstGoto)(stack->now());
	stack->next();

	if(!stack->now()->is(TokenType::Semicolon))
		throw ParseException(stack, "expect ;");
	stack->next();

	return node;
}

PTR(AstNode) fay::Parser::_StmtBlock(TokenStack* stack)
{
	if(!stack->now()->is(TokenType::LeftBrace))
		throw ParseException(stack, "expect {");
	PTR(AstBlock) node = MKPTR(AstBlock)(stack->now());
	stack->next();

	while(true)
	{
		if(stack->now()->is(TokenType::RightBrace))
			break;

		//跳过空语句
		//while(stack->now()->is(TokenType::Semicolon))
		//	stack->next();

		auto subNode = _Stmt(stack);
		if(subNode)
			node->addChildNode(subNode);
		else
			break;

		//跳过空语句
		//while(stack->now()->is(TokenType::Semicolon))
		//	stack->next();
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
	PTR(AstVar) node = MKPTR(AstVar)(stack->now());
	stack->next();

	do
	{
		//变量名
		if(!stack->now()->is(TokenType::ID))
			throw ParseException(stack, "err.bad_var_name");
		PTR(AstVarItem) item = MKPTR(AstVarItem)(stack->now());
		stack->next();

		//数据类型
		if(stack->now()->is(TokenType::Colon))
		{
			stack->next();
			PTR(AstNode) typeNode = _Type(stack);
			item->addChildNode(typeNode);
		}
		else
			item->addChildNode(nullptr);

		//处理赋值
		if(stack->now()->is("="))
		{
			stack->next();
			PTR(AstNode) valueNode = _Expr(stack);
			item->addChildNode(valueNode);
		}
		else
			item->addChildNode(nullptr);

		node->addChildNode(item);

		//是否进行下一项的解析
		if(stack->now()->is(","))
			stack->next();
		else
		{
			if(stack->now()->is(";"))
			{
				stack->next();
				break;
			}
			else
				throw ParseException(stack, "err.expect", ";");
		}
	}
	while(true);

	return node;
}

PTR(AstNode) fay::Parser::_StmtAssign(TokenStack* stack)
{
	return PTR(AstNode)();
}

PTR(AstNode) fay::Parser::_StmtIf(TokenStack* stack)
{
	//if
	if(!stack->now()->is(TokenType::If))
		throw ParseException(stack, "expect if");
	PTR(AstIf) node = MKPTR(AstIf)(stack->now());

	//elseif
	while(stack->now()->is(TokenType::If) || stack->now()->is(TokenType::ElseIf) || stack->now()->is(TokenType::Else))
	{
		PTR(AstCondition) cond = MKPTR(AstCondition)(stack->now());

		if(stack->now()->is(TokenType::If) || stack->now()->is(TokenType::ElseIf))
		{
			stack->next();

			//(
			if(!stack->now()->is("("))
				throw ParseException(stack, "expect (");
			stack->next();

			//判断条件
			PTR(AstNode) condition = _Expr(stack);
			if(!condition)
				throw ParseException(stack, "if condition error");
			cond->addChildNode(condition);

			//)
			if(!stack->now()->is(")"))
				throw ParseException(stack, "expect )");
			stack->next();
		}
		else
			stack->next();

		//添加条件子节点
		node->addChildNode(cond);

		//分支的执行语句
		PTR(AstNode) stmt = _Stmt(stack);
		if(!stmt)
			throw ParseException(stack, "if statement error");
		node->addChildNode(stmt);
	}

	return node;
}

PTR(AstNode) fay::Parser::_StmtFor(TokenStack* stack)
{
	//for
	if(!stack->now()->is(TokenType::For))
		throw ParseException(stack, "expect for");
	PTR(AstFor) node = MKPTR(AstFor)(stack->now());
	stack->next();

	//(
	if(!stack->now()->is("("))
		throw ParseException(stack, "expect (");
	stack->next();

	//expr1
	if(stack->now()->is(TokenType::Semicolon))
	{
		node->addChildNode(nullptr);
		stack->next();
	}
	else
	{
		PTR(AstNode) stmt = _Stmt(stack);
		if(!stmt)
			throw ParseException(stack, "bad expr1");
		node->addChildNode(stmt);
	}

	//expr2
	{
		PTR(AstNode) stmt = _Expr(stack);
		if(!stmt)
			throw ParseException(stack, "bad expr2");
		node->addChildNode(stmt);

		if(!stack->now()->is(TokenType::Semicolon))
			throw ParseException(stack, "expect ;");
		stack->next();
	}

	//expr3
	if(!stack->now()->is(")"))
	{
		PTR(AstNode) stmt = _Stmt(stack);
		if(!stmt)
			throw ParseException(stack, "bad expr3");
		node->addChildNode(stmt);
	}

	//)
	if(!stack->now()->is(")"))
		throw ParseException(stack, "expect )");
	stack->next();

	//循环体
	{
		PTR(AstNode) stmt = _Stmt(stack);
		if(!stmt)
			throw ParseException(stack, "cannot find statement");
		node->addChildNode(stmt);
	}

	return node;
}

PTR(AstNode) fay::Parser::_StmtWhile(TokenStack* stack)
{
	//while
	if(!stack->now()->is(TokenType::While))
		throw ParseException(stack, "err.expect", "while");
	PTR(AstWhile) node = MKPTR(AstWhile)(stack->now());
	stack->next();

	//(
	if(!stack->now()->is("("))
		throw ParseException(stack, "err.expect", "(");
	stack->next();

	//条件
	PTR(AstNode) expr = _Expr(stack);
	if(expr == nullptr)
		throw ParseException(stack, "err.while_expr");
	node->addChildNode(expr);

	//(
	if(!stack->now()->is(")"))
		throw ParseException(stack, "err.expect", ")");
	stack->next();

	//执行体
	PTR(AstNode) block = _Stmt(stack);
	node->addChildNode(block);

	return node;
}

PTR(AstNode) fay::Parser::_StmtDoWhile(TokenStack* stack)
{
	//do
	if(!stack->now()->is(TokenType::Do))
		throw ParseException(stack, "err.expect", "do");
	PTR(AstDoWhile) node = MKPTR(AstDoWhile)(stack->now());
	stack->next();

	//执行体
	PTR(AstNode) block = _Stmt(stack);
	node->addChildNode(block);

	//while
	if(!stack->now()->is(TokenType::While))
		throw ParseException(stack, "err.expect", "while");
	stack->next();

	//(
	if(!stack->now()->is("("))
		throw ParseException(stack, "err.expect", "(");
	stack->next();

	//条件
	PTR(AstNode) expr = _Expr(stack);
	if(expr == nullptr)
		throw ParseException(stack, "err.while_expr");
	node->addChildNode(expr);

	//(
	if(!stack->now()->is(")"))
		throw ParseException(stack, "err.expect", ")");
	stack->next();

	//;
	if(!stack->now()->is(";"))
		throw ParseException(stack, "err.expect", ";");
	stack->next();

	return node;
}

PTR(AstNode) fay::Parser::_StmtReturn(TokenStack* stack)
{
	PTR(AstReturn) node = MKPTR(AstReturn)(stack->now());
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

PTR(AstNode) fay::Parser::_StmtTry(TokenStack* stack)
{
	PTR(AstTry) node = MKPTR(AstTry)(stack->now());
	stack->next();

	PTR(AstNode) stmt = _Stmt(stack);
	if(!stmt)
		throw ParseException(stack, "err.null_stmt");
	node->addChildNode(stmt);

	//catch
	if(!stack->now()->is(TokenType::Catch))
		throw ParseException(stack, "err.expect", "catch");
	stack->next();

	stmt = _Stmt(stack);
	if(!stmt)
		throw ParseException(stack, "err.null_stmt");
	node->addChildNode(stmt);

	//finally
	if(stack->now()->is(TokenType::Finally))
	{
		stack->next();
		stmt = _Stmt(stack);
		if(!stmt)
			throw ParseException(stack, "err.null_stmt");
		node->addChildNode(stmt);
	}

	return node;
}

PTR(AstNode) fay::Parser::_Array(TokenStack* stack)
{
	if(!stack->now()->is("["))
		throw ParseException(stack, "expect array start with [");
	auto node = MKPTR(AstArray)(stack->now());
	stack->next();

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
	PTR(AstParamDefineList) node = MKPTR(AstParamDefineList)(stack->now());

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
	PTR(AstParams) node = MKPTR(AstParams)(stack->now());

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
	else if(stack->now()->is(TokenType::New))
		return _New(stack);
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
		leftNode = MKPTR(AstBracket)(stack->now());
		stack->next();

		//取下标
		auto n = _Expr(stack);
		if(!n)
			throw ParseException(stack, "bad index");

		if(!stack->now()->is("]"))
			throw ParseException(stack, "except ]");

		stack->next();
		leftNode->addChildNode(leftNode);
		leftNode->addChildNode(n);
	}

	return leftNode;
}

PTR(AstNode) fay::Parser::_ExprParen(TokenStack* stack)
{
	if(stack->now()->is("("))
	{
		stack->next();

		auto subNode = _Expr(stack);
		if(subNode == nullptr)
			throw ParseException(stack, "err.bad_expr");

		if(!stack->now()->is(")"))
			throw ParseException(stack, "err.expect", ")");
		stack->next();

		return subNode;
	}

	return _ExprBracket(stack);
}

PTR(AstNode) fay::Parser::_ExprMinus(TokenStack* stack)
{
	if(stack->now()->is(TokenType::OP) && stack->now()->is("-"))
	{
		auto node = MKPTR(AstMinusOP)(stack->now());
		stack->next();
		node->addChildNode(_ExprParen(stack));
		return node;
	}

	return _ExprParen(stack);
}

PTR(AstNode) fay::Parser::_ExprCast(TokenStack* stack)
{
	if(stack->now()->is("(") && stack->after(1)->is(TokenType::BasicType) && stack->after(2)->is(")"))
	{
		stack->next();
		auto node = MKPTR(AstCast)(stack->now());
		stack->next();
		stack->next();
		auto subNode = _ExprMinus(stack);
		node->addChildNode(subNode);
		return node;
	}

	return  _ExprMinus(stack);
}

PTR(AstNode) fay::Parser::_ExprPost(TokenStack* stack)
{
	auto node = _ExprCast(stack);

	if(stack->now()->is(TokenType::OP) &&
		(stack->now()->is("++") || stack->now()->is("--")))
	{
		auto newNode = MKPTR(AstPostOP)(stack->now());
		newNode->addChildNode(node);
		stack->next();
		node = newNode;
	}

	return node;
}

PTR(AstNode) fay::Parser::_ExprPre(TokenStack* stack)
{
	if(stack->now()->is(TokenType::OP))
	{
		if(stack->now()->is("++")
			|| stack->now()->is("--"))
		{
			auto node = MKPTR(AstPreOP)(stack->move());
			auto rightNode = _ExprPost(stack);
			node->addChildNode(rightNode);
			return node;
		}
		else if(stack->now()->is("!"))
		{
			auto node = MKPTR(AstBoolNot)(stack->move());
			auto rightNode = _ExprPost(stack);
			node->addChildNode(rightNode);
			return node;
		}
		else if(stack->now()->is("~"))
		{
			auto node = MKPTR(AstBitNot)(stack->move());
			//LOG_DEBUG(stack->after()->toString());
			auto rightNode = _ExprPost(stack);
			node->addChildNode(rightNode);
			return node;
		}
	}

	return _ExprPost(stack);
}

PTR(AstNode) fay::Parser::_ExprMulDiv(TokenStack* stack)
{
	return _MakeLeftRightOPNode(_ExprPre, { "*", "/", "%" }, stack, [](PTR(Token) token)->PTR(AstNode) { return MKPTR(AstLeftRightOP)(token); });
}

PTR(AstNode) fay::Parser::_ExprAddSub(TokenStack* stack)
{
	return _MakeLeftRightOPNode(_ExprMulDiv, { "+", "-" }, stack, [](PTR(Token) token)->PTR(AstNode) { return MKPTR(AstLeftRightOP)(token); });
}

PTR(AstNode) fay::Parser::_ExprLeftRightMove(TokenStack* stack)
{
	return _MakeLeftRightOPNode(_ExprAddSub, { ">>", "<<" }, stack, [](PTR(Token) token)->PTR(AstNode) { return MKPTR(AstLeftRightOP)(token); });
}

PTR(AstNode) fay::Parser::_ExprEquality(TokenStack* stack)
{
	return _MakeBoolOPNode(_ExprLeftRightMove, { ">", "<", "==", ">=", "<=", "!=" }, stack);
}

PTR(AstNode) fay::Parser::_ExprBit(TokenStack* stack)
{
	return _MakeLeftRightOPNode(_ExprEquality, { "&", "^", "|" }, stack, [](PTR(Token) token)->PTR(AstNode) { return MKPTR(AstBitOP)(token); });
}

PTR(AstNode) fay::Parser::_ExprBool(TokenStack* stack)
{
	return _MakeLeftRightOPNode(_ExprBit, { "&&", "||" }, stack, [](PTR(Token) token)->PTR(AstNode) { return MKPTR(AstBoolOP)(token); });
}

PTR(AstNode) fay::Parser::_ExprCondExpr(TokenStack* stack)
{
	//return _MakeLeftRightOPNode(_ExprCondOption, { "?" }, stack, [](PTR(Token) token)->PTR(AstNode) { return MKPTR(AstCondExpr)(token); });

	auto expr = _ExprBool(stack);
	if(expr == nullptr)
		throw ParseException(stack, "err.bad_expr");

	if(stack->now()->is(TokenType::OP) && stack->now()->is("?"))
	{
		stack->next();

		//取第一个值
		auto v1 = _Expr(stack);
		if(v1 == nullptr)
			throw ParseException(stack, "err.bad_expr");

		//:
		if(!stack->now()->is(":"))
			throw ParseException(stack, "err.expect", ":");
		stack->next();

		//取第二个值
		auto v2 = _Expr(stack);
		if(v2 == nullptr)
			throw ParseException(stack, "err.bad_expr");

		//生成节点
		auto node = MKPTR(AstCondExpr)(stack->now());
		node->addChildNode(expr);
		node->addChildNode(v1);
		node->addChildNode(v2);
		return node;
	}

	return expr;
}

PTR(AstNode) fay::Parser::_ExprAssign(TokenStack* stack)
{
	return _MakeLeftRightOPNode(_ExprCondExpr, { "=", "*=", "/=", "+=", "-=", "%=", "<<=", ">>=", "&=", "^=", "|=" }, stack, [](PTR(Token) token)->PTR(AstNode) { return MKPTR(AstAssign)(token); });
}

PTR(AstNode) fay::Parser::_Expr(TokenStack* stack)
{
	return _ExprAssign(stack);
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
		PTR(AstBracket) node = MKPTR(AstBracket)(stack->now());
		stack->next();
		auto expr1 = _Expr(stack);
		if(!stack->move()->is(")"))
			throw ParseException(stack, "expect )");

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
		leftNode = MKPTR(AstBracket)(stack->now());
		stack->next();
		auto index = _Expr(stack);
		if(!index)
			throw ParseException(stack, "bad index");

		if(!stack->move()->is("]"))
			throw ParseException(stack, "err.expect", "]");
		leftNode->addChildNode(leftNode);
		leftNode->addChildNode(index);
	}

	return leftNode;
}

PTR(AstNode) fay::Parser::_AddrExpr(TokenStack* stack)
{
	return _AddrExprBracket(stack);
}

PTR(AstNode) fay::Parser::Parse(PTR(FayFile) file, PTR(std::vector<PTR(Token)>) tokens)
{
	TokenStack stack(file, tokens);
	PTR(AstFile) ast = MKPTR(AstFile)(file);

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
			switch(token->type)
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

