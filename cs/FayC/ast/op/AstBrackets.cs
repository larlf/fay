namespace fay.compiler.ast
{
	public class AstBrackets : AstNode
	{
		public AstBrackets(AstNode value) : base("")
		{
			if (value != null)
				this.AddChildNode(value);
		}
	}

}
