using fay.compiler.ast;
using fay.lib;
using Mirage.Utils;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace fay.compiler.runtime
{
	public class FayLib : IDataObj
	{
		public string Name;  //名称
		public int Major;  //大版本
		public int Minor;  //小版本
		public Dictionary<string, FayClass> Classes = new Dictionary<string, FayClass>();

		public void FromData(byte[] data, ref int pos)
		{
			this.Name = DataUtils.ReadString(data, ref pos);
			this.Major = DataUtils.ReadInt32(data, ref pos);
			this.Minor = DataUtils.ReadInt32(data, ref pos);
		}

		/// <summary>
		/// 添加Class
		/// </summary>
		/// <param name="clazz"></param>
		public void addClass(FayClass clazz)
		{
			this.Classes[clazz.Name] = clazz;
		}

		public void ToData(MemoryStream data)
		{
			DataUtils.Write(data, this.Name);
			DataUtils.Write(data, this.Major);
			DataUtils.Write(data, this.Minor);

			//写入所有的类型
			DataUtils.Write(data, this.Classes.Count);
			foreach(var it in this.Classes)
			{
				it.Value.ToData(data);
			}
		}

		public string ToString(string indent)
		{
			StringBuilder sb = new StringBuilder();
			sb.Append(indent+"[Types]\n");

			foreach (var it in this.Classes)
			{
				sb.Append(it.Value.ToString(indent+"\t"));
				sb.Append("\n");
			}

			return sb.ToString();
		}

		public override string ToString()
		{
			return this.ToString("");
		}
	}
}
