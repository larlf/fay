using fay.compiler.ast;
using fay.compiler.lexical;
using fay.lib;
using System;
using System.Collections.Generic;

namespace fay.compiler.syntax
{
	/// <summary>
	/// 语法分析器
	/// </summary>
	public class Parser
	{
		public AstNode Execute(List<Token> tokens, string filename)
		{
			TokenStack stack = new TokenStack(tokens);
			return this._file(stack, filename);
		}

		/// <summary>
		/// 生成左右操作的节点
		/// </summary>
		/// <param name="subExpr"></param>
		/// <param name="ops"></param>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode makeLeftRightOPNode(Func<TokenStack, AstNode> subExpr, string[] ops, TokenStack stack)
		{
			var leftNode = subExpr(stack);

			while (stack.Now.Is(TokenType.OP) && Array.IndexOf(ops, stack.Now.Text) > -1)
			{
				var node = new AstLeftRightOP(stack.Now.Text);
				node.AddChildNode(leftNode);

				//读取第二项
				stack.Next();
				var rightNode = subExpr(stack);
				node.AddChildNode(rightNode);

				leftNode = node;
			}

			return leftNode;
		}

		/// <summary>
		/// 生成布尔操作的节点
		/// </summary>
		/// <param name="subExpr"></param>
		/// <param name="ops"></param>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode makeBoolOPNode(Func<TokenStack, AstNode> subExpr, string[] ops, TokenStack stack)
		{
			var leftNode = subExpr(stack);

			while (stack.Now.Is(TokenType.OP) && Array.IndexOf(ops, stack.Now.Text) > -1)
			{
				var node = new AstBoolOP(stack.Now.Text);
				node.AddChildNode(leftNode);

				//读取第二项
				stack.Next();
				var rightNode = subExpr(stack);
				node.AddChildNode(rightNode);

				leftNode = node;
			}

			return leftNode;
		}

		/// <summary>
		/// 表达式中的各项基本值
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode exprPrimary(TokenStack stack)
		{
			if (stack.Now.Is(TokenType.Number))
				return new AstNumber(stack.Move().Text);
			else if (stack.Now.Is(TokenType.String))
				return new AstString(stack.Move().Text);
			else if (stack.Now.Is(TokenType.Char))
				return new AstByte(stack.Move());
			else if (stack.Now.Is(TokenType.ID))
			{
				//如果下面是括号，那应该是个调用
				if (stack.After.Text == "(")
					return call(stack);

				//不然就是一个ID
				return new AstID(stack.Move().Text);
			}
			else if (stack.Now.Is("("))  //处理括号里的内容
			{
				stack.Next();
				var expr1 = this.expr(stack);
				if (stack.Now.Is(")"))
					stack.Next();
				else this.error(stack, "Expect )");

				return new AstBrackets(expr1);
			}

			return null;
		}

		/// <summary>
		/// 括号和下标处理
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode exprBracket(TokenStack stack)
		{
			var leftNode = exprPrimary(stack);
			while (leftNode != null && stack.Now.Is("["))
			{
				stack.Next();
				var n1 = expr(stack);

				if (stack.Now.Is("]"))
					stack.Next();
				else
					this.error(stack, "Expect ]");

				leftNode = new AstBracket(leftNode, n1);
			}

			return leftNode;
		}

		/// <summary>
		/// 处理后置操作符
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode exprPost(TokenStack stack)
		{
			var node = exprBracket(stack);

			if (stack.Now.Is(TokenType.OP) &&
				(stack.Now.Text == "++" || stack.Now.Text == "--"))
			{
				node = new AstPostOP(stack.Now.Text, node);
				stack.Next();
			}

			return node;
		}

		/// <summary>
		/// 处理前置操作符
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode exprPre(TokenStack stack)
		{
			if (stack.Now.Is(TokenType.OP) &&
				(stack.Now.Is("++") || stack.Now.Is("--") || stack.Now.Is("!") || stack.Now.Is("~")))
			{
				var node = new AstPreOP(stack.Now.Text);
				stack.Next();
				var n1 = exprPost(stack);
				node.AddChildNode(n1);
				return node;
			}

			return exprPost(stack);
		}



		/// <summary>
		/// 处理乘除表达式
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode exprMulDiv(TokenStack stack)
		{
			return this.makeLeftRightOPNode(this.exprPre, new string[] { "*", "/", "%" }, stack);
		}

		/// <summary>
		/// 处理加减表达式
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode exprAddSub(TokenStack stack)
		{
			return this.makeLeftRightOPNode(this.exprMulDiv, new string[] { "+", "-" }, stack);
		}

