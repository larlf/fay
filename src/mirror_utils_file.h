#pragma once

#include <iostream>
#include <vector>
#include <filesystem>

namespace mirror
{
	namespace utils
	{
		class FileUtils
		{
		private:
			static void _FindFiles(std::vector<std::string> &files, const std::experimental::filesystem::path &path, bool recursive = true, const std::string &extName = "");

		public:
			//读取文本文件
			static std::string ReadTextFile(const std::string &filename);

			//查找目录下所有的文件（递归）
			static std::vector<std::string> FindFiles(const std::string &path, bool recursive = true, const std::string &extName = "");

		};
	}
}