#pragma once
#include "fay_lang_const.h"
#include <iostream>
#include <string>

namespace fay
{
	namespace lang
	{
		class TextBuilder
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
			TextBuilder() : indent(0), isNewline(true)
			{
			}

			TextBuilder *add(const char *str)
			{
				this->checkNewline();
				text.append(str);
				return this;
			}

			TextBuilder *add(const std::string &str)
			{
				this->checkNewline();
				text.append(str);
				return this;
			}

			TextBuilder *add(const char c)
			{
				this->checkNewline();
				text.append(&c, 1);
				return this;
			}

			TextBuilder *add(int v)
			{
				this->checkNewline();
				text.append(std::to_string(v));
				return this;
			}

			TextBuilder *endl()
			{
				text.append("\n");
				this->isNewline = true;
				return this;
			}

			TextBuilder *indentAdd()
			{
				this->indent++;
				this->indentStr.clear();
				for (auto i = 0; i < this->indent; ++i)
					this->indentStr.append("  ");
				return this;
			}

			TextBuilder *indentSub()
			{
				this->indent--;
				this->indentStr.clear();
				for (auto i = 0; i < this->indent; ++i)
					this->indentStr.append("  ");
				return this;
			}

			TextBuilder *clear()
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