		/// <summary>
		/// 左移和右移
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode exprLeftRightMove(TokenStack stack)
		{
			return this.makeLeftRightOPNode(this.exprAddSub, new string[] { ">>", "<<" }, stack);
		}

		/// <summary>
		/// 逻辑操作符
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode exprBool(TokenStack stack)
		{
			return this.makeBoolOPNode(this.exprLeftRightMove, new string[] { ">", "<", "==", ">=", "<=" }, stack);
		}

		private AstNode expr(TokenStack stack)
		{
			return exprBool(stack);
		}

		private AstNode addressExprItem(TokenStack stack)
		{
			if (stack.Now.Is(TokenType.ID))
			{
				//如果下面是括号，那应该是个调用
				if (stack.After.Text == "(")
					return call(stack);

				//不然就是一个ID
				return new AstID(stack.Move().Text);
			}
			else if (stack.Now.Is("("))  //处理括号里的内容
			{
				stack.Next();
				var expr1 = this.expr(stack);
				if (stack.Now.Is(")"))
					stack.Next();
				else this.error(stack, "Expect )");

				return new AstBrackets(expr1);
			}

			return null;
		}

		private AstNode addressExprBracket(TokenStack stack)
		{
			var leftNode = addressExprItem(stack);
			while (leftNode != null && stack.Now.Is("["))
			{
				stack.Next();
				var n1 = expr(stack);

				if (stack.Now.Is("]"))
					stack.Next();
				else
					this.error(stack, "Expect ]");

				leftNode = new AstBracket(leftNode, n1);
			}

			return leftNode;
		}

		private AstNode addressExpr(TokenStack s)
		{
			return addressExprBracket(s);
		}

		/// <summary>
		/// 数组
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode array(TokenStack stack)
		{
			if (stack.Now.Is("["))
				stack.Next();
			else
				this.error(stack, "Expect array start with [");

			var node = new AstArray();
			while (stack.Now != null && !stack.Now.Is("]"))
			{
				var expr1 = this.expr(stack);
				node.AddChildNode(expr1);

				if (stack.Now.Is(","))
					stack.Next();
			}

			if (stack.Now.Is("]"))
				stack.Next();
			else
				this.error(stack, "Expect array end with ]");

			return node;
		}

		/// <summary>
		/// 数据类型
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode varType(TokenStack stack)
		{
			if (stack.Now.Is(TokenType.BasicType) || stack.Now.Is(TokenType.ID))
			{
				string typeName = stack.Now.Text;
				stack.Next();

				bool isArray = false;
				if (stack.Now.Is("[") && stack.After.Is("]"))
				{
					stack.Next();
					stack.Next();
					isArray = true;
				}

				return new AstType(typeName, isArray);
			}
			else
				this.error(stack, "Bad type");

			return null;
		}

		/// <summary>
		/// 参数定义
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode paramDefine(TokenStack stack)
		{
			AstParamDefine node = null;

			if (stack.Now.Is(TokenType.ID) && stack.After.Is(TokenType.Colon))
			{
				node = new AstParamDefine(stack.Now.Text);
				stack.Next();
				stack.Next();

				if (stack.Now.Is(TokenType.BasicType))
				{
					node.AddChildNode(new AstType(stack.Now.Text));
					stack.Next();
				}
				else if (stack.Now.Is(TokenType.ID))
				{
					node.AddChildNode(new AstType(stack.Now.Text));
					stack.Next();
				}
				else
					this.error(stack, "Bad param define");
			}

			return node;
		}

		/// <summary>
		/// 参数定义列表
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode paramDefineList(TokenStack stack)
		{
			AstParamDefineList node = new AstParamDefineList();

			while (true)
			{
				AstNode paramDefine = this.paramDefine(stack);
				if (paramDefine == null)
					break;

				node.AddChildNode(paramDefine);

				if (stack.Now.Is(TokenType.Comma))
					stack.Next();
				else
					break;
			}

			return node;
		}

		/// <summary>
		/// 一项参数
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode param(TokenStack stack)
		{
			var node = expr(stack);
			return node;
		}

		/// <summary>
		/// 参数列表
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode paramList(TokenStack stack)
		{
			var node = new AstParams();
			var it = param(stack);
			while (it != null)
			{
				node.AddChildNode(it);
				if (stack.Now.Is(TokenType.Comma))
				{
					stack.Next();
					it = param(stack);
				}
				else it = null;
			}

			return node;
		}

