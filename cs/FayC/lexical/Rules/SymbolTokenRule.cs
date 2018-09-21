using fay.lib;

namespace fay.compiler.lexical.Rules
{
	public class SymbolTokenRule : BasicTokenRule
	{
		private char value;

		public SymbolTokenRule(LexMode mode, char c) : base(mode, (TokenType)c)
		{
			this.value = c;
		}

		public override Token Match(char[] data, int pos, int linenum, int col)
		{
			if (data[pos] == this.value)
			{
				return new Token(this.type, new string(data, pos, 1), linenum, col);
			}

			return null;
		}
	}
}
