using System.Text;

namespace fay.compiler.ast
{
	public class AstParamDefine : AstNode
	{
		public AstParamDefine(string text) : base(text)
		{
		}

		public override void Format(StringBuilder sb, string indent = "")
		{
			sb.Append(this.text);
			sb.Append(":");
			sb.Append(this.nodes[0].Text);
		}
	}

}
