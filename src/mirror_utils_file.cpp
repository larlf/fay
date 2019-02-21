#include "mirror_utils_file.h"
#include <mirror_utils_file.h>
#include <mirror_utils_log.h>

using namespace mirror::utils;

void mirror::utils::FileUtils::_FindFiles(std::vector<fs::path> &files, const fs::path &path, bool recursive, const std::string &extName)
{
	for (auto &fe : fs::directory_iterator(path))
	{
		//如果是目录，就递归
		if (fs::is_directory(fe.path()))
		{
			if (recursive)
				FileUtils::_FindFiles(files, fe.path(), recursive, extName);
		}
		else
		{
			std::string filename = fe.path().string();
			if (extName.size() < 1 || (filename.find_last_of(extName) == filename.size() - 1))
				files.push_back(fe.path());
		}
	}
}

bool mirror::utils::FileUtils::_WriteTextFile(const std::string & filename, const std::string & content, const std::string & mode)
{
	FILE* file = fopen(filename.c_str(), mode.c_str());
	if (file == nullptr)
	{
		LOG_ERROR("Cannot write file : " << filename);
		return false;
	}

	size_t length = fwrite(content.c_str(), 1, content.size(), file);
	if (length != content.size())
		LOG_ERROR("Write file size error : " << length << " != " << content.size());

	//文本文件的追加模式式，默认加换行
	if (mode.size() > 0 && mode[0] == 'a')
		putc('\n', file);

	fclose(file);
	return true;
}

std::string mirror::utils::FileUtils::ReadTextFile(const std::string &filename)
{
	FILE* fp = fopen(filename.c_str(), "rb");

	if (!fp)
		LOG_ERROR("Cannot read file : " << filename);
	else
	{
		string text;
		char* buffer = new char[1024];
		size_t len = 0;
		while (len = fread(buffer, 1, 1024, fp))
			text.append(buffer, len);

		delete[] buffer;
		fclose(fp);
		return text;
	}

	return "";
}

void mirror::utils::FileUtils::FindFiles(std::vector<fs::path> &files, const std::string &path, bool recursive, const std::string &extName)
{
	_FindFiles(files, path, recursive, extName);
}

void mirror::utils::FileUtils::FildFiles(std::vector<fs::path> &files, const fs::path &path, bool recursive, const std::string &extName)
{
	_FindFiles(files, path, recursive, extName);
}

bool mirror::utils::FileUtils::WriteTextFile(const std::string &filename, const std::string &content)
{
	return FileUtils::_WriteTextFile(filename, content, "w");
}

//向文本文件添加内容
//注意，这个方法写完后，会自己添加了一个\n
bool mirror::utils::FileUtils::AppendTextFile(const std::string & filename, const std::string & content)
{
	return FileUtils::_WriteTextFile(filename, content, "a");
}
