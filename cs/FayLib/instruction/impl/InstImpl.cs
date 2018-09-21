using Mirage.Utils;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace fay.lib.Instruction
{
	public class PushString : FayInst
	{
		private string p1;

		public PushString(string p1) : base(FayInstType.InstPushString)
		{
			this.p1 = p1;
		}

		public override void FromData(byte[] data, ref int pos)
		{
		}

		public override void ToData(MemoryStream data)
		{
			DataUtils.Write(data, this.type);
			DataUtils.Write(data, this.p1);
		}

	}

	public class StaticCall : FayInst
	{
		private string p1;  //调用的方法
		private int p2;  //参数数量

		public StaticCall(string p1, int p2) : base(FayInstType.InstStaticCall)
		{
			this.p1 = p1;
			this.p2 = p2;
		}

		public override void FromData(byte[] data, ref int pos)
		{
		}

		public override void ToData(MemoryStream data)
		{
			DataUtils.Write(data, this.type);
			DataUtils.Write(data, this.p1);
			DataUtils.Write(data, this.p2);
		}
	}
}
