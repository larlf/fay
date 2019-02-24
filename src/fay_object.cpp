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
	return "Old Method";
}

std::string fay::BaseObject::toString()
{
	utils::StringBuilder sb;
	this->buildString(&sb);
	return sb.toString();
}