		/// <summary>
		/// 函数调用
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode call(TokenStack stack)
		{
			if (!stack.Now.Is(TokenType.ID))
				this.error(stack, "call");

			var node = new AstCall(stack.Move().Text);

			//检查调用开始
			if (stack.Now.Text != "(")
				this.error(stack, "call");

			//取参数
			stack.Next();
			var param = paramList(stack);
			node.AddChildNode(param);

			//检查调用结束
			if (stack.Now.Text != ")")
				this.error(stack, "call");

			stack.Next();
			return node;
		}

		/// <summary>
		/// 语句
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode _stmt(TokenStack stack)
		{
			int pos = stack.Pos;
			AstNode node = null;

			//如果是{开头，认为是一个语句块
			if (stack.Now.Is(TokenType.LeftBrace))
				node = this._block(stack);
			else if (stack.Now.Is(TokenType.Var))
				node = _var(stack);
			else if (stack.Now.Is(TokenType.Return))
				node = _return(stack);
			else if (stack.Now.Is(TokenType.If))
				node = _if(stack);
			else if (stack.Now.Is(TokenType.For))
				node = _for(stack);
			else
			{
				Token nextToken = stack.FindNextToken(new TokenType[] { TokenType.Assign, TokenType.Semicolon });
				if (nextToken.Type == TokenType.Assign)
					node = this._assign(stack);
				else
					node = this.expr(stack);
			}

			if (node == null)
				this.error(stack, "Unknow stmt");

			return node;
		}

		/// <summary>
		/// 赋值
		/// </summary>
		/// <param name="stack"></param>
		private AstNode _assign(TokenStack stack)
		{
			AstNode rightValue = null;  //变量的值

			//左边的变量
			AstNode leftValue = this.addressExpr(stack);
			if (leftValue == null)
				this.error(stack, "Cannot find left var");

			//是否有赋值
			if (stack.Now.Is(TokenType.Assign))
			{
				stack.Next();

				rightValue = this.expr(stack);
				if (rightValue == null)
					this.error(stack, "Bad var value");
			}

			if (!stack.Now.Is(TokenType.Semicolon))
				this.error(stack, "Expect ;");
			stack.Next();

			return new AstAssign(leftValue, rightValue);
		}

		private AstNode _var(TokenStack stack)
		{
			if (stack.Now.Is(TokenType.Var))
				stack.Next();
			else
				this.error(stack, "Expect var");

			AstNode node;
			string varName = null;  //变量名
			AstNode varType = null;  //变量的类型
			AstNode varValue = null;  //变量的值

			if (!stack.Now.Is(TokenType.ID))
				this.error(stack, "Cannot find var name");
			varName = stack.Now.Text;
			stack.Next();

			//处理数据类型
			if (stack.Now.Is(TokenType.Colon))
			{
				stack.Next();
				varType = this.varType(stack);
			}

			//是否有赋值
			if (stack.Now.Is(TokenType.Assign))
			{
				stack.Next();

				if (stack.Now.Is("["))  //数组
					varValue = this.array(stack);
				else
					varValue = this.expr(stack);

				if (varValue == null)
					this.error(stack, "Bad var value");
			}

			if (stack.Now.Is(TokenType.Semicolon))
				stack.Next();
			else
				this.error(stack, "Expect ;");

			node = new AstVar(varName, varType, varValue);
			return node;
		}

		private AstNode _if(TokenStack stack)
		{
			AstIf node = new AstIf();

			//if
			if (!stack.Now.Is(TokenType.If))
				this.error(stack, "Expect if");

			while (stack.Now.Is(TokenType.If) || stack.Now.Is(TokenType.ElseIf))
			{
				stack.Next();

				AstNode cond = new AstCondition();

				//处理条件
				{
					if (stack.Now.Text == "(")
						stack.Next();
					else
						this.error(stack, "Expect (");

					AstNode condition = this.expr(stack);
					if (condition == null)
						this.error(stack, "Cannot find condition for if");
					else
						cond.AddChildNode(condition);

					if (stack.Now.Text == ")")
						stack.Next();
					else
						this.error(stack, "Expect )");
				}

				AstNode action = this._stmt(stack);
				if (action == null)
					this.error(stack, "Cannot find action for if");
				else
					cond.AddChildNode(action);

				node.AddChildNode(cond);
			}

			//处理最后的else
			if (stack.Now.Is(TokenType.Else))
			{
				stack.Next();

				AstNode cond = new AstCondition();
				AstNode action = this._stmt(stack);
				if (action == null)
					this.error(stack, "Cannot find action for if");
				else
					cond.AddChildNode(action);

				node.AddChildNode(cond);
			}

			return node;
		}

