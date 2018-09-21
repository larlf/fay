using fay.lib;

namespace fay.compiler.lexical.Rules
{
	/// <summary>
	/// 基本的规则
	/// </summary>
	public abstract class BasicTokenRule : ITokenRule
	{
		protected TokenType type;
		protected LexMode mode;

		public BasicTokenRule(LexMode mode, TokenType type)
		{
			this.type = type;
			this.mode = mode;
		}

		public LexMode Mode { get => this.mode; }

		public abstract Token Match(char[] data, int pos, int line, int col);
	}
}
