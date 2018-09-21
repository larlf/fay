using System;
using System.Collections.Generic;
using System.Text;

namespace fay.compiler.runtime
{
    public class FayVar
    {
		public string name;
		public string type;
		public int index;

		public FayVar(string name, string type, int index)
		{
			this.index = index;
			this.name = name;
			this.type = type;
		}
    }
}
