using fay.lib;

namespace fay.compiler.lexical
{
	/// <summary>
	/// 用于生成Token的规则
	/// </summary>
	public interface ITokenRule
	{
		//只在什么模式下有效果
		LexMode Mode { get; }

		Token Match(char[] data, int pos, int line, int col);
	}
}
