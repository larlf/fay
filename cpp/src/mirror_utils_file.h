#pragma once

#include <iostream>

namespace mirror
{
	namespace utils
	{
		class FileUtils
		{
		public:
			/**
			* 读取文本文件
			*/
			static std::string ReadTextFile(const std::string& filename);
		};
	}
}