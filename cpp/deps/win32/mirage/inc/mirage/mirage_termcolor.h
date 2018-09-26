#pragma once

//!
//! termcolor
//! ~~~~~~~~~
//!
//! termcolor is a header-only c++ library for printing colored messages
//! to the terminal. Written just for fun with a help of the Force.
//!
//! :copyright: (c) 2013 by Igor Kalnitsky
//! :license: BSD, see LICENSE for details
//!

// the following snippet of code detects the current OS and
// defines the appropriate macro that is used to wrap some
// platform specific things
#if defined(_WIN32) || defined(_WIN64)
#   define OS_WINDOWS
#elif defined(__APPLE__)
#   define OS_MACOS
#elif defined(linux) || defined(__linux) || defined(__CYGWIN__)
#   define OS_LINUX
#else
#   error unsupported platform
#endif


// This headers provides the `isatty()`/`fileno()` functions,
// which are used for testing whether a standart stream refers
// to the terminal. As for Windows, we also need WinApi funcs
// for changing colors attributes of the terminal.
#if defined(OS_MACOS) || defined(OS_LINUX)
#   include <unistd.h>
#elif defined(OS_WINDOWS)
#   include <io.h>
#   include <windows.h>
#endif


#include <iostream>
#include <cstdio>

using namespace std;

namespace termcolor
{
	class TermColorUtils
	{
	public:
		static FILE* get_standard_stream(const ostream& stream);
		static bool is_atty(const ostream& stream);

#ifdef WIN32
		static void win_change_attributes(ostream& stream, int foreground, int background = -1);
#endif
	};

    inline
    ostream& reset(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[00m";
        #elif defined(OS_WINDOWS)
            TermColorUtils::win_change_attributes(stream, -1, -1);
        #endif
        }
        return stream;
    }


    inline
    ostream& bold(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[1m";
        #elif defined(OS_WINDOWS)
        #endif
        }
        return stream;
    }


    inline
    ostream& dark(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[2m";
        #elif defined(OS_WINDOWS)
        #endif
        }
        return stream;
    }


    inline
    ostream& underline(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[4m";
        #elif defined(OS_WINDOWS)
        #endif
        }
        return stream;
    }


    inline
    ostream& blink(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[5m";
        #elif defined(OS_WINDOWS)
        #endif
        }
        return stream;
    }


    inline
    ostream& reverse(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[7m";
        #elif defined(OS_WINDOWS)
        #endif
        }
        return stream;
    }


    inline
    ostream& concealed(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[8m";
        #elif defined(OS_WINDOWS)
        #endif
        }
        return stream;
    }


    inline
    ostream& grey(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[30m";
        #elif defined(OS_WINDOWS)
            TermColorUtils::win_change_attributes(stream,
                0   // grey (black)
            );
        #endif
        }
        return stream;
    }

    inline
    ostream& red(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[31m";
        #elif defined(OS_WINDOWS)
            TermColorUtils::win_change_attributes(stream,
                FOREGROUND_RED | FOREGROUND_INTENSITY
            );
        #endif
        }
        return stream;
    }

    inline
    ostream& green(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[32m";
        #elif defined(OS_WINDOWS)
            TermColorUtils::win_change_attributes(stream,
                FOREGROUND_GREEN | FOREGROUND_INTENSITY
            );
        #endif
        }
        return stream;
    }

    inline
    ostream& yellow(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[33m";
        #elif defined(OS_WINDOWS)
            TermColorUtils::win_change_attributes(stream,
                FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
            );
        #endif
        }
        return stream;
    }

    inline
    ostream& blue(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[34m";
        #elif defined(OS_WINDOWS)
            TermColorUtils::win_change_attributes(stream,
                FOREGROUND_BLUE | FOREGROUND_INTENSITY
            );
        #endif
        }
        return stream;
    }

    inline
    ostream& magenta(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[35m";
        #elif defined(OS_WINDOWS)
            TermColorUtils::win_change_attributes(stream,
                FOREGROUND_BLUE | FOREGROUND_RED
            );
        #endif
        }
        return stream;
    }

    inline
    ostream& cyan(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[36m";
        #elif defined(OS_WINDOWS)
            TermColorUtils::win_change_attributes(stream,
                FOREGROUND_BLUE | FOREGROUND_GREEN
            );
        #endif
        }
        return stream;
    }

    inline
    ostream& white(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[37m";
        #elif defined(OS_WINDOWS)
            TermColorUtils::win_change_attributes(stream,
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED
            );
        #endif
        }
        return stream;
    }



    inline
    ostream& on_grey(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[40m";
        #elif defined(OS_WINDOWS)
            TermColorUtils::win_change_attributes(stream, -1,
                0   // grey (black)
            );
        #endif
        }
        return stream;
    }

    inline
    ostream& on_red(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[41m";
        #elif defined(OS_WINDOWS)
            TermColorUtils::win_change_attributes(stream, -1,
                BACKGROUND_RED
            );
        #endif
        }
        return stream;
    }

    inline
    ostream& on_green(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[42m";
        #elif defined(OS_WINDOWS)
            TermColorUtils::win_change_attributes(stream, -1,
                BACKGROUND_GREEN
            );
        #endif
        }
        return stream;
    }

    inline
    ostream& on_yellow(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[43m";
        #elif defined(OS_WINDOWS)
            TermColorUtils::win_change_attributes(stream, -1,
                BACKGROUND_GREEN | BACKGROUND_RED
            );
        #endif
        }
        return stream;
    }

    inline
    ostream& on_blue(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[44m";
        #elif defined(OS_WINDOWS)
            TermColorUtils::win_change_attributes(stream, -1,
                BACKGROUND_BLUE
            );
        #endif
        }
        return stream;
    }

    inline
    ostream& on_magenta(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[45m";
        #elif defined(OS_WINDOWS)
            TermColorUtils::win_change_attributes(stream, -1,
                BACKGROUND_BLUE | BACKGROUND_RED
            );
        #endif
        }
        return stream;
    }

    inline
    ostream& on_cyan(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[46m";
        #elif defined(OS_WINDOWS)
            TermColorUtils::win_change_attributes(stream, -1,
                BACKGROUND_GREEN | BACKGROUND_BLUE
            );
        #endif
        }
        return stream;
    }

    inline
    ostream& on_white(ostream& stream)
    {
        if (TermColorUtils::is_atty(stream))
        {
        #if defined(OS_MACOS) || defined(OS_LINUX)
            stream << "\033[47m";
        #elif defined(OS_WINDOWS)
            TermColorUtils::win_change_attributes(stream, -1,
                BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED
            );
        #endif
        }

        return stream;
    }

} // namespace termcolor


#undef OS_WINDOWS
#undef OS_MACOS
#undef OS_LINUX

