using fay.lib;
using System.Text;

namespace fay.compiler.ast
{
	/// <summary>
	/// 左右操作节点
	/// </summary>
	public class AstLeftRightOP : AstNode
	{
		public AstLeftRightOP(string text) : base(text)
		{
		}

		public override FayDataType ValueType()
		{
			return FayDataType.Void;
		}

		public override void Format(StringBuilder sb, string indent = "")
		{
			this.nodes[0].Format(sb);
			sb.Append(" ").Append(this.text).Append(" ");
			this.nodes[1].Format(sb);
		}
	}

}
