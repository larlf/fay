#pragma once

#include <fay_ast.h>
#include <fay_token_stack.h>

namespace fay
{
	//语法解析中的异常
	class ParseException : public std::exception
	{
	public:
		//stack : 当前正在处理的TokenStack
		//msg : 错误信息
		ParseException(TokenStack* stack, const std::string &msg)
			: std::exception::exception((msg + "\n" + stack->now()->toString()).c_str()) {}
	};

	//语法解析器
	class Parser
	{
	private:
		//生成左右双向式操作的节点
		static PTR(AstNode) _MakeLeftRightOPNode(std::function<PTR(AstNode)(TokenStack*)> subExpr, const std::vector<std::string> &ops, TokenStack* stack);
		//生成布尔操作的节点
		static PTR(AstNode) _MakeBoolOPNode(std::function<PTR(AstNode)(TokenStack*)> subExpr, std::vector<std::string> ops, TokenStack* stack);

		//文件节点
		static PTR(AstNode) _File(TokenStack* stack, const std::string &filename);
		//Using语句
		static PTR(AstNode) _Using(TokenStack* stack);
		//Package语句
		static PTR(AstNode) _Package(TokenStack* stack);
		//调用语句
		static PTR(AstNode) _Call(TokenStack* stack);

		//表达式中的各项基本值
		static PTR(AstNode) _ExprPrimary(TokenStack* stack);
		//下标
		static PTR(AstNode) _ExprBracket(TokenStack* stack);
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