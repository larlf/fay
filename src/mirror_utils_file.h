﻿#pragma once

#include <iostream>
#include <vector>
#include <filesystem>

namespace fs = std::experimental::filesystem;

namespace mirror
{
	namespace utils
	{
		class FileUtils
		{
		private:
			static void _FindFiles(std::vector<fs::path> &files, const fs::path &path, bool recursive = true, const std::string &extName = "");

		public:
			//读取文本文件
			static std::string ReadTextFile(const std::string &filename);

			//查找目录下所有的文件（递归）
			static void FindFiles(std::vector<fs::path> &files, const std::string &path, bool recursive = true, const std::string &extName = "");
			static void FildFiles(std::vector<fs::path> &files, const fs::path &path, bool recursive = true, const std::string &extName = "");

			//写入文本文件
			static bool WriteTextFile(const std::string &filename, const std::string &content);
		};
	}
}