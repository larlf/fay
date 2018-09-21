namespace fay.compiler.ast
{
	public class AstAssign : AstNode
	{
		public AstAssign(AstNode leftValue, AstNode value) : base("=")
		{
			this.AddChildNode(leftValue);
			this.AddChildNode(value);
		}
	}

}
