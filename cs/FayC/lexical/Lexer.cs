using fay.compiler.lexical.Rules;
using fay.lib;
using System;
using System.Collections.Generic;
using System.Text;

namespace fay.compiler.lexical
{
	/// <summary>
	/// 用于进行词法分析入口
	/// 不设计成静态的是为了以后可以多线程的处理代码
	/// </summary>
	public class Lexer
	{
		LexMode mode = LexMode.Program;
		List<ITokenRule> rules = new List<ITokenRule>();

		/// <summary>
		/// 生成词法分析器的实例
		/// </summary>
		/// <returns></returns>
		static public Lexer GetInstance()
		{
			Lexer lexer = new Lexer();

			//关键字
			lexer.AddRule(new WordTokenRule(LexMode.Program, TokenType.Class, "class"));
			lexer.AddRule(new WordTokenRule(LexMode.Program, TokenType.Interface, "interface"));
			lexer.AddRule(new WordTokenRule(LexMode.Program, TokenType.Var, "var"));
			lexer.AddRule(new WordTokenRule(LexMode.Program, TokenType.Function, "fun"));
			lexer.AddRule(new WordTokenRule(LexMode.Program, TokenType.If, "if"));
			lexer.AddRule(new WordTokenRule(LexMode.Program, TokenType.ElseIf, "elseif"));
			lexer.AddRule(new WordTokenRule(LexMode.Program, TokenType.Else, "else"));
			lexer.AddRule(new WordTokenRule(LexMode.Program, TokenType.For, "for"));
			lexer.AddRule(new WordTokenRule(LexMode.Program, TokenType.Using, "using"));
			lexer.AddRule(new WordTokenRule(LexMode.Program, TokenType.Package, "package"));
			lexer.AddRule(new WordTokenRule(LexMode.Program, TokenType.While, "while"));
			lexer.AddRule(new WordTokenRule(LexMode.Program, TokenType.Do, "do"));
			lexer.AddRule(new WordTokenRule(LexMode.Program, TokenType.Return, "return"));

			//分隔符
			lexer.AddRule(new SymbolTokenRule(LexMode.Program, ';'));
			lexer.AddRule(new SymbolTokenRule(LexMode.Program, '='));
			lexer.AddRule(new SymbolTokenRule(LexMode.Program, ':'));
			lexer.AddRule(new SymbolTokenRule(LexMode.Program, ','));
			lexer.AddRule(new SymbolTokenRule(LexMode.Program, '{'));
			lexer.AddRule(new SymbolTokenRule(LexMode.Program, '}'));

			//基本数据类型
			lexer.AddRule(new WordsTokenRule(LexMode.Program, TokenType.BasicType, new string[]
			{
				"void","byte","int","float","double","string","long"
			}));

			//对方法和类的说明符
			lexer.AddRule(new WordsTokenRule(LexMode.Program, TokenType.DescSymbol, new string[]
			{
				"public","private","static","inline"
			}));

			//特别的方法名
			lexer.AddRule(new WordsTokenRule(LexMode.Program, TokenType.SystemName, new string[]
			{
				".create", ".destory"
			}));

			//数字
			lexer.AddRule(new NumberTokenRule(LexMode.Program, TokenType.Number));

			//ID
			lexer.AddRule(new RegexTokenRule(LexMode.Program, TokenType.ID, @"[a-zA-Z_][a-zA-Z_0-9\.]*"));

			//不同级别的操作符
			lexer.AddRule(new WordsTokenRule(LexMode.Program, TokenType.OP, new string[]
			{
				"(",")","[","]",
				"++","--",">>","<<",
				"+", "-", "*", "/","%",
				"~","!",">","<","==",">=","<="
			}));

			//字符和字符串
			lexer.AddRule(new CharTokenRule());
			lexer.AddRule(new StringTokenRule());

			//注释
			lexer.AddRule(new WordTokenRule(LexMode.Program, TokenType.DoubleSlash, "//"));
			lexer.AddRule(new RegexTokenRule(LexMode.Comment, TokenType.Comment, @"[^\n\r]*"));

			return lexer;
		}

		/// <summary>
		/// 执行词法解析
		/// </summary>
		/// <param name="text"></param>
		/// <returns></returns>
		public List<Token> Execute(string text)
		{
			//输入和输出
			char[] chars = text.ToCharArray();
			List<Token> r = new List<Token>();

			int pos = 0;  //当前处理的位置
			int line = 1;  //当前所在的行
			int lineEnd = 0;  //最后一行结束的位置
			while (pos < chars.Length)
			{
				//跳过空白字符
				if (char.IsWhiteSpace(chars[pos]))
				{
					if (chars[pos] == '\n')
					{
						line++;
						lineEnd = pos;
					}

					pos++;
					continue;
				}

				//计算所在列
				int col = pos - lineEnd;

				//生成当前位置的token列表
				List<Token> tokens = new List<Token>();
				foreach (var it in this.rules)
				{
					if (it.Mode == this.mode)
					{
						Token t = it.Match(chars, pos, line, col);
						if (t != null)
						{
							tokens.Add(t);
						}
					}
				}

				//如果找到了，就向下进行，不然就报错
				if (tokens.Count > 0)
				{
					Token t = tokens[0];

					//有多个结果的时候，取一个最长的
					//长度相同取优先级最高的
					if (tokens.Count > 1)
					{
						foreach (var it in tokens)
						{
							if (it.Length > t.Length)
							{
								t = it;
							}
						}
					}

					//对这部分内容进行其它分析
					for (var i = 0; i < t.Length; ++i)
					{
						//检查换行符，对行数进行计数
						if (chars[pos + i] == '\n')
						{
							line++;
							lineEnd = pos + i;
						}
					}

					//修改位置指针
					pos += t.Length;

					//添加取返回结果
					t = changeMode(t);
					if (t != null) r.Add(t);

				}
				else
				{
					
					StringBuilder sb = new StringBuilder("Lexer error at line:" + line + " column:" + col + "\n");

					//取当前行的内容
					int strPos = lineEnd + 1;
					while (strPos < chars.Length && chars[strPos] != '\n' && chars[strPos] != '\r')
					{
						if (chars[strPos] == '\t')
							sb.Append(' ');
						else
							sb.Append(chars[strPos]);

						strPos++;
					}

					//显示位置
					sb.Append("\n");
					for (var i = 0; i < col - 1; ++i)
					{
						sb.Append(' ');
					}
					sb.Append('^');

					//生成错误信息
					throw new LexerException(sb.ToString());
				}
			}

			return r;
		}

		/// <summary>
		/// 如果有需要对模式进行变量的操作，在这里进行处理
		/// </summary>
		/// <param name="t"></param>
		/// <returns>如果Token不需要记录，这里可以返回空</returns>
		private Token changeMode(Token t)
		{
			//如果进入到特别的内容中，处理下mode转换
			switch (t.Type)
			{
				case TokenType.DoubleSlash:
					this.mode = LexMode.Comment;
					return null;
				case TokenType.Comment:
					this.mode = LexMode.Program;
					return t;
			}

			return t;
		}

		/// <summary>
		/// 添加规则，注意，添加顺序会影响优先级
		/// </summary>
		/// <param name="rule"></param>
		public void AddRule(ITokenRule rule)
		{
			this.rules.Add(rule);
		}
	}

	public class LexerException : Exception
	{
		public LexerException(string msg) : base(msg)
		{
		}
	}
}
