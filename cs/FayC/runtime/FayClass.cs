using fay.compiler.ast;
using fay.lib;
using Mirage.Sys;
using Mirage.Utils;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace fay.compiler.runtime
{
	public class FayClass : IDataObj
	{
		public string Name;  //名称
		public List<FayFun> methods = new List<FayFun>();

		public FayClass(string name)
		{
			this.Name = name;
		}

		public void addFun(FayFun fun)
		{
			this.methods.Add(fun);
		}

		public void FromData(byte[] data, ref int pos)
		{
		}

		public void ToData(MemoryStream data)
		{
			DataUtils.Write(data, this.Name);

			DataUtils.Write(data, this.methods.Count);
			foreach(var it in this.methods)
			{
				it.ToData(data);
			}
		}

		public string ToString(string index)
		{
			StringBuilder sb = new StringBuilder();
			sb.Append(index + "Class : " + this.Name);

			foreach (var it in this.methods)
			{
				sb.Append("\n");
				sb.Append(it.ToString(index + "\t"));
			}

			return sb.ToString();
		}

		public override string ToString()
		{
			return this.ToString("");
		}
	}
}
