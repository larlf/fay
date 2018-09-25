using fay.compiler.ast;
using System.Collections.Generic;

namespace fay.compiler.runtime
{
    public class FayFile
    {
		private string _filename;
		private List<string> _usings = new List<string>();
		private string _package;

		public List<string> usings => this._usings;
		public string packageName => this._package;

		//________________________________________________________

		public FayFile(string filename)
		{
			this._filename = filename;
		}

		public void setPackage(string package)
		{
			this._package = package;
		}

		public void addUsing(string package)
		{
			this._usings.Add(package);
		}
    }
}
