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
		private List<FayLib> _libs = new List<FayLib>();

		public void addLib(FayLib lib)
		{
			this._libs.Add(lib);
		}

		/// <summary>
		/// 查找类型
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		public List<FayType> findType(string name)
		{
			List<FayType> types = new List<FayType>();

			return types;
		}
    }
}
