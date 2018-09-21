using fay.lib;
using System.Text;

namespace fay.compiler.lexical.Rules
{
	public class NumberTokenRule : BasicTokenRule
	{
		public NumberTokenRule(LexMode mode, TokenType type) : base(mode, type)
		{
		}

		public override Token Match(char[] data, int pos, int linenum, int col)
		{
			//处理不符合要求的情况
			if ((data[pos] < '0' || data[pos] > '9') && data[pos] != '.')
				return null;
			if (data[pos] == '.' && (data[pos + 1] < '0' || data[pos + 1] > '9'))
				return null;

			bool hasDot = false;  //是否已经有.
			StringBuilder sb = new StringBuilder();
			while (pos < data.Length && ((data[pos] >= '0' && data[pos] <= '9') || data[pos] == '.'))
			{
				if (data[pos] == '.')
				{
					if (hasDot)
						return null;
					else
						hasDot = true;
				}

				sb.Append(data[pos++]);
			}

			//处理结尾的描述符
			if (data[pos] == 'l' || data[pos] == 'L' || data[pos] == 'd' || data[pos] == 'D')
				sb.Append(data[pos++]);

			if (sb.Length > 0)
			{
				return new Token(this.type, sb.ToString(), linenum, col);
			}

			return null;
		}
	}
}
