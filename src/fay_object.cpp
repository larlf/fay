#include <fay_object.h>
#include <mirror_utils_log.h>

using namespace fay;

fay::FayException::FayException(const std::string & msg)
	: std::exception(msg.c_str())
{
	this->_trace = mirror::log::SysTrace::TraceInfo();
}

fay::FayException::FayException(const std::string & msg, const std::string & filename, int line, int col)
	: std::exception(msg.c_str()), _filename(filename), _line(line), _col(col)
{
	this->_trace = mirror::log::SysTrace::TraceInfo();
}
