#include "fay_object.h"
#include <fay_object.h>
#include <mirror_utils_log.h>
#include <mirror_utils_string.h>

using namespace fay;
using namespace mirror;

fay::FayCompileException::FayCompileException(const std::string &msg)
	: std::exception(msg.c_str())
{
	this->_trace = mirror::log::SysTrace::TraceInfo();
}

fay::FayCompileException::FayCompileException(const std::string &msg, PTR(FayFile) file, int line, int col)
	: std::exception(msg.c_str()), _file(file), _line(line), _col(col)
{
	this->_trace = mirror::log::SysTrace::TraceInfo();
}

const std::string fay::FayCompileException::source()
{
	if (this->_file)
	{
		std::string line = this->_file->line(this->_line);
		std::string str;
		std::string pos;

		for (int i = 0; i < line.size(); ++i)
		{
			char c = line[i];
			if (c == '\t')
			{
				str.push_back(' ');
				pos.push_back(' ');
			}
			else if (i == this->_col - 1)
			{
				str.push_back(c);
				pos.push_back('^');
			}
			else
			{
				str.push_back(c);
				pos.push_back(' ');
			}
		}

		return str + "\n" + pos;
	}

	return "";
}

const std::string fay::FayFile::line(int index)
{
	std::string line;
	int linenum = 1;

	for (int i = 0; i < this->_text.size(); ++i)
	{
		char c = this->_text[i];
		if (c == '\n')
		{
			linenum++;
			continue;
		}

		if (linenum == index && c != '\n' && c != '\r')
			line.push_back(c);
		else if (linenum > index)
			break;
	}

	return line;
}

const std::string fay::FayFile::line(int line, int col)
{
	std::string str = this->line(line);
	std::stringstream ss;

	int state = 0;
	char c;
	for (auto i = 1; i <= str.size(); ++i)
	{
		c = str[i - 1];

		switch (state)
		{
		case 0:
			if (i >= col)
			{
				state = 1;
				ss << termcolor::on_red;
			}
			break;
		case 1:
			if (c == ' ' || c == '\t' || c == ';' || c == ')')
			{
				state = 2;
				ss << termcolor::reset;
			}
			break;
		}

		ss << c;
	}

	//处理结束的特殊情况
	if(state==1)
		ss << termcolor::reset;

	return std::string(ss.str());
}

std::string fay::BaseObject::toString()
{
	utils::StringBuilder sb;
	this->buildString(&sb);
	return sb.toString();
}
