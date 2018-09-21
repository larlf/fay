using fay.lib;

namespace fay.compiler.ast
{
	/// <summary>
	/// 前置操作符节点
	/// </summary>
	public class AstPreOP : AstNode
	{
		public AstPreOP(string text) : base(text)
		{
		}

		public override FayDataType ValueType()
		{
			return FayDataType.Void;
		}
	}

}
