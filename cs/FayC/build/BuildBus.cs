

using fay.compiler.runtime;

namespace fay.compiler.Build
{
	/// <summary>
	/// 编译时的全局对象
	/// </summary>
	public static class BuildBus
	{
		public static FayDomain Domain = new FayDomain();
    }
}
