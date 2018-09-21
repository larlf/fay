using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace fay.lib
{
	/// <summary>
	/// 数据对象，可以生成二进制数据
	/// </summary>
    public interface IDataObj
    {
		void ToData(MemoryStream data);
		void FromData(byte[] data, ref int pos);
    }
}
