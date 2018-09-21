using fay.lib;

namespace fay.compiler.ast
{
	public class AstPostOP : AstNode
	{
		public AstPostOP(string text, AstNode subNode) : base(text)
		{
			this.AddChildNode(subNode);
		}

		public override FayDataType ValueType()
		{
			return FayDataType.Void;
		}
	}

}
