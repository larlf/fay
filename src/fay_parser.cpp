#include <fay_parser.h>
#include <mirror_utils_string.h>
#include <algorithm>

using namespace fay;
using namespace mirror;

PTR(AstNode) fay::Parser::_MakeLeftRightOPNode(std::function<PTR(AstNode)(TokenStack*)> subExpr, const std::vector<std::string> &ops, TokenStack* stack, std::function<PTR(AstNode)(PTR(Token))> nodeCreater)
{
	auto leftNode = subExpr(stack);
	if(leftNode == nullptr)
		_Error(stack, I18n::Err_NoLeftValue);

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
			_Error(stack, I18n::Err_NoRightValue);
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
		_Error(stack, I18n::Err_NoLeftValue);

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
			_Error(stack, I18n::Err_NoRightValue);
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
			_Error(stack, I18n::Err_BadFormat, "using");

		node = MKPTR(AstUsing)(stack->now());

		if(!stack->next()->is(TokenType::Semicolon))
			_Error(stack, I18n::Err_Expect, ";");

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
			_Error(stack, I18n::Err_BadFormat, "package");

		node = MKPTR(AstPackage)(stack->now());

		if(!stack->next()->is(TokenType::Semicolon))
			_Error(stack, I18n::Err_Expect, ";");

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
		_Error(stack, I18n::Err_NoKeyword, "class");
	stack->next();

	//Class name
	if(!stack->now()->is(TokenType::ID))
		_Error(stack, I18n::Err_BadID);
	PTR(AstClass) node = MKPTR(AstClass)(stack->now(), descWords);
	stack->next();

	//处理父类
	if(stack->now()->is(":"))
	{
		stack->next();

		if(!stack->now()->is(TokenType::ID))
			_Error(stack, I18n::Err_BadID);
		node->superClassText = stack->now()->text;
		stack->next();
	}

	//开始的括号
	if(!stack->now()->is("{"))
		_Error(stack, I18n::Err_Expect, "{");
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
				_Error(stack, I18n::Err_BadTokenType, TypeDict::ToName(nextToken->type));
		}
	}

	if(!stack->now()->is("}"))
		_Error(stack, I18n::Err_Expect, "}");
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
		_Error(stack, I18n::Err_UnknowKeyword, stack->now()->text);
	stack->next();

	if(!stack->now()->is(TokenType::ID))
		_Error(stack, I18n::Err_BadID);
	PTR(AstField) node = MKPTR(AstField)(stack->now());
	for(const auto &it : words)
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
		_Error(stack, I18n::Err_Expect, ";");
	stack->next();

	return node;
}

PTR(AstNode) fay::Parser::_Fun(TokenStack* stack)
{
	std::vector<std::string> descWords;
	while(stack->now()->is(TokenType::DescSymbol))
		descWords.push_back(stack->move()->text);

	if(!stack->now()->is(TokenType::Fun))
		_Error(stack, I18n::Err_UnknowKeyword, stack->now()->text);
	stack->next();

	if(!stack->now()->is(TokenType::ID) && !stack->now()->is(TokenType::SystemName))
		_Error(stack, I18n::Err_BadID);

	PTR(AstFun) node = MKPTR(AstFun)(stack->move(), descWords);

	//参数列表
	if(!stack->now()->is("("))
		_Error(stack, I18n::Err_Expect, "(");
	stack->next();
	auto pds = _ParamDefList(stack);
	if(pds)
		node->addChildNode(pds);
	else
		node->addChildNode(nullptr);
	if(!stack->now()->is(")"))
		_Error(stack, I18n::Err_Expect, ")");
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
		_Error(stack, I18n::Err_NoFunBody);
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
			_Error(stack, I18n::Err_Expect, "(");
		stack->next();

		node->addChildNode(_ParamList((stack)));

		if(!stack->now()->is(")"))
			_Error(stack, I18n::Err_Expect, ")");
		stack->next();

		return node;
	}

	return nullptr;
}

PTR(AstNode) fay::Parser::_New(TokenStack* stack)
{
	if(!stack->now()->is(TokenType::New))
		_Error(stack, I18n::Err_Expect, "new");
	stack->next();

	if(!stack->now()->is(TokenType::ID))
		_Error(stack, I18n::Err_BadID);
	PTR(AstNew) node = MKPTR(AstNew)(stack->now());
	stack->next();

	if(!stack->now()->is("("))
		_Error(stack, I18n::Err_Expect, "(");
	stack->next();

	node->addChildNode(_ParamList((stack)));

	if(!stack->now()->is(")"))
		_Error(stack, I18n::Err_Expect, ")");
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

	_Error(stack, I18n::Err_UnknowStatment);
}

