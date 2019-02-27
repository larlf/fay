#pragma once
#include <mirror.h>
#include <fay_const.h>
#include <fay_object.h>
#include <thread>
#include <mutex>

namespace fay
{
	//Log的类型
	enum class LogType
	{
		Debug,
		Info,
		Warn,
		Error,
		_Max
	};

	//一条Log数据
	class LogData
	{
	public:
		LogType type = LogType::Debug;
		std::string msg;
		std::string trace;
		PTR(fay::FilePart) part;

		LogData(LogType type, const std::string &msg)
			: type(type), msg(msg) {}
		LogData(LogType type, const std::string &msg, PTR(FilePart) part)
			: type(type), msg(msg), part(part) {}
		LogData(LogType type, const std::string &msg, PTR(FilePart) part, const std::string &trace)
			: type(type), msg(msg), part(part), trace(trace) {}
	};

	//用于统一处理Log的部分
	class LogBus
	{
	private:
		static std::thread::id _MainThreadID;
		static std::string _LogFile;
		static std::recursive_mutex _LogLock;
		static size_t _Count[];  //Log的计数器

		//用于保存当前线程的Log
		static MAP<std::thread::id, PTR(std::vector<PTR(LogData)>)> _Loggers;
		//查找当前线程的Logger
		static PTR(std::vector<PTR(LogData)>) _GetThreadLogger(const std::thread::id &id);
		//删除当前线程的Log
		static void _RemoveThreadLogger(const std::thread::id &id);
		//输出Log
		static void _Log(LogType type, const std::string &msg, PTR(FilePart) part, const std::string &trace);

	public:

		static void Init(const std::string &filename = "");
		static bool IsDebug() { return true; }

		static void Log(LogType type, const std::string &msg, PTR(FilePart) part, const std::string &trace);
		static void Debug(const std::string &msg, PTR(FilePart) part = nullptr, const std::string &trace = "") { LogBus::Log(LogType::Debug, msg, part, trace); }
		static void Info(const std::string &msg, PTR(FilePart) part = nullptr, const std::string &trace = "") { LogBus::Log(LogType::Info, msg, part, trace); }
		static void Warn(const std::string &msg, PTR(FilePart) part = nullptr, const std::string &trace = "") { LogBus::Log(LogType::Warn, msg, part, trace); }
		static void Error(const std::string &msg, PTR(FilePart) part = nullptr, const std::string &trace = "") { LogBus::Log(LogType::Error, msg, part, trace); }

		//输出当前线程下的Log
		static void EndThread();
		//清理并输出所有缓存的Log记录
		static void EndAllThread();

		//重置Log的计数器
		static void ResetCount();
		//取得指定类型Log输出的数量
		static size_t Count(LogType type);

		//打印代码
		//static void PrintSource(const std::string &filename, const std::string &text, int line, int col, int count);
	};


}

