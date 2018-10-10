#pragma once
#include <mirror_utils_string.h>

//这个文件主要用于定义一些不同对象之前公用或限定规则的类

namespace fay
{
	class FayObject
	{
	public:
		//生成字符串的方法
		virtual void toString(mirror::utils::StringBuilder* sb) {}
	};
}