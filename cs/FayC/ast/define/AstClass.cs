using fay.compiler.runtime;
using System.Collections.Generic;
using System.Text;

namespace fay.compiler.ast
{
	public class AstClass : AstNode
	{
		List<string> descWords = new List<string>();

		public AstClass(string text) : base(text)
		{
		}

		public void AddDesc(string word)
		{
			this.descWords.Add(word);
		}

		public override void Build(CodeBuilder builder)
		{
			FayClass type = new FayClass(builder.currentLib, this.text);
			type.Name = this.text;
			builder.bindClass(type);

			base.Build(builder);
		}

		public override void Format(StringBuilder sb = null, string indent = "")
		{
			sb.Append(indent + "class\n");
			sb.Append(indent + "{\n");
			base.Format(sb, indent+"\t");
			sb.Append(indent + "}\n");
		}
	}

}
