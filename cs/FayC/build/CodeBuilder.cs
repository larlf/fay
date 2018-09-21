using fay.compiler.runtime;
using fay.lib.Instruction;
using System;
using System.Collections.Generic;
using System.Text;

namespace fay.compiler
{
	/// <summary>
	/// 代码生成器
	/// </summary>
	public class CodeBuilder
	{
		FayLib _lib;
		FayClass _class;
		FayFun _fun;
		FayScope _scope;

		List<FayInst> insts = new List<FayInst>();

		public FayLib Lib => this._lib;

		//_________________________________________________________

		public CodeBuilder()
		{

		}

		public void bindLib(FayLib lib)
		{
			this._lib = lib;
		}

		public void bindClass(FayClass clazz)
		{
			this._lib.addClass(clazz);
			this._class = clazz;
		}

		public void bindFun(FayFun fun)
		{
			this._class.addFun(fun);
			this._fun = fun;
		}

		public void addScope()
		{
			this._scope = new FayScope(this._scope);
		}

		public void subScope()
		{

		}

		public void AddInst(FayInst inst)
		{
			this.insts.Add(inst);
		}

		public List<FayInst> Build()
		{
			return this.insts;
		}

		public string ToString(string index)
		{
			StringBuilder sb = new StringBuilder();
			sb.Append(index + "[Lib]\n");
			sb.Append(this._lib.ToString(index + "\t"));

			return sb.ToString();
		}

		public override string ToString()
		{
			return this.ToString("");
		}
	}
}
