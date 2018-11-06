#include "fay_object.h"
#include <fay_object.h>
#include <mirror_utils_log.h>

using namespace fay;

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
	if(this->_file)
	{
		std::string line = this->_file->line(this->_line);
		std::string str;
		std::string pos;

		for(int i = 0; i < line.size(); ++i)
		{
			char c = line[i];
			if(c == '\t')
			{
				str.push_back(' ');
				pos.push_back(' ');
			}
			else if(i == this->_col - 1)
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

	for(int i = 0; i < this->_text.size(); ++i)
	{
		char c = this->_text[i];
		if(c == '\n')
		{
			linenum++;
			continue;
		}

		if(linenum == index)
			line.push_back(c);
		else if(linenum > index)
			break;
	}

	return line;
}
