using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace fay.lib.Instruction
{
	/// <summary>
	/// 指令的接口
	/// </summary>
	public abstract class FayInst : IDataObj
	{
		protected int type;

		public FayInst(int type)
		{
			this.type = type;
		}

		public abstract void FromData(byte[] data, ref int pos);
		public abstract void ToData(MemoryStream data);

		public string ToString(string index)
		{
			StringBuilder sb = new StringBuilder();
			sb.Append(index + this.GetType().Name);
			return sb.ToString();
		}

		public override string ToString()
		{
			return this.ToString("");
		}
	}
}
