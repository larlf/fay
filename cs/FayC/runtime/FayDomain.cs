using fay.compiler.ast;
using Mirage.Sys;
using System;
using System.Collections.Generic;
using System.Text;

namespace fay.compiler.runtime
{
	/// <summary>
	/// 运行环境
	/// </summary>
	public class FayDomain
    {
		public List<FayLib> Libs = new List<FayLib>();
		private Dictionary<string, FayClass> classes = new Dictionary<string, FayClass>();

	

		public void ReadFunctions(AstNode rootNode)
		{

		}
    }
}
