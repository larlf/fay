﻿using System.Text;

namespace fay.compiler.ast
{
	public class AstBlock : AstNode
	{
		public AstBlock() : base("")
		{
		}

		public override void Format(StringBuilder sb, string indent = "")
		{
			sb.Append(indent).Append("{\n");
			base.Format(sb, indent+"\t");
			sb.Append(indent).Append("}\n");
		}
	}

}
