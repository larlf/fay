#pragma once

#include <iostream>
#include <string>

namespace mirror
{
	namespace utils
	{
		/**
		 * 字符串相关的公用方法
		 */
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
			/**
			* 对特殊字符进行转码
			*/
			static std::string EncodeSpecialChar(const std::string &str);
		};
		
		//字符串构造类
		//比普通字符串增强了：
		//1. 一些自动转换
		//2. 对缩进的处理
		class StringBuilder
		{
		private:
			std::string text;
			int indent;
			std::string indentStr;
			bool isNewline;

			void checkNewline()
			{
				if (this->isNewline)
				{
					text.append(this->indentStr);
					this->isNewline = false;
				}
			}

		public:
			StringBuilder() : indent(0), isNewline(true)
			{
			}

			StringBuilder *add(const char *str)
			{
				this->checkNewline();
				text.append(str);
				return this;
			}

			StringBuilder *add(const std::string &str)
			{
				this->checkNewline();
				text.append(str);
				return this;
			}

			StringBuilder *add(const char c)
			{
				this->checkNewline();
				text.append(&c, 1);
				return this;
			}

			StringBuilder *add(int v)
			{
				this->checkNewline();
				text.append(std::to_string(v));
				return this;
			}

			StringBuilder *endl()
			{
				text.append("\n");
				this->isNewline = true;
				return this;
			}

			//增加缩进
			StringBuilder *increaseIndent()
			{
				this->indent++;
				this->indentStr.clear();
				for (auto i = 0; i < this->indent; ++i)
					this->indentStr.append("  ");
				return this;
			}

			//减少缩进
			StringBuilder *decreaseIndent()
			{
				this->indent--;
				this->indentStr.clear();
				for (auto i = 0; i < this->indent; ++i)
					this->indentStr.append("  ");
				return this;
			}

			StringBuilder *clear()
			{
				this->text.clear();
				this->indentStr.clear();
				this->indent = 0;
				this->isNewline = false;
				return this;
			}

			std::string toString()
			{
				return this->text;
			}
		};
	}
}
