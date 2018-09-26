using System;
using System.Collections.Generic;
using System.Text;

namespace fay.compiler.runtime
{
	/// <summary>
	/// 方法定义中的参数定义
	/// </summary>
    public class FayParamDefine
    {
		private FayDomain _domain;
		private string _name;
		private string _type;

		public FayParamDefine(FayDomain domain, string name, string type)
		{
			this._domain = domain;
			this._name = name;
			this._type = type;
		}

		/// <summary>
		/// 对类型进行检查
		/// </summary>
		public void check()
		{
			
		}
	}
}
