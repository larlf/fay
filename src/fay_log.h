#pragma once
#include <mirror.h>
#include <fay_const.h>

namespace fay
{
	//Log������
	enum class LogType
	{
		Debug,
		Info,
		Warn,
		Error
	};

	//����ͳһ����Log�Ĳ���
	class LogBus
	{
	private:
		static std::atomic_flag LogLock;
		static std::string _LogFile;

	public:
		static void SetLogFile(const std::string &filename);
		static void Log(LogType type, const std::string &msg);
		static bool IsDebug() { return true; }
		static void Debug(const std::string &msg) { LogBus::Log(LogType::Debug, msg); }
		static void Info(const std::string &msg) { LogBus::Log(LogType::Info, msg); }
		static void Warn(const std::string &msg) { LogBus::Log(LogType::Warn, msg); }
		static void Error(const std::string &msg) { LogBus::Log(LogType::Error, msg); }
	};
}

