#pragma once

#include <iostream>

namespace mirror
{
	namespace util
	{
		//字符串相关的公用方法
		class StringUtils
		{
		public:
			/**
			* 对字符串进行转码
			* @param source 源字符串
			* @param fromCode 源编码
			* @param toCode 目标编码
			*/
			static std::string Encoding(const std::string &source, const std::string &fromCode, const std::string &toCode);
		};
	}
}
