#include <mirror_utils_file.h>
#include <mirror_utils_log.h>

using namespace mirror::utils;
namespace fs = std::experimental::filesystem;

void mirror::utils::FileUtils::_FindFiles(std::vector<std::string> &files, const std::experimental::filesystem::path &path, bool recursive, const std::string &extName)
{
	for(auto &fe : fs::directory_iterator(path))
	{
		//如果是目录，就递归
		if(fs::is_directory(fe.path()))
		{
			if(recursive)
				FileUtils::_FindFiles(files, fe.path(), recursive, extName);
		}
		else
		{
			std::string filename = fe.path().string();
			if(extName.size() < 1 || (filename.find_last_of(extName) == filename.size() - 1))
				files.push_back(fe.path().string());
		}
	}
}

std::string mirror::utils::FileUtils::ReadTextFile(const std::string &filename)
{
	FILE* fp = fopen(filename.c_str(), "rb");

	if(!fp)
		LOG_ERROR("Cannot read file : " << filename);
	else
	{
		string text;
		char* buffer = new char[1024];
		size_t len = 0;
		while(len = fread(buffer, 1, 1024, fp))
			text.append(buffer, len);

		delete[] buffer;
		fclose(fp);
		return text;
	}

	return "";
}

std::vector<std::string> mirror::utils::FileUtils::FindFiles(const std::string &path, bool recursive, const std::string &extName)
{
	std::vector<std::string> files;
	fs::path thePath(path);
	_FindFiles(files, path, recursive, extName);
	return files;
}

