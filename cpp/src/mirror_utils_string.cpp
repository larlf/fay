#include <mirror_utils_string.h>

#include <iconv.h>
#include <string>

using namespace mirror::utils;

const std::string StringUtils::Blank;

std::string mirror::utils::StringUtils::Encoding(const std::string &source, const std::string &fromCode, const std::string &toCode)
{
	if (fromCode == toCode)
		return source;

#ifdef WIN32
	//源字符串的位置和长度
	const char* c1 = source.c_str();
	size_t s1 = source.size();

	//目标字符串的空间和长度
	size_t s2 = s1 * 2;
	size_t _s2 = s2;  //因为s2会变，缓存一下
	char* buffer = new char[s2];
	char* c2 = buffer;  //因为c2会变，不能直接用buffer
	bool success = true;

	if (s1 > 0)
	{
		iconv_t cd = iconv_open(toCode.c_str(), fromCode.c_str());

		while (s1 > 0)
		{
#ifdef WIN32
			int r = iconv(cd, &c1, &s1, &c2, &s2);
#else
			int r = iconv(cd, (char**)&c1, &s1, &c2, &s2);
#endif
			if (r < 0)
			{
				//cerr<<"Error in Iconv, code : "<<errno<<endl;
				success = false;
				break;
			}
		}

		iconv_close(cd);
	}

	if (success)	//转码出错
	{
		std::string r(buffer, _s2 - s2);
		delete[] buffer;
		return r;
	}
	else
	{
		delete[] buffer;
		return source;
	}
#else
	return source;
#endif
}

std::string mirror::utils::StringUtils::EncodeSpecialChar(const std::string & str)
{
	std::string r;

	for (auto i = 0; i < str.size(); ++i)
	{
		switch (str[i])
		{
		case '\r':
			r += "\\r";
			break;
		case '\n':
			r += "\\n";
			break;
		default:
			if (str[i] >= 128)
				r += '?';
			else
				r += str[i];
			break;
		}
	}

	return r;
}
