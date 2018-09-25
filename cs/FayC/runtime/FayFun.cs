using fay.compiler.ast;
using fay.lib;
using fay.lib.Instruction;
using Mirage.Sys;
using Mirage.Utils;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace fay.compiler.runtime
{
	public class FayFun : IDataObj
	{
		private FayClass _class;
		public string Name;
		private AstParamDefineList ps;
		private AstNode ast;
		private List<FayInst> insts = new List<FayInst>();
		private List<FayParamDefine> _paramDefineList = new List<FayParamDefine>();  //参数表

		public FayDomain domain => this._class.domain;

		public FayFun(FayClass clazz, string name, AstParamDefineList ps, AstNode ast=null)
		{
			this._class = clazz;
			this.Name = name;
			this.ps = ps;
			this.ast = ast;
		}

		public void addParam(FayParamDefine param)
		{
			this._paramDefineList.Add(param);
		}

		public List<FayInst> Insts
		{
			get
			{
				return this.insts;
			}
		}

		public void FromData(byte[] data, ref int pos)
		{
		}

		public void ToData(MemoryStream data)
		{
			DataUtils.Write(data, this.Name);

			List<FayInst> insts = this.Insts;

			DataUtils.Write(data, insts.Count);
			foreach (var it in insts)
			{
				it.ToData(data);
			}
		}

		public string ToString(string index)
		{
			StringBuilder sb = new StringBuilder();
			sb.Append(index + "Method : " + this.Name);
			if (this.insts != null)
			{
				foreach (var it in this.Insts)
				{
					sb.Append("\n");
					it.ToString(sb, index + "\t");
				}
			}

			return sb.ToString();
		}

		public override string ToString()
		{
			return this.ToString("");
		}
	}
}
