#pragma once
#include <mirror.h>
#include <fay_const.h>
#include <fay_object.h>
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

	//һ��Log����
	class LogData
	{
	public:
		LogType type = LogType::Debug;
		std::string msg;
		std::string filename;
		int line = 0;
		int col = 0;
		int count = 0;

		LogData(LogType type, const std::string &msg, const std::string &filename, int line, int col, int count)
			: type(type), msg(msg), filename(filename), line(line), col(col), count(count) {}
	};
}