		private AstNode _for(TokenStack stack)
		{
			if (stack.Now.Is(TokenType.For))
				stack.Next();
			else
				this.error(stack, "Expect for");

			AstNode stmt1 = null;
			AstNode stmt2 = null;
			AstNode stmt3 = null;
			AstNode block = null;

			if (stack.Now.Text == "(")
				stack.Next();
			else
				this.error(stack, "Except (");

			stmt1 = this._stmt(stack);

			//if (stack.Now.Text == ";")
			//	stack.Move();
			//else
			//	this.error(stack, "Except ;");

			stmt2 = this.expr(stack);

			if (stack.Now.Text == ";")
				stack.Next();
			else
				this.error(stack, "Except ;");

			stmt3 = this._stmt(stack);

			if (stack.Now.Text == ")")
				stack.Next();
			else
				this.error(stack, "Except )");

			block = this._stmt(stack);

			AstFor node = new AstFor();
			node.AddChildNode(stmt1);
			node.AddChildNode(stmt2);
			node.AddChildNode(stmt3);
			node.AddChildNode(block);

			return node;
		}

		private AstNode _return(TokenStack stack)
		{
			AstNode node = new AstReturn();
			stack.Next();

			//是否有参数
			if (stack.Now.Is(TokenType.Semicolon))
				stack.Next();
			else
			{
				AstNode subNode = this.expr(stack);
				if (subNode == null)
					this.error(stack, "Bad return value");

				node.AddChildNode(subNode);

				//;
				if (!stack.Now.Is(TokenType.Semicolon))
					this.error(stack, "Expect ;");
				stack.Next();
			}

			return node;
		}

		/// <summary>
		/// 语句块
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode _block(TokenStack stack)
		{
			AstBlock node = new AstBlock();

			if (!stack.Now.Is(TokenType.LeftBrace))
				this.error(stack, "Expect {");
			stack.Next();

			while (true)
			{
				//}
				if (stack.Now.Is(TokenType.RightBrace))
					break;

				//跳过空语句
				while (stack.Now.Is(TokenType.Semicolon))
				{
					stack.Next();
				}

				var subNode = this._stmt(stack);
				if (subNode != null)
					node.AddChildNode(subNode);
				else
					break;

				//跳过空语句
				while (stack.Now.Is(TokenType.Semicolon))
				{
					stack.Next();
				}
			}

			if (!stack.Now.Is(TokenType.RightBrace))
				this.error(stack, "Expect }");
			stack.Next();

			return node;
		}

		/// <summary>
		/// 字段
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode _field(TokenStack stack)
		{
			AstField node = null;

			//处理所有的描述关键字
			List<string> descWords = new List<string>();
			while (stack.Now.Is(TokenType.DescSymbol))
			{
				descWords.Add(stack.Now.Text);
				stack.Next();
			}

			//var
			if (!stack.Now.Is(TokenType.Var))
				this.error(stack, "Unknow desc symbol");

			stack.Next();

			//name
			if (!stack.Now.Is(TokenType.ID))
				this.error(stack, "Cannot find var name");

			node = new AstField(stack.Now.Text);
			stack.Next();

			//处理类型，也可能没有，这样的话需要由数据进行推断
			if (stack.Now.Is(TokenType.Colon))
			{
				stack.Next();

				if (stack.Now.Is(TokenType.BasicType))
					node.SetBasicType(stack.Now.Text);
				else if (stack.Now.Is(TokenType.ID))
					node.SetObjectType(stack.Now.Text);
				else
					this.error(stack, "Bad var type");

				stack.Next();
			}

			//=
			if (stack.Now.Is(TokenType.Assign))
			{
				stack.Next();

				AstNode value = this.expr(stack);
				node.AddChildNode(value);
			}

			//;
			if (!stack.Now.Is(TokenType.Semicolon))
				this.error(stack, "Lost ;");

			stack.Next();

			return node;
		}

