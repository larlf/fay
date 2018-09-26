using fay.lib;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace fay.compiler.runtime
{
	/// <summary>
	/// Fay语言的节点
	/// </summary>
    public class FayNode : IDataObj
	{
		virtual public void FromData(byte[] data, ref int pos)
		{
		}

		virtual public void ToData(MemoryStream data)
		{
		}

		virtual public void ToString(StringBuilder sb, string indent)
		{
			sb.Append(indent).Append(this.ToString()).Append("\n");
		}
	}
}
