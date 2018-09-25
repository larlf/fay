using System.Text;

namespace fay.compiler.ast
{
	public class AstParamDefineList : AstNode
	{
		public AstParamDefineList() : base(null)
		{
		}

		public override void Format(StringBuilder sb = null, string indent = "")
		{
			for (var i = 0; i < this.nodes.Count; ++i)
			{
				this.nodes[i].Format(sb, indent);
				if (i < this.nodes.Count - 1)
					sb.Append(", ");
			}
		}

		public override string ToString(string index)
		{
			return base.ToString(index);
		}
	}

}
