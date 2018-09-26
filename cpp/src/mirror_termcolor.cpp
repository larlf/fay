#include "mirror_termcolor.h"

FILE* termcolor::TermColorUtils::get_standard_stream(const ostream& stream)
{
	if (&stream == &cout)
		return stdout;
	else if ((&stream == &cerr) || (&stream == &clog))
		return stderr;

	return nullptr;
}

bool termcolor::TermColorUtils::is_atty(const ostream& stream)
{
	FILE* std_stream = get_standard_stream(stream);

#ifdef WIN32
	return ::_isatty(_fileno(std_stream));
#else
	return ::isatty(fileno(std_stream));
#endif
}

#ifdef WIN32
void termcolor::TermColorUtils::win_change_attributes(ostream& stream, int foreground, int background /*= -1*/)
{
	// yeah, i know.. it's ugly, it's windows.
	static WORD defaultAttributes = 0;

	// get terminal handle
	HANDLE hTerminal = INVALID_HANDLE_VALUE;
	if (&stream == &cout)
		hTerminal = GetStdHandle(STD_OUTPUT_HANDLE);
	else if (&stream == &cerr)
		hTerminal = GetStdHandle(STD_ERROR_HANDLE);

	// save default terminal attributes if it unsaved
	if (!defaultAttributes)
	{
		CONSOLE_SCREEN_BUFFER_INFO info;
		if (!GetConsoleScreenBufferInfo(hTerminal, &info))
			return;
		defaultAttributes = info.wAttributes;
	}

	// restore all default settings
	if (foreground == -1 && background == -1)
	{
		SetConsoleTextAttribute(hTerminal, defaultAttributes);
		return;
	}

	// get current settings
	CONSOLE_SCREEN_BUFFER_INFO info;
	if (!GetConsoleScreenBufferInfo(hTerminal, &info))
		return;

	if (foreground != -1)
	{
		info.wAttributes &= ~(info.wAttributes & 0x0F);
		info.wAttributes |= static_cast<WORD>(foreground);
	}

	if (background != -1)
	{
		info.wAttributes &= ~(info.wAttributes & 0xF0);
		info.wAttributes |= static_cast<WORD>(background);
	}

	SetConsoleTextAttribute(hTerminal, info.wAttributes);
}
#endif
