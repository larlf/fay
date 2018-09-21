using System;
using System.Collections.Generic;
using System.Text;

namespace fay.compiler.runtime
{
	/// <summary>
	/// Fay语言的作用域
	/// </summary>
	public class FayScope
	{
		private int _index = 0;

		private FayScope _parent;
		private Dictionary<string, FayVar> _vars = new Dictionary<string, FayVar>();

		public int Index => this._index;
		public FayScope parentScope => this._parent;

		public FayScope(FayScope parentScope)
		{
			this._parent = parentScope;
			this._index = parentScope.Index;
		}

		public void CreateVar(string name, string type)
		{
			this._vars[name] = new FayVar(name, type, this._index++);
		}

		public FayVar findVar(string name)
		{
			FayVar var;

			if (_vars.TryGetValue(name, out var))
				return var;

			if (this._parent != null)
				return this._parent.findVar(name);

			return null;
		}
	}
}
