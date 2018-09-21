using fay.lib;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;
using System;
using System.Collections.Generic;
using System.Text;

namespace fay.compiler.lexical
{
	/// <summary>
	/// 词法解析后产生的Token
	/// </summary>
	public class Token
	{
		private string text;
		private TokenType type;
		private int line;
		private int col;

		/// <summary>
		/// 文字
		/// </summary>
		public string Text => this.text;
		/// <summary>
		/// 类型
		/// </summary>
		public TokenType Type => this.type;
		/// <summary>
		/// 所在行
		/// </summary>
		public int Line => this.line;
		/// <summary>
		/// 所在列
		/// </summary>
		public int Col => this.col;
		/// <summary>
		/// 长度
		/// </summary>
		public int Length => string.IsNullOrEmpty(this.text) ? 0 : this.text.Length;

		public Token(TokenType type, string text, int line, int col)
		{
			this.type = type;
			this.text = text;
			this.line = line;
			this.col = col;
		}

		public Token(TokenType type, string text)
		{
			this.type = type;
			this.text = text;
		}

		/// <summary>
		/// 是否是指定类型
		/// </summary>
		/// <param name="type"></param>
		/// <returns></returns>
		public bool Is(TokenType type)
		{
			if (this.type == type)
				return true;

			return false;
		}

		public bool Is(string str)
		{
			if (this.text == str)
				return true;

			return false;
		}

		public override string ToString()
		{
			StringBuilder sb = new StringBuilder();
			//sb.Append(base.ToString());
			//sb.Append(JsonConvert.SerializeObject(this));
			sb.Append("Token(")
				.Append(this.Type).Append("  ")
				.Append(this.Text).Append("  ")
				.Append(this.line).Append(",")
				.Append(this.col).Append(")");

			return sb.ToString();
		}
	}

}
