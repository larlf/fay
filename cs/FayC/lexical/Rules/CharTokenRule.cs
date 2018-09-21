using fay.lib;

namespace fay.compiler.lexical.Rules
{
	public class CharTokenRule : BasicTokenRule
	{
		public CharTokenRule() : base(LexMode.Program, TokenType.Char)
		{
		}

		public override Token Match(char[] data, int pos, int linenum, int col)
		{
			if (data[pos] == '\'')
			{
				if (pos < data.Length - 2)
				{
					if (data[pos + 1] == '\\')
					{
						if (pos < data.Length - 3 && data[pos + 3] == '\'')
						{
							return new Token(this.type, new string(data, pos, 4), linenum, col);
						}
					}
					else
					{
						if (data[pos + 2] == '\'')
						{
							return new Token(this.type, new string(data, pos, 3), linenum, col);
						}
					}
				}
			}

			return null;
		}
	}
}
