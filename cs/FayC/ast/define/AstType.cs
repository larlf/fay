using System.Text;

namespace fay.compiler.ast
{
	/// <summary>
	/// 数据类型
	/// </summary>
	public class AstType : AstNode
	{
		public AstType(string text, bool isArray) : base(text)
		{
			if (isArray)
				this.text = text + "[]";
		}

		public override void Format(StringBuilder sb, string indent = "")
		{
			sb.Append(this.text);
		}
	}


}
