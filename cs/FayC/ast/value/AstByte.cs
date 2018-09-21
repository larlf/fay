
using fay.compiler.lexical;
using fay.lib;

namespace fay.compiler.ast
{
	public class AstByte : AstNode
	{
		private Token token;

		public AstByte(Token token) : base(token.Text)
		{
			this.token = token;
		}

		public override FayDataType ValueType()
		{
			return FayDataType.Byte;
		}
	}

}
