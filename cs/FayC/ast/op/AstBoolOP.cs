using fay.lib;

namespace fay.compiler.ast
{
	public class AstBoolOP : AstNode
	{
		public AstBoolOP(string text) : base(text)
		{
		}

		public override FayDataType ValueType()
		{
			return FayDataType.Bool;
		}
	}

}
