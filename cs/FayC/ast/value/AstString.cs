using System.Text;
using fay.lib;
using fay.lib.Instruction;

namespace fay.compiler.ast
{
	/// <summary>
	/// 这符串节点
	/// </summary>
	public class AstString : AstNode
	{
		public AstString(string text) : base(text)
		{
		}

		public override FayDataType ValueType()
		{
			return FayDataType.String;
		}

		public override void Build(CodeBuilder builder)
		{
			builder.AddInst(new PushString(this.text));
		}

		public override void Format(StringBuilder sb, string indent = "")
		{
			sb.Append(this.text);
		}
	}

}
