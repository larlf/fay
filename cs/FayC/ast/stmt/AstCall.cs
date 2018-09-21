using System.Text;
using fay.lib.Instruction;

namespace fay.compiler.ast
{
	public class AstCall : AstNode
	{
		public AstCall(string text) : base(text)
		{
		}

		public override void Build(CodeBuilder builder)
		{
			base.Build(builder);

			//参数的数量
			int paramNum = 0;

			if (this.ChildNodeNum > 0)
			{
				paramNum = this.nodes[0].ChildNodeNum;
			}

			var inst = new StaticCall(this.text, paramNum);
			builder.AddInst(inst);
		}

		public override void Format(StringBuilder sb, string indent = "")
		{
			sb.Append(indent).Append(this.text).Append("(");
			base.Format(sb, indent);
			sb.Append(");\n");
		}
	}

}
