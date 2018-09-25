using System.Text;

namespace fay.compiler.ast
{
	/// <summary>
	/// using语句节点
	/// </summary>
	public class AstUsing : AstNode
	{
		public AstUsing(string text) : base(text)
		{

		}

		public override void Build(CodeBuilder builder)
		{
			builder.currentFile.addUsing(this.text);
			base.Build(builder);
		}

		public override void Format(StringBuilder sb = null, string indent = "")
		{
			sb.Append("using ");
			sb.Append(this.Text);
			sb.Append(";\n");
		}
	}

}
