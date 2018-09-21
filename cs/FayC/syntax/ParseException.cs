using System;
using System.Collections.Generic;
using System.Text;

namespace fay.compiler.syntax
{
	/// <summary>
	/// 语法解析的异常
	/// </summary>
    public class ParseException : Exception
    {
		public ParseException(TokenStack stack, string msg) : base(msg)
		{
			
		}
    }
}
