using System.Text;

namespace fay.compiler.ast
{
	/// <summary>
	/// 变量节点
	/// </summary>
	public class AstID : AstNode
	{
		public AstID(string text) : base(text)
		{
		}

		public override void Format(StringBuilder sb, string indent = "")
		{
			sb.Append(this.text);
		}
	}

}
