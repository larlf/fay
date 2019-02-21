#include "fay_log.h"

using namespace mirror::utils;

std::atomic_flag fay::LogBus::LogLock = ATOMIC_FLAG_INIT;

std::string fay::LogBus::_LogFile;

void fay::LogBus::SetLogFile(const std::string & filename)
{
	LogBus::_LogFile = filename;
	if (filename.size() > 0)
		FileUtils::WriteTextFile(filename, "");
}

void fay::LogBus::Log(LogType type, const std::string & msg)
{
	while (LogLock.test_and_set())
		std::this_thread::yield();  //也可以用空循环

	switch (type)
	{
	case LogType::Info:
		std::cout << termcolor::yellow << msg << termcolor::reset << std::endl;
		break;
	case LogType::Warn:
		std::cout << termcolor::magenta << msg << termcolor::reset << std::endl;
		break;
	case LogType::Error:
		std::cout << termcolor::red << msg << termcolor::reset << std::endl;
		break;
	}

	if (_LogFile.size() > 0)
	{
		FileUtils::AppendTextFile(_LogFile, msg);
	}

	LogLock.clear();
}
