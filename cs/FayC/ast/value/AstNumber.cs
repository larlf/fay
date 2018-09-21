using System.Text;
using fay.lib;

namespace fay.compiler.ast
{
	/// <summary>
	/// 数字节点
	/// </summary>
	public class AstNumber : AstNode
	{
		public AstNumber(string text) : base(text)
		{
		}

		public override FayDataType ValueType()
		{
			return FayDataType.Double;
		}

		public override void Format(StringBuilder sb, string indent = "")
		{
			sb.Append(this.text);
		}
	}

}
