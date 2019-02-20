#pragma once
#include <fay_object.h>

namespace fay
{
	class LogBus
	{
	private:
		static std::atomic_flag LogLock;

	public:
		static std::string LogFile;
		static void Log(const std::string &msg);
	};
}

