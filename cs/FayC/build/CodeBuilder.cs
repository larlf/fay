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
		FayDomain _domain = new FayDomain();
		FayFile _file;
		FayLib _lib;
		FayClass _class;
		FayFun _fun;
		FayScope _scope;

		public FayDomain domain => this._domain;
		public FayFile currentFile => this._file;
		public FayLib currentLib => this._lib;
		public FayClass currentClass => this._class;
		public FayFun currentFun => this._fun;

		//_________________________________________________________

		public CodeBuilder()
		{

		}

		public void bindFile(FayFile file)
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

		/// <summary>
		/// 第一次，生成类的结构信息
		/// </summary>
		public void build1()
		{
			this._domain.addLib(this._lib);
		}

		public void addScope()
		{
			this._scope = new FayScope(this._scope);
		}

		public void subScope()
		{
			this._scope = this._scope.parentScope;
		}

		public void AddInst(FayInst inst)
		{
			this._fun.Insts.Add(inst);
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
