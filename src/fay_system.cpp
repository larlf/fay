#include <fay_system.h>
#include <mirror_utils_log.h>
#include <string>

using namespace fay;

#pragma unmanaged
void fay::SystemEnv::SystemExceptionHandler(unsigned int u, PEXCEPTION_POINTERS pExp)
{
	//std::string message(pExp->ContextRecord);
	throw SEHException("my_trans_func");
}

#pragma unmanaged
void fay::SystemEnv::SystemPurecallHandler()
{
	throw SEHException("myPurecallHandler");
}

#pragma unmanaged
void fay::SystemEnv::SystemInvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved)
{
	throw SEHException("myInvalidParameterHandler");
}

void fay::SystemEnv::SignalHandler(int signal)
{
	throw SEHException("SignalHandler");
}

void fay::SystemEnv::Init()
{
	LOG_DEBUG("Init System Env");

	_set_se_translator(SystemEnv::SystemExceptionHandler);
	_set_invalid_parameter_handler(SystemEnv::SystemInvalidParameterHandler);
	_set_purecall_handler(SystemEnv::SystemPurecallHandler);

	//signal(SIGSEGV, SignalHandler);
}
