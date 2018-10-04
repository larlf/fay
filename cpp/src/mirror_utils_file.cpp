#include <mirror_utils_file.h>
#include <mirror_utils_log.h>

std::string mirror::utils::FileUtils::ReadTextFile(const std::string &filename)
{
	FILE* fp = fopen(filename.c_str(), "rb");

	if (!fp)
	{
		LOG_ERROR("Cannot read file : " << filename)
	}
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

