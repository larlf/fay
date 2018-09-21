using fay.lib;
using System.Text.RegularExpressions;

namespace fay.compiler.lexical.Rules
{
	/// <summary>
	/// 正则匹配
	/// </summary>
	public class RegexTokenRule : BasicTokenRule
	{
		private Regex regex;

		public RegexTokenRule(LexMode mode, TokenType type, string regexStr)
			: base(mode, type)
		{
			this.regex = new Regex("^" + regexStr + "$", RegexOptions.Compiled);
		}

		public override Token Match(char[] data, int pos, int linenum, int col)
		{
			int len = 1;
			while ((pos + len) < data.Length && this.regex.IsMatch(new string(data, pos, len)))
			{
				len++;
			}

			len--;
			if (len > 0)
			{
				return new Token(this.type, new string(data, pos, len), linenum, col);
			}

			return null;
		}
	}
}
