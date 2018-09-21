using fay.lib;

namespace fay.compiler.lexical.Rules
{
	/// <summary>
	/// 单词匹配
	/// </summary>
	public class WordTokenRule : BasicTokenRule
	{
		private string word;

		public WordTokenRule(LexMode mode, TokenType type, string str)
			: base(mode, type)
		{
			this.word = str;
		}

		public override Token Match(char[] data, int pos, int linenum, int col)
		{
			if ((pos + this.word.Length) < data.Length)
			{
				string str = new string(data, pos, this.word.Length);
				if (str.Equals(this.word))
				{
					return new Token(this.type, str, linenum, col);
				}
			}

			return null;
		}
	}
}