		/// <summary>
		/// 方法
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode _func(TokenStack stack)
		{
			AstFun node = null;

			//所有的描述符
			List<string> descList = new List<string>();
			while (stack.Now.Is(TokenType.DescSymbol))
			{
				descList.Add(stack.Now.Text);
				stack.Next();
			}

			//func
			if (stack.Now.Is(TokenType.Function))
				stack.Next();
			else
				this.error(stack, "Need fun keyword");

			//方法名
			if (stack.Now.Is(TokenType.ID) || stack.Now.Is(TokenType.SystemName))
				node = new AstFun(stack.Move().Text);
			else
				this.error(stack, "Bad function name");

			if (stack.Now == null || stack.Now.Text != "(")
				this.error(stack, "Expect (");
			stack.Next();

			var paramsDefine = this.paramDefineList(stack);
			node.AddChildNode(paramsDefine);

			if (stack.Now == null || stack.Now.Text != ")")
				this.error(stack, "Expect )");
			stack.Next();

			//返回值
			if (stack.Now.Is(TokenType.Colon))
			{
				stack.Next();

				if (stack.Now.Is(TokenType.BasicType))
				{
					stack.Next();
				}
				else if (stack.Now.Is(TokenType.ID))
				{
					stack.Next();
				}
				else
					this.error(stack, "Error return value");
			}

			var blockNode = this._block(stack);
			if (blockNode == null)
				this.error(stack, "Cannot find fun body");

			node.AddChildNode(blockNode);

			return node;
		}

		/// <summary>
		/// 类定义
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode _class(TokenStack stack)
		{
			AstClass node = null;

			//处理所有的描述关键字
			List<string> descWords = new List<string>();
			while (stack.Now.Is(TokenType.DescSymbol))
			{
				descWords.Add(stack.Now.Text);
				stack.Next();
			}

			//class
			if (!stack.Now.Is(TokenType.Class))
				this.error(stack, "Cannot find keyword : class");
			stack.Next();

			//className
			if (!stack.Now.Is(TokenType.ID))
				this.error(stack, "Bad class name");

			//生成class节点
			node = new AstClass(stack.Now.Text);
			stack.Next();

			//{
			if (!stack.Now.Is(TokenType.LeftBrace))
				this.error(stack, "Cannot find start brace for class");
			stack.Next();

			TokenType[] keyTokenTypes = new TokenType[]
			{
				TokenType.Var, TokenType.Function, TokenType.Class, TokenType.Interface
			};

			while (true)
			{
				Token nextToken = stack.FindNextToken(keyTokenTypes);
				if (nextToken == null)
					break;

				AstNode subNode = null;
				switch (nextToken.Type)
				{
					case TokenType.Var:
						subNode = this._field(stack);
						break;
					case TokenType.Function:
						subNode = this._func(stack);
						break;
				}

				if (subNode != null)
					node.AddChildNode(subNode);
				else
					break;
			}

			if (!stack.Now.Is(TokenType.RightBrace))
				this.error(stack, "Cannot find end brace for class");
			stack.Next();

			return node;
		}

		/// <summary>
		/// 角析using语句
		/// </summary>
		/// <param name="stack"></param>
		/// <returns></returns>
		private AstNode _using(TokenStack stack)
		{
			AstNode node = null;

			if (stack.Now.Is(TokenType.Using))
			{
				stack.Next();

				if (!stack.Now.Is(TokenType.ID))
					this.error(stack, "using format error");

				node = new AstUsing(stack.Now.Text);
				stack.Next();

				if (!stack.Now.Is(TokenType.Semicolon))
					this.error(stack, "using not stop with ;");

				stack.Next();
			}

			return node;
		}

		private AstNode _package(TokenStack stack)
		{
			AstNode node = null;

			if (stack.Now.Is(TokenType.Package))
			{
				stack.Next();

				if (!stack.Now.Is(TokenType.ID))
					this.error(stack, "package format error");

				node = new AstPackage(stack.Now.Text);
				stack.Next();

				if (!stack.Now.Is(TokenType.Semicolon))
					this.error(stack, "package not stop with ;");

				stack.Next();
			}

			return node;
		}

		private AstNode _file(TokenStack stack, string filename)
		{
			AstFile ast = new AstFile(filename);

			TokenType[] keyTokenTypes = new TokenType[]
			{
				TokenType.Using, TokenType.Package, TokenType.Class
			};

			while (true)
			{
				Token nextKeyToken = stack.FindNextToken(keyTokenTypes);
				if (nextKeyToken == null)
					break;

				AstNode node = null;
				switch (nextKeyToken.Type)
				{
					case TokenType.Using:
						node = this._using(stack);
						break;
					case TokenType.Package:
						node = this._package(stack);
						break;
					case TokenType.Class:
						node = this._class(stack);
						break;
				}

				//找到就记录，啥都没找到就返回
				if (node != null)
					ast.AddChildNode(node);
				else
					break;
			}

			return ast;
		}

		private void error(TokenStack stack, string msg)
		{
			throw new Exception(msg + " line:" + stack.Now.Line);
		}
	}
}