PTR(AstNode) fay::Parser::_StmtLabel(TokenStack* stack)
{
	PTR(AstLabel) node;

	if(!stack->now()->is(TokenType::Label))
		_Error(stack, I18n::Err_Expect, "label");
	stack->next();

	if(!stack->now()->is(TokenType::ID))
		_Error(stack, I18n::Err_BadID);
	node = MKPTR(AstLabel)(stack->now());
	stack->next();

	if(!stack->now()->is(TokenType::Semicolon))
		_Error(stack, I18n::Err_Expect, ";");
	stack->next();

	return node;
}

PTR(AstNode) fay::Parser::_StmtGoto(TokenStack* stack)
{
	PTR(AstGoto) node;

	if(!stack->now()->is(TokenType::Goto))
		_Error(stack, I18n::Err_Expect, "goto");
	stack->next();

	if(!stack->now()->is(TokenType::ID))
		_Error(stack, I18n::Err_BadID);
	node = MKPTR(AstGoto)(stack->now());
	stack->next();

	if(!stack->now()->is(TokenType::Semicolon))
		_Error(stack, I18n::Err_Expect, ";");
	stack->next();

	return node;
}

PTR(AstNode) fay::Parser::_StmtBlock(TokenStack* stack)
{
	if(!stack->now()->is(TokenType::LeftBrace))
		_Error(stack, I18n::Err_Expect, "{");
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
		_Error(stack, I18n::Err_Expect, "}");
	stack->next();

	return node;
}

PTR(AstNode) fay::Parser::_StmtVar(TokenStack* stack)
{
	if(!stack->now()->is(TokenType::Var))
		_Error(stack, I18n::Err_Expect, "var");
	PTR(AstVar) node = MKPTR(AstVar)(stack->now());
	stack->next();

	do
	{
		//变量名
		if(!stack->now()->is(TokenType::ID))
			_Error(stack, I18n::Err_BadID);
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
				_Error(stack, I18n::Err_Expect, ";");
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
		_Error(stack, I18n::Err_Expect, "if");
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
				_Error(stack, I18n::Err_Expect, "(");
			stack->next();

			//判断条件
			PTR(AstNode) condition = _Expr(stack);
			if(!condition)
				_Error(stack, I18n::Err_ExprError);
			cond->addChildNode(condition);

			//)
			if(!stack->now()->is(")"))
				_Error(stack, I18n::Err_Expect, ")");
			stack->next();
		}
		else
			stack->next();

		//添加条件子节点
		node->addChildNode(cond);

		//分支的执行语句
		PTR(AstNode) stmt = _Stmt(stack);
		if(!stmt)
			_Error(stack, I18n::Err_NoBody);
		node->addChildNode(stmt);
	}

	return node;
}

PTR(AstNode) fay::Parser::_StmtFor(TokenStack* stack)
{
	//for
	if(!stack->now()->is(TokenType::For))
		_Error(stack, I18n::Err_Expect, "for");
	PTR(AstFor) node = MKPTR(AstFor)(stack->now());
	stack->next();

	//(
	if(!stack->now()->is("("))
		_Error(stack, I18n::Err_Expect, "(");
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
			_Error(stack, I18n::Err_ExprError);
		node->addChildNode(stmt);
	}

	//expr2
	{
		PTR(AstNode) stmt = _Expr(stack);
		if(!stmt)
			_Error(stack, I18n::Err_ExprError);
		node->addChildNode(stmt);

		if(!stack->now()->is(TokenType::Semicolon))
			_Error(stack, I18n::Err_Expect, ";");
		stack->next();
	}

	//expr3
	if(!stack->now()->is(")"))
	{
		PTR(AstNode) stmt = _Stmt(stack);
		if(!stmt)
			_Error(stack, I18n::Err_ExprError);
		node->addChildNode(stmt);
	}

	//)
	if(!stack->now()->is(")"))
		_Error(stack, I18n::Err_Expect, ")");
	stack->next();

	//循环体
	{
		PTR(AstNode) stmt = _Stmt(stack);
		if(!stmt)
			_Error(stack, I18n::Err_NoBody);
		node->addChildNode(stmt);
	}

	return node;
}

