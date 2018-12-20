#pragma once
#ifdef WIN32
#include <windows.h>
#include <eh.h>
#include <signal.h>
#endif
#include <exception>
#include <string>

//����ļ����Ҫ�ǶԲ�ͬϵͳƽ̨�ϵ�һЩ���⴦��

namespace fay
{
	class SystemEnv
	{
	private:
		static void SystemExceptionHandler(unsigned int u, PEXCEPTION_POINTERS pExp);
		static void SystemPurecallHandler();
		static void SystemInvalidParameterHandler(const wchar_t* expression,
			const wchar_t* function,
			const wchar_t* file,
			unsigned int line,
			uintptr_t pReserved);
		static void SignalHandler(int signal);

	public:
		static void Init();
	};

	class SEHException : public std::exception
	{
	private:
		int _code = 0;
	public:
		SEHException(int code, const std::string &message)
			: std::exception(message.c_str()), _code(code) {}

		int code() { return this->_code; }
	};
}