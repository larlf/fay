using fay.lib;

namespace fay.compiler.ast
{
	public class AstBracket : AstNode
	{
		public AstBracket(AstNode leftNode, AstNode rightNode) : base("")
		{
			this.AddChildNode(leftNode);
			this.AddChildNode(rightNode);
		}

		public override FayDataType ValueType()
		{
			return FayDataType.Void;
		}
	}

}
