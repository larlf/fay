using fay.compiler.runtime;
using System.Collections.Generic;
using System.Text;

namespace fay.compiler.ast
{
	public class AstFun : AstNode
	{
		List<string> descWords = new List<string>();

		public AstFun(string text) : base(text)
		{
		}

		public void SetName(string name)
		{
			this.text = name;
		}

		public void AddDesc(string word)
		{
			this.descWords.Add(word);
		}

		public override void Build(CodeBuilder builder)
		{
			FayFun method = new FayFun(this.text);
			method.Name = this.text;
			builder.bindFun(method);

			base.Build(builder);
		}

		public override void Format(StringBuilder sb = null, string indent = "")
		{
			sb.Append(indent).Append("fun ").Append(this.text).Append("(");
			this.nodes[0].Format(sb, indent);
			sb.Append(")\n");
			this.nodes[1].Format(sb, indent);
		}
	}

}
