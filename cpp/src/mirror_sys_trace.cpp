#include <mirror_sys_trace.h>

#if WIN32
#include <windows.h>
#include <process.h>
#include <dbghelp.h>
#endif

#include <string>

#pragma comment(lib, "Dbghelp.lib")

std::string mirror::sys::SysTrace::TraceInfo()
{
#if WIN32
	//获取进程
	HANDLE process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);

	//取得调用堆栈
	void* stack[100];
	unsigned short frames = CaptureStackBackTrace(0, 100, stack, NULL);

	//用于取符号信息的数据结构
	SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	//用于取调试信息的数据结构
	DWORD displacement;
	IMAGEHLP_LINE64* line = (IMAGEHLP_LINE64*)malloc(sizeof(IMAGEHLP_LINE64));
	line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);

	//生成堆栈信息
	std::string str;
	std::string indent;
	for (auto i = 0; i < frames; i++)
	{
		DWORD64 address = (DWORD64)(stack[i]);
		SymFromAddr(process, address, NULL, symbol);

		//当前方法不加入进来
		if (strstr(symbol->Name, "::SysTrace::") != NULL)
			continue;

		if (SymGetLineFromAddr64(process, address, &displacement, line))
		{
			if (str.size())
			{
				str.append("\n");
				str.append(indent);
				indent.append(" ");
				str.append("┖ ");
			}

			str.append(symbol->Name).append("() at ");
			str.append(line->FileName).append(":").append(std::to_string(line->LineNumber));
		}
	}

	free(symbol);
	free(line);

	return str;
#else

	return "";

#endif
}