PTR(AstNode) fay::Parser::_StmtWhile(TokenStack* stack)
{
	//while
	if(!stack->now()->is(TokenType::While))
		_Error(stack, I18n::Err_Expect, "while");
	PTR(AstWhile) node = MKPTR(AstWhile)(stack->now());
	stack->next();

	//(
	if(!stack->now()->is("("))
		_Error(stack, I18n::Err_Expect, "(");
	stack->next();

	//条件
	PTR(AstNode) expr = _Expr(stack);
	if(expr == nullptr)
		_Error(stack, I18n::Err_ExprError);
	node->addChildNode(expr);

	//(
	if(!stack->now()->is(")"))
		_Error(stack, I18n::Err_Expect, ")");
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
		_Error(stack, I18n::Err_Expect, "do");
	PTR(AstDoWhile) node = MKPTR(AstDoWhile)(stack->now());
	stack->next();

	//执行体
	PTR(AstNode) block = _Stmt(stack);
	node->addChildNode(block);

	//while
	if(!stack->now()->is(TokenType::While))
		_Error(stack, I18n::Err_Expect, "while");
	stack->next();

	//(
	if(!stack->now()->is("("))
		_Error(stack, I18n::Err_Expect, "(");
	stack->next();

	//条件
	PTR(AstNode) expr = _Expr(stack);
	if(expr == nullptr)
		_Error(stack, I18n::Err_ExprError);
	node->addChildNode(expr);

	//(
	if(!stack->now()->is(")"))
		_Error(stack, I18n::Err_Expect, ")");
	stack->next();

	//;
	if(!stack->now()->is(";"))
		_Error(stack, I18n::Err_Expect, ";");
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
			_Error(stack, I18n::Err_ExprError);

		node->addChildNode(valueNode);
	}

	//;
	if(!stack->now()->is(TokenType::Semicolon))
		_Error(stack, I18n::Err_Expect, ";");
	stack->next();

	return node;
}

PTR(AstNode) fay::Parser::_StmtTry(TokenStack* stack)
{
	PTR(AstTry) node = MKPTR(AstTry)(stack->now());
	stack->next();

	PTR(AstNode) stmt = _Stmt(stack);
	if(!stmt)
		_Error(stack, I18n::Err_NoBody);
	node->addChildNode(stmt);

	//catch
	if(!stack->now()->is(TokenType::Catch))
		_Error(stack, I18n::Err_Expect, "catch");
	stack->next();

	stmt = _Stmt(stack);
	if(!stmt)
		_Error(stack, I18n::Err_NoBody);
	node->addChildNode(stmt);

	//finally
	if(stack->now()->is(TokenType::Finally))
	{
		stack->next();
		stmt = _Stmt(stack);
		if(!stmt)
			_Error(stack, I18n::Err_NoBody);
		node->addChildNode(stmt);
	}

	return node;
}

PTR(AstNode) fay::Parser::_Array(TokenStack* stack)
{
	if(!stack->now()->is("["))
		_Error(stack, I18n::Err_Expect, "array start with [");
	auto node = MKPTR(AstArray)(stack->now());
	stack->next();

	while(stack->now() && !stack->now()->is("]"))
	{
		auto expr1 = _Expr(stack);
		if(!expr1)
			_Error(stack, I18n::Err_BadIndexValue);

		node->addChildNode(expr1);

		//处理多个下标
		if(stack->now()->is(","))
			stack->next();
	}

	if(!stack->now()->is("]"))
		_Error(stack, I18n::Err_Expect, "array end with ]");

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

	_Error(stack, I18n::Err_BadType);
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
			_Error(stack, I18n::Err_BadParam);

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
			_Error(stack, I18n::Err_Expect, ")");
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
			_Error(stack, I18n::Err_BadIndexValue);

		if(!stack->now()->is("]"))
			_Error(stack, I18n::Err_Expect, "]");

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
			_Error(stack, I18n::Err_ExprError);

		if(!stack->now()->is(")"))
			_Error(stack, I18n::Err_Expect, ")");
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
		_Error(stack, I18n::Err_ExprError);

	if(stack->now()->is(TokenType::OP) && stack->now()->is("?"))
	{
		stack->next();

		//取第一个值
		auto v1 = _Expr(stack);
		if(v1 == nullptr)
			_Error(stack, I18n::Err_ExprError);

		//:
		if(!stack->now()->is(":"))
			_Error(stack, I18n::Err_Expect, ":");
		stack->next();

		//取第二个值
		auto v2 = _Expr(stack);
		if(v2 == nullptr)
			_Error(stack, I18n::Err_ExprError);

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
			_Error(stack, I18n::Err_Expect, ")");

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
			_Error(stack, I18n::Err_BadIndexValue);

		if(!stack->move()->is("]"))
			_Error(stack, I18n::Err_Expect, "]");
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

