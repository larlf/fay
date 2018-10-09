#pragma once

#include <mirror_sys_const.h>
#include <fay_lang.h>

namespace fay
{
	//用于生成整体的代码结构
	class OutlineBuilder
	{
	private:
		PTR(FayLib) _lib;
		PTR(FayClass) _class;

	public:
		OutlineBuilder(PTR(FayLib) lib);
		~OutlineBuilder();

		void bindClass(const std::string &name);
	};

	//用于生成中间代码
	class InstBuilder
	{

	};
}

