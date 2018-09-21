#pragma once

#include "mirage_const.h"
#include "mirage_byte_data.h"
#include "mirage_byte_buffer.h"

#include <time.h>
#include <math.h>
#include <sstream>
#include <memory>
#include <jansson.h>
#include <algorithm> 
#include <cctype>
#include <locale>

namespace mirage
{
	namespace utils
	{
		/**
		 * 一些系统信息的辅助类
		 */
		class SystemUtils
		{
		public:
			/**
			 * 读取文本文件
			 */
			static string ReadTextFile(const string& filename);
		};

		//文件系统的辅助类
		class FileUtils
		{
		public:
			//判断文件是否存在
			//@path : 要判断的文件路径
			static bool IsExist(const char *path);

			//判断是否是文件
			//@path : 要判断的文件路径
			static bool IsFile(const char *path);

			//判断是否是目录
			//@path : 要判断的文件路径
			static bool IsDir(const char *path);

			//取得文件的长度
			//@path : 文件路径
			//@retrun : 返回文件的长度，如果出错的话返回-1
			static long FileSize(const char *path);

			//读取文件
			static mirage::ByteData* ReadFile(const char *path);
		};

		/**
		 * 新加的字符串工具类，以后以这个为准
		 */
		class StringUtils
		{
		public:
			static inline void LTrimSelf(std::string &s)
			{
				s.erase(s.begin(), std::find_if(s.begin(), s.end(),
					[](int ch)
				{
					return !std::isspace(ch);
				}));
			}

			static inline void RTrimSelf(std::string &s)
			{
				s.erase(std::find_if(s.rbegin(), s.rend(),
					[](int ch)
				{
					return !std::isspace(ch);
				}).base(), s.end());
			}

			static inline void TrimSelf(std::string &s)
			{
				LTrimSelf(s);
				RTrimSelf(s);
			}

			static inline std::string LTrim(std::string s)
			{
				LTrimSelf(s);
				return s;
			}

			static inline std::string RTrim(std::string s)
			{
				RTrimSelf(s);
				return s;
			}

			static inline std::string Trim(std::string s)
			{
				TrimSelf(s);
				return s;
			}

			/////////////////////////////////////////////////////////////

			/**
			 * 查找字符串里是否有指定的字符
			 */
			static inline size_t FindAny(const std::string &s, const std::string &c, size_t start)
			{
				for (auto i = start; i < s.size(); ++i)
				{
					for (auto j = 0; j < c.size(); ++j)
					{
						if (s[i] == c[j])
							return i;
					}
				}

				return std::string::npos;
			}

			static inline size_t FindAny(const std::string &s, const std::string &c)
			{
				return FindAny(s, c, 0);
			}

			//////////////////////////////////////////////////////////////

			static void Split(const std::string& s, std::vector<std::string>& v, const std::string& c)
			{
				std::string::size_type pos1, pos2;

				pos2 = s.find(c);
				pos1 = 0;
				while (std::string::npos != pos2)
				{
					v.push_back(s.substr(pos1, pos2 - pos1));

					pos1 = pos2 + c.size();
					pos2 = s.find(c, pos1);
				}

				if (pos1 != s.length())
					v.push_back(s.substr(pos1));
			}

			static std::vector<std::string> Split(const std::string &s, const std::string &c)
			{
				std::vector<std::string> v;
				Split(s, v, c);
				return v;
			}

			static void SplitWithAny(const std::string& s, std::vector<std::string>& v, const std::string& c)
			{
				std::string::size_type pos1, pos2;

				pos2 = FindAny(s, c);
				pos1 = 0;
				while (std::string::npos != pos2)
				{
					v.push_back(s.substr(pos1, pos2 - pos1));

					pos1 = pos2 + 1;
					pos2 = FindAny(s, c, pos1);
				}

				if (pos1 != s.length())
					v.push_back(s.substr(pos1));
			}

			static std::vector<std::string> SplitWithAny(const std::string &s, const std::string &c)
			{
				std::vector<std::string> v;
				SplitWithAny(s, v, c);
				return v;
			}
		};

		/**
		 * 字符串处理的工具类
		 */
		class TextUtils
		{
		private:
			static char SqlChar[];
			static size_t SqlCharSize;

		public:
			/**
			 * 对字符串进行转码
			 * @param source 源字符串
			 * @param fromCode 源编码
			 * @param toCode 目标编码
			 */
			static string Encoding(const string &source, const string &fromCode, const string &toCode);

			/**
			 * 输出二进制数据的结构
			 * @param data 数据
			 * @@param size 长度
			 * @return 显示结构的字符串信息
			 */
			static string Dump(const byte *data, uint size);

			/**
			 * 转换成16进制字符串
			 */
			static string ToHex(const byte *data, uint size);
			static string ToHex(int value) { return ToHex((byte*)&value, 4); }
			static string ToHex(uint value) { return ToHex((byte*)&value, 4); }
			static string ToHex(float value) { return ToHex((byte*)&value, 4); }
			static string ToHex(double value) { return ToHex((byte*)&value, 8); }

			/**
			 * 对字符串进行SQL转码
			 */
			static string SQLEncoding(const string& str);

			/**
			 * 把字符串转换成Json中可以使用的字符串格式
			 * 主要是对控制字符进行了替换，手工生成Json字符串的时候最好都用这个先处理一下
			 */
			static string JsonEncoding(const string &str);

			static string Trim(const string& str);
			static int Split(const string& str, vector<string>& ret_, string sep = ",");
			static string Replace(const string& str, const string& src, const string& dest);
		};

		/**
		 * 时间相关的工具类
		 */
		class TimeUtils
		{
		public:
			/**
			* 取得秒级时间戳
			*/
			static long TimeStamp() { return (long)time(0); }

			/**
			* 取得毫秒级的时间戳
			*/
			static uint64 MSTimeStamp()
			{
#ifdef WIN32
				struct timeb tp;
				ftime(&tp);
				return tp.time * 1000 + tp.millitm;
#else
				struct timeval t;
				gettimeofday(&t, 0);
				uint64 s = time(0);
				return s * 1000 + (t.tv_usec / 1000);
#endif
			}

			/**
			 * 取得秒级时间戳的字符串
			 */
			static string TimeString(long t = 0);

			/**
			 * 取得毫秒级时间戳的字符串
			 */
			static string MSTimeString(uint64 t = 0);

			/**
			 * 取得指定月的天数
			 * @param year 年，如 2015
			 * @param month 月，1~12
			 * @return 天数，出错返回0
			 */
			static int DaysInMonth(int year, int month);

			/**
			 * 取得时区的偏移值，以秒为单位
			 * @@return 偏移值
			 */
			static int GetTimezoneOffset();
		};

		/**
		 * Json处理相关的工具类
		 */
		class JsonUtils
		{
		private:
			/** 检查是不是一个数组的下标 */
			static bool IsArrayIndex(const string &str);
		public:
			static json_t* OpenJsonFile(const string &filename);
			static json_t* GetValue(const json_t *node, const string &props);
			static string GetStringValue(const json_t *node, const string &props);
			static int GetIntValue(const json_t *node, const string &props);
			static double GetNumValue(const json_t *node, const string &props);
			static bool GetBoolValue(const json_t *node, const string &props);
			static size_t GetArraySize(const json_t *node, const string &props);
			static vector<string> GetMapKeys(const json_t *node, const string &props);
		};

	}
}
