#pragma once

#include <fay_ast.h>
#include <fay_token_stack.h>
#include <fay_i18n.h>

namespace fay
{
	//语法解析中的异常
	class ParseException : public FayCompileException
	{
	public:
		//stack : 当前正在处理的TokenStack
		//key : 错误信息的国际化信息
		template<typename... Params>
		ParseException(TokenStack* stack, const std::string &key, Params... args)
			: FayCompileException((I18N::Get(key, args...)))
		{
			PTR(Token) token = stack->now();
			if(token)
			{
				this->_file = token->file();
				this->_line = token->line();
				this->_col = token->col();
			}
		}
	};

	//语法解析器
	class Parser
	{
	private:
		//生成左右双向式操作的节点
		static PTR(AstNode) _MakeLeftRightOPNode(
			std::function<PTR(AstNode)(TokenStack*)> subExpr,
			const std::vector<std::string> &ops,
			TokenStack* stack,
			std::function<PTR(AstNode)(PTR(Token))> nodeCreater);
		//生成布尔操作的节点
		static PTR(AstNode) _MakeBoolOPNode(std::function<PTR(AstNode)(TokenStack*)> subExpr, std::vector<std::string> ops, TokenStack* stack);

		//Using语句
		static PTR(AstNode) _Using(TokenStack* stack);
		//Package语句
		static PTR(AstNode) _Package(TokenStack* stack);
		static PTR(AstNode) _Class(TokenStack* stack);
		static PTR(AstNode) _Field(TokenStack* stack);
		static PTR(AstNode) _Fun(TokenStack* stack);

		//支持的各种语句
		static PTR(AstNode) _Stmt(TokenStack* stack);
		static PTR(AstNode) _StmtLabel(TokenStack* stack);
		static PTR(AstNode) _StmtGoto(TokenStack* stack);
		static PTR(AstNode) _StmtBlock(TokenStack* stack);
		static PTR(AstNode) _StmtVar(TokenStack* stack);
		static PTR(AstNode) _StmtAssign(TokenStack* stack);
		static PTR(AstNode) _StmtIf(TokenStack* stack);
		static PTR(AstNode) _StmtFor(TokenStack* stack);
		static PTR(AstNode) _StmtReturn(TokenStack* stack);

		//数组
		static PTR(AstNode) _Array(TokenStack* stack);
		//类型定义
		static PTR(AstNode) _Type(TokenStack* stack);

		//参数定义
		static PTR(AstNode) _ParamDef(TokenStack* stack);
		//参数定义 列表
		static PTR(AstNode) _ParamDefList(TokenStack* stack);
		//调用方法时的参数列表
		static PTR(AstNode) _ParamList(TokenStack* stack);
		//调用语句
		static PTR(AstNode) _Call(TokenStack* stack);


		//表达式中的各项基本值
		static PTR(AstNode) _ExprPrimary(TokenStack* stack);
		//下标
		static PTR(AstNode) _ExprBracket(TokenStack* stack);
		//取负数的操作
		static PTR(AstNode) _ExprMinus(TokenStack* stack);
		//后置操作符
		static PTR(AstNode) _ExprPost(TokenStack* stack);
		//前置操作符
		static PTR(AstNode) _ExprPre(TokenStack* stack);
		//乘除表达式
		static PTR(AstNode) _ExprMulDiv(TokenStack* stack);
		//加减表达式
		static PTR(AstNode) _ExprAddSub(TokenStack* stack);
		//左移和右移
		static PTR(AstNode) _ExprLeftRightMove(TokenStack* stack);
		//逻辑操作符
		static PTR(AstNode) _ExprBool(TokenStack* stack);
		//赋值运算符
		static PTR(AstNode) _ExprAssign(TokenStack* stack);
		//表达式
		static PTR(AstNode) _Expr(TokenStack* stack);

		//寻址表达式中的元素
		static PTR(AstNode) _AddrExprItem(TokenStack* stack);
		//寻址表达式中的下标
		static PTR(AstNode) _AddrExprBracket(TokenStack* stack);
		//寻址表达式，用于接收赋值
		static PTR(AstNode) _AddrExpr(TokenStack* stack);

	public:
		//进行语法解析的入口
		static PTR(AstNode) Parse(PTR(std::vector<PTR(Token)>) tokens, const std::string &filename);
	};
}