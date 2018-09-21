using fay.lib;

namespace fay.compiler.lexical.Rules
{
	public class WordsTokenRule : BasicTokenRule
	{
		private string[] words;

		public WordsTokenRule(LexMode mode, TokenType type, string[] words) : base(mode, type)
		{
			this.words = words;
		}

		public override Token Match(char[] data, int pos, int linenum, int col)
		{
			foreach (var word in this.words)
			{
				//数据长度不足，跳过
				if (data.Length - pos < word.Length)
					continue;

				string str = new string(data, pos, word.Length);
				if (str.Equals(word))
				{
					return new Token(this.type, str, linenum, col);
				}
			}

			return null;
		}
	}
}
