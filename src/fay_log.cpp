#include "fay_log.h"

using namespace mirror::utils;

std::atomic_flag fay::LogBus::LogLock = ATOMIC_FLAG_INIT;

std::string fay::LogBus::LogFile;

void fay::LogBus::Log(const std::string & msg)
{
	while (LogLock.test_and_set())
		std::this_thread::yield();  //也可以用空循环

	if (LogFile.size() > 0)
	{
		if (msg[msg.size() - 1] != '\n')
		{
			std::string str = msg;
			str.append("\n");
			FileUtils::AppendTextFile(LogFile, str);
		}
		else
		{
			FileUtils::AppendTextFile(LogFile, msg);
		}
	}

	LogLock.clear();
}
