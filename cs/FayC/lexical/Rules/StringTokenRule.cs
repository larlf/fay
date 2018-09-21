using fay.lib;

namespace fay.compiler.lexical.Rules
{
	public class StringTokenRule : BasicTokenRule
	{
		public StringTokenRule() : base(LexMode.Program, TokenType.String)
		{
		}

		public override Token Match(char[] data, int pos, int linenum, int col)
		{
			if(data[pos]=='"')
			{
				for(var i=pos+1; i<data.Length; ++i)
				{
					//不能换行
					if (data[i] == '\n' || data[i] == '\r')
						break;

					//字符串结束
					if(data[i]=='"' && data[i-1]!='\\')
					{
						//返回字符串的token
						string str = new string(data, pos, i - pos + 1);
						return new Token(this.type, str, linenum, col);
					}
				}
			}

			return null;
		}
	}
}
