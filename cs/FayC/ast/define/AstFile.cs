using fay.compiler.runtime;
using System.Text;

namespace fay.compiler.ast
{
	/// <summary>
	/// 一个文件节点
	/// </summary>
	public class AstFile : AstNode
	{
		public AstFile(string text) : base(text)
		{
		}

		public override void Build(CodeBuilder builder)
		{
			FayFile file = new FayFile(this.text);
			builder.bindFile(file);

			base.Build(builder);
		}

		public override void Format(StringBuilder sb, string indent = "")
		{
			sb.Append("File : ").Append(this.text).Append("\n");
			base.Format(sb, indent);
		}
	}

}
