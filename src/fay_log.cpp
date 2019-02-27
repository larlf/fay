#include "fay_log.h"
#include "fay_log.h"
#include <fay_log.h>

using namespace mirror::utils;
using namespace fay;

std::recursive_mutex fay::LogBus::LogLock;

size_t fay::LogBus::_Count[(size_t)LogType::_Max];

std::thread::id fay::LogBus::_MainThreadID;

std::string fay::LogBus::_LogFile;

MAP<std::thread::id, PTR(std::vector<PTR(fay::LogData)>)> fay::LogBus::_Loggers;

PTR(std::vector<PTR(LogData)>) fay::LogBus::_GetThreadLogger(const std::thread::id &id)
{
	std::lock_guard<std::recursive_mutex> lg(LogBus::LogLock);

	auto it = LogBus::_Loggers.find(id);
	if(it != LogBus::_Loggers.end())
		return it->second;

	PTR(std::vector<PTR(LogData)>) logger = MKPTR(std::vector<PTR(LogData)>)();
	LogBus::_Loggers[id] = logger;
	return logger;
}

void fay::LogBus::_RemoveThreadLogger(const std::thread::id &id)
{
	std::lock_guard<std::recursive_mutex> lg(LogBus::LogLock);

	auto it = LogBus::_Loggers.find(id);
	if(it != LogBus::_Loggers.end())
		LogBus::_Loggers.erase(it);
}

void fay::LogBus::Init(const std::string &filename)
{
	LogBus::_MainThreadID = std::this_thread::get_id();
	LogBus::ResetCount();

	if(filename.size() > 0)
	{
		LogBus::_LogFile = filename;
		FileUtils::WriteTextFile(filename, "");
	}
}

void fay::LogBus::Log(LogType type, const std::string &msg, PTR(FilePart) part)
{
	std::thread::id id = std::this_thread::get_id();

	if(id == LogBus::_MainThreadID)
		LogBus::_Log(type, msg, part);
	else
	{
		auto data = MKPTR(LogData)(type, msg, part);

		std::lock_guard<std::recursive_mutex> lg(LogBus::LogLock);
		LogBus::_GetThreadLogger(id)->push_back(data);
	}
}

void fay::LogBus::_Log(LogType type, const std::string &msg, PTR(FilePart) part)
{
	//std::string msg = data;

	std::lock_guard<std::recursive_mutex> lg(LogBus::LogLock);
	LogBus::_Count[(size_t)type]++;

	switch(type)
	{
		case LogType::Info:
#if WIN32
			std::cout << termcolor::yellow << "Info : " << StringUtils::Encoding(msg, "UTF-8", "GBK") << termcolor::reset << std::endl;
#else
			std::cout << termcolor::yellow << msg << termcolor::reset << std::endl;
#endif
			break;
		case LogType::Warn:
#if WIN32
			std::cout << termcolor::magenta << "Warn : " << StringUtils::Encoding(msg, "UTF-8", "GBK") << termcolor::reset << std::endl;
#else
			std::cout << termcolor::magenta << msg << termcolor::reset << std::endl;
#endif
			break;
		case LogType::Error:
#if WIN32
			std::cout << termcolor::red << "Error : " << StringUtils::Encoding(msg, "UTF-8", "GBK") << termcolor::reset << std::endl;
#else
			std::cout << termcolor::red << msg << termcolor::reset << std::endl;
#endif
			break;
	}

	//输出到文件
	if(_LogFile.size() > 0)
	{
		switch(type)
		{
			case fay::LogType::Debug:
				break;
			case fay::LogType::Info:
				FileUtils::AppendTextFile(_LogFile, "[Info]");
				break;
			case fay::LogType::Warn:
				FileUtils::AppendTextFile(_LogFile, "[Warn]");
				break;
			case fay::LogType::Error:
				FileUtils::AppendTextFile(_LogFile, "[Error]");
				break;
			default:
				break;
		}
		FileUtils::AppendTextFile(_LogFile, msg);
	}

	//如果有代码的位置，输出一下
	if(part)
	{
		std::string str = part->print();

		if(_LogFile.size() > 0)
			FileUtils::AppendTextFile(_LogFile, str);
	}
}

void fay::LogBus::EndThread()
{
	std::thread::id id = std::this_thread::get_id();

	std::lock_guard<std::recursive_mutex> lg(LogBus::LogLock);
	auto logger = LogBus::_GetThreadLogger(id);
	if(logger != nullptr)
	{
		for(auto log : *logger)
			LogBus::_Log(log->type, log->msg, log->part);

		LogBus::_RemoveThreadLogger(id);
	}
}

void fay::LogBus::EndAllThread()
{
	std::lock_guard<std::recursive_mutex> lg(LogBus::LogLock);

	for(auto logger : LogBus::_Loggers)
	{
		for(auto log : *logger.second)
			LogBus::_Log(log->type, log->msg, log->part);
	}

	LogBus::_Loggers.clear();
}

void fay::LogBus::ResetCount()
{
	for(size_t i = 0; i < (size_t)LogType::_Max; ++i)
		LogBus::_Count[i] = 0;
}

size_t fay::LogBus::Count(LogType type)
{
	return LogBus::_Count[(size_t)type];
}
