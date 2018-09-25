using System.Text;

namespace fay.compiler.ast
{
	/// <summary>
	/// Package语句节点
	/// </summary>
	public class AstPackage : AstNode
	{
		public AstPackage(string text) : base(text)
		{
		}

		public override void Build(CodeBuilder builder)
		{
			builder.currentFile.setPackage(this.text);
		}

		public override void Format(StringBuilder sb = null, string indent = "")
		{
			sb.Append("package ").Append(this.text).Append(";\n");
		}
	}

}
