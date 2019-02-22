#include <fay_system.h>
#include <fay_log.h>
#include <mirror.h>
#include <string>
#include <thread>

using namespace fay;

#pragma unmanaged
void fay::SystemEnv::SystemExceptionHandler(unsigned int u, PEXCEPTION_POINTERS pExp)
{
	switch(pExp->ExceptionRecord->ExceptionCode)
	{
		case EXCEPTION_ACCESS_VIOLATION:
			throw SEHException(1, "EXCEPTION_ACCESS_VIOLATION");
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			throw SEHException(2, "EXCEPTION_INT_DIVIDE_BY_ZERO");
		case EXCEPTION_STACK_OVERFLOW:
			throw SEHException(3, "EXCEPTION_STACK_OVERFLOW");
		case EXCEPTION_GUARD_PAGE:
			throw SEHException(4, "EXCEPTION_GUARD_PAGE");
	}

	//std::string message(pExp->ContextRecord);
	throw SEHException(0, "Unknow Exception");
}

#pragma unmanaged
void fay::SystemEnv::SystemPurecallHandler()
{
	throw SEHException(0, "myPurecallHandler");
}

#pragma unmanaged
void fay::SystemEnv::SystemInvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved)
{
	throw SEHException(0, "myInvalidParameterHandler");
}

void fay::SystemEnv::SignalHandler(int signal)
{
	throw SEHException(0, "SignalHandler");
}

size_t fay::SystemEnv::CPUS = 0;

void fay::SystemEnv::Init()
{
	LogBus::Debug("\n[SetupSystem]");

	SystemEnv::CPUS = std::thread::hardware_concurrency();
	LogBus::Debug(TOSTR("CPUS : "<<SystemEnv::CPUS));

	_set_se_translator(SystemEnv::SystemExceptionHandler);
	_set_invalid_parameter_handler(SystemEnv::SystemInvalidParameterHandler);
	_set_purecall_handler(SystemEnv::SystemPurecallHandler);

	//signal(SIGSEGV, SignalHandler);
}
