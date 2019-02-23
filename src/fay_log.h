#pragma once
#include <mirror.h>
#include <fay_const.h>
#include <thread>
#include <mutex>

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
		static std::string _LogFile;

	public:
		static std::recursive_mutex LogLock;

		static void SetLogFile(const std::string &filename);
		static void Log(LogType type, const std::string &msg);
		static bool IsDebug() { return true; }
		static void Debug(const std::string &msg) { LogBus::Log(LogType::Debug, msg); }
		static void Info(const std::string &msg) { LogBus::Log(LogType::Info, msg); }
		static void Warn(const std::string &msg) { LogBus::Log(LogType::Warn, msg); }
		static void Error(const std::string &msg) { LogBus::Log(LogType::Error, msg); }
		//��ӡ����
		static void PrintSource(const std::string &filename, const std::string &text, int line, int col, int count);
	};
}

