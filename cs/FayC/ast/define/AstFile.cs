using System.Text;

namespace fay.compiler.ast
{
	/// <summary>
	/// 一个文件节点
	/// </summary>
	public class AstFile : AstNode
	{
		public AstFile(string text) : base(text)
		{
		}
	}

}
