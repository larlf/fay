using System.Text;

namespace fay.compiler.ast
{
	public class AstVar : AstNode
	{

		public AstVar(string text, AstNode type, AstNode value) : base(text)
		{
			this.AddChildNode(type);
			this.AddChildNode(value);
		}

		public override void Format(StringBuilder sb, string indent = "")
		{
			sb.Append(indent).Append("var ").Append(this.text);
			if(this.nodes.Count>0)
			{
				if(this.nodes[0]!=null)
				{
					sb.Append(":");
					this.nodes[0].Format(sb, indent);
				}

				if(this.nodes.Count>1)
				{
					sb.Append(" = ");
					this.nodes[1].Format(sb, indent);
				}
			}
			sb.Append(";\n");
		}
	}

}
