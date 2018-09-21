using fay.compiler.lexical;
using fay.lib;
using Mirage.Sys;
using System;
using System.Collections.Generic;
using System.Text;

namespace fay.compiler.syntax
{

	/// <summary>
	/// 用于进行语法解新的堆栈
	/// </summary>
	public class TokenStack
	{
		public static Token EndToken = new Token(TokenType.None, "", -1, -1);

		private List<Token> _tokens;
		private int _pos = 0;
		private int _sign = -1;

		public TokenStack(List<Token> tokens)
		{
			this._tokens = tokens;
		}

		/// <summary>
		/// 当前的位置
		/// </summary>
		public int Pos
		{
			get { return this._pos; }
		}

		/// <summary>
		/// 返回当前的Token
		/// </summary>
		public Token Now
		{
			get
			{
				if (this._pos < this._tokens.Count)
					return this._tokens[this._pos];

				return EndToken;
			}
		}

		/// <summary>
		/// 下一项
		/// </summary>
		public Token After
		{
			get
			{
				if (this._pos < this._tokens.Count - 1)
					return this._tokens[this._pos + 1];

				return EndToken;
			}
		}

		/// <summary>
		/// 上一项
		/// </summary>
		public Token Before
		{
			get
			{
				if (this._pos < this._tokens.Count + 1)
					return this._tokens[this._pos - 1];

				return EndToken;
			}
		}

		/// <summary>
		/// 返回当前的Token，并移动动下一个
		/// </summary>
		public Token Move()
		{
			Token token = this.Now;
			this.Next();

			return token;
		}

		/// <summary>
		/// 移动并返回下一个Token
		/// </summary>
		public Token Next()
		{
			if (this._pos < this._tokens.Count)
				this._pos++;

			//分析的时候可以打印这个看看
			//Log.Debug("Move : " + this.Now);

			//跳过注释
			if (this.Now != null && this.Now.Is(TokenType.Comment))
				this.Next();

			return this.Now;
		}

		public void Sign()
		{
			this._sign = this._pos;
		}

		/// <summary>
		/// 回退
		/// </summary>
		public void Back()
		{
			if (this._sign >= 0)
				this._pos = this._sign;
		}

		/// <summary>
		/// 查找符合要求的下一个关键字
		/// </summary>
		/// <param name="types"></param>
		/// <returns></returns>
		public Token FindNextToken(TokenType[] types)
		{
			for (var i = this._pos; i < this._tokens.Count; ++i)
			{
				for (var j = 0; j < types.Length; ++j)
				{
					if (this._tokens[i].Type == types[j])
						return this._tokens[i];
				}
			}

			return null;
		}
	}
}
