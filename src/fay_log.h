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

	//һ��Log����
	class LogData
	{
	public:
		LogType type = LogType::Debug;
		std::string msg;
		PTR(fay::FilePart) part;

		LogData(LogType type, const std::string &msg)
			: type(type), msg(msg) {}
		LogData(LogType type, const std::string &msg, PTR(FilePart) part)
			: type(type), msg(msg), part(part) {}
	};

	//����ͳһ����Log�Ĳ���
	class LogBus
	{
	private:
		static std::thread::id _MainThreadID;
		static std::string _LogFile;

		//���ڱ��浱ǰ�̵߳�Log
		static MAP<std::thread::id, PTR(std::vector<PTR(LogData)>)> _Loggers;
		//���ҵ�ǰ�̵߳�Logger
		static PTR(std::vector<PTR(LogData)>) _GetThreadLogger(const std::thread::id &id);
		//ɾ����ǰ�̵߳�Log
		static void _RemoveThreadLogger(const std::thread::id &id);
		//���Log
		static void _Log(LogType type, const std::string &msg, PTR(FilePart) part);

	public:
		static std::recursive_mutex LogLock;

		static void Init(const std::string &filename = "");
		static bool IsDebug() { return true; }

		static void Log(LogType type, const std::string &msg, PTR(FilePart) part);
		static void Debug(const std::string &msg, PTR(FilePart) part = nullptr) { LogBus::Log(LogType::Debug, msg, part); }
		static void Info(const std::string &msg, PTR(FilePart) part = nullptr) { LogBus::Log(LogType::Info, msg, part); }
		static void Warn(const std::string &msg, PTR(FilePart) part = nullptr) { LogBus::Log(LogType::Warn, msg, part); }
		static void Error(const std::string &msg, PTR(FilePart) part = nullptr) { LogBus::Log(LogType::Error, msg, part); }

		//�����ǰ�߳��µ�Log
		static void EndThread();
		//����������л����Log��¼
		static void Clear();

		//��ӡ����
		//static void PrintSource(const std::string &filename, const std::string &text, int line, int col, int count);
	};


}

