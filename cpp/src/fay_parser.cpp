#include <fay_parser.h>
#include <mirror_utils_string.h>
#include <algorithm>

using namespace fay;
using namespace mirror;

PTR(AstNode) fay::Parser::_MakeLeftRightOPNode(std::function<PTR(AstNode)(TokenStack*)> subExpr,const std::vector<std::string> &ops, TokenStack* stack)
{
	auto leftNode = subExpr(stack);
	while (stack->now()->is(TokenType::OP)
		&& (std::find(ops.begin(), ops.end(), stack->now()->text()) != ops.end()))
	{
		//生成节点
		auto node = MKPTR(AstLeftRightOP)(stack->now()->text());
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
	while (stack->now()->is(TokenType::OP)
		&& (std::find(ops.begin(), ops.end(), stack->now()->text()) != ops.end()))
	{
		//生成节点
		auto node = MKPTR(AstBoolOP)(stack->now()->text());
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

PTR(AstNode) fay::Parser::_File(TokenStack* stack, const std::string &filename)
{
	PTR(AstFile) ast = MKPTR(AstFile)(filename);

	while (true)
	{
		PTR(Token) token = stack->findNextToken(
		{
			TokenType::Class,
			TokenType::Using,
			TokenType::Package
		});

		PTR(AstNode) node;
		switch (token->type())
		{
			case TokenType::Class:
				break;
			case TokenType::Using:
				node = Parser::_Using(stack);
				break;
			case TokenType::Package:
				node = Parser::_Package(stack);
				break;
		}

		if (node)
			ast->addChildNode(node);
		else
			break;
	}

	return ast;
}

PTR(AstNode) fay::Parser::_Using(TokenStack* stack)
{
	PTR(AstNode) node;

	if (stack->now()->is(TokenType::Using))
	{
		if (!stack->next()->is(TokenType::ID))
			throw ParseException(stack, "using format error");

		node = MKPTR(AstUsing)(stack->now()->text());

		if (!stack->next()->is(TokenType::Semicolon))
			throw ParseException(stack, "using not stop with ;");

		stack->next();
	}

	return node;
}

PTR(AstNode) fay::Parser::_Package(TokenStack* stack)
{
	PTR(AstPackage) node;

	if (stack->now()->is(TokenType::Package))
	{
		if (!stack->next()->is(TokenType::ID))
			throw ParseException(stack, "package format error");

		node = MKPTR(AstPackage)(stack->now()->text());

		if (!stack->next()->is(TokenType::Semicolon))
			throw ParseException(stack, "package not stop with ;");

		stack->next();
	}

	return node;
}

PTR(AstNode) fay::Parser::_Call(TokenStack* stack)
{
	return PTR(AstNode)();
}



PTR(AstNode) fay::Parser::_ExprPrimary(TokenStack* stack)
{
	if (stack->now()->is(TokenType::Number))
		return MKPTR(AstNumber)(stack->move()->text());
	else if (stack->now()->is(TokenType::String))
		return MKPTR(AstString)(stack->move()->text());
	else if (stack->now()->is(TokenType::Char))
		return MKPTR(AstByte)(stack->move()->text());
	else if (stack->now()->is(TokenType::ID))
	{
		//如果下面是括号，应该是个调用
		if (stack->after()->is("("))
			return Parser::_Call(stack);

		//不然就是一个ID
		return MKPTR(AstID)(stack->now()->text());
	}
	else if (stack->now()->is("("))  //处理括号里的内容
	{
		stack->next();
		auto expr1 = Parser::_Expr(stack);
		if (stack->now()->is(")"))
			stack->next();
		else
			throw ParseException(stack, "except )");
	}

	return nullptr;
}

PTR(AstNode) fay::Parser::_ExprBracket(TokenStack* stack)
{
	auto leftNode = Parser::_ExprPrimary(stack);

	while (leftNode && stack->now()->is("["))
	{
		stack->next();

		//取下标
		auto n = _Expr(stack);
		if (!n)
			throw ParseException(stack, "bad index");

		if (!stack->now()->is("]"))
			throw ParseException(stack, "except ]");

		stack->next();
		leftNode = MKPTR(AstBracket)(std::vector<PTR(AstNode)> {leftNode, n});
	}

	return leftNode;
}

PTR(AstNode) fay::Parser::_ExprPost(TokenStack* stack)
{
	auto node = _ExprBracket(stack);

	if (stack->now()->is(TokenType::OP) &&
		(stack->now()->is("++") || stack->now()->is("--")))
	{
		node = MKPTR(AstPostOP)(stack->now()->text(), std::vector<PTR(AstNode)> { node });
		stack->next();
	}

	return node;
}

PTR(AstNode) fay::Parser::_ExprPre(TokenStack* stack)
{
	if (stack->now()->is(TokenType::OP)
		&& (stack->now()->is("++")
			|| stack->now()->is("--")
			|| stack->now()->is("!")
			|| stack->now()->is("~")))
	{
		auto node = MKPTR(AstPreOP)(stack->move()->text());
		auto rightNode = _ExprPost(stack);
		node->addChildNode(rightNode);
		return node;
	}

	return _ExprPost(stack);
}

PTR(AstNode) fay::Parser::_ExprMulDiv(TokenStack * stack)
{
	return _MakeLeftRightOPNode(_ExprPre, {"*","/","%"}, stack);
}

PTR(AstNode) fay::Parser::_ExprAddSub(TokenStack * stack)
{
	return _MakeLeftRightOPNode(_ExprMulDiv, { "+","-" }, stack);
}

PTR(AstNode) fay::Parser::_ExprLeftRightMove(TokenStack * stack)
{
	return _MakeLeftRightOPNode(_ExprAddSub, { ">>","<<" }, stack);
}

PTR(AstNode) fay::Parser::_ExprBool(TokenStack * stack)
{
	return _MakeBoolOPNode(_ExprLeftRightMove, { ">","<","==",">=","<=" }, stack);
}

PTR(AstNode) fay::Parser::_Expr(TokenStack* stack)
{
	return _ExprBool(stack);
}

PTR(AstNode) fay::Parser::_AddrExprItem(TokenStack * stack)
{
	if (stack->now()->is(TokenType::ID))
	{
		//如果下面是括号，那应该是个调用
		if (stack->after()->is("("))
			return _Call(stack);

		//不然就是一个ID
		return MKPTR(AstID)(stack->move()->text());
	}
	else if (stack->now()->is("("))  //处理括号里的内容
	{
		stack->next();
		auto expr1 = _Expr(stack);
		if (!stack->move()->is(")"))
			throw ParseException(stack, "expect )");

		return MKPTR(AstBracket)(std::vector<PTR(AstNode)>{ expr1 });
	}

	return nullptr;
}

PTR(AstNode) fay::Parser::_AddrExprBracket(TokenStack * stack)
{
	auto leftNode = _AddrExprItem(stack);
	while (leftNode && stack->now()->is("["))
	{
		stack->next();
		auto index = _Expr(stack);
		if (!index)
			throw ParseException(stack, "bad index");

		if (!stack->move()->is("]"))
			throw ParseException(stack, "expect ]");
		leftNode = MKPTR(AstBracket)(std::vector<PTR(AstNode)>{leftNode, index});
	}

	return leftNode;
}

PTR(AstNode) fay::Parser::_AddrExpr(TokenStack * stack)
{
	return _AddrExprBracket(stack);
}

PTR(AstNode) fay::Parser::Parse(PTR(std::vector<PTR(Token)>) tokens, const std::string &filename)
{
	TokenStack stack(tokens);
	return Parser::_File(&stack, filename);
}


