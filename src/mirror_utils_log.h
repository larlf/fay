#pragma once

#include <iostream>
#include <sstream>
#include <atomic>
#include <thread>
#include <mirror_sys_termcolor.h>
#include <mirror_utils_string.h>
#include <mirror_utils_time.h>

namespace mirror
{
	namespace log
	{
		class SysTrace
		{
		public:
			static std::string TraceInfo();
		};

		//用于对Log的状态进行控制
		class LogState
		{
		public:
			//自旋锁，防止Log同时输出
			static std::atomic_flag LogLock;
		};
	}
}

#ifdef WIN32

#define _LOG_START(p1, p2) do { \
	std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<##p1##<<mirror::utils::TimeUtils::MSTimeString()<<" | "<<##p2##; \
	std::string str=mirror::utils::StringUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "GBK");
#define _LOG_BODY(p1) while(mirror::log::LogState::LogLock.test_and_set()) { std::this_thread::yield(); }; try{ \
	std::cout<<termcolor::##p1##<<str<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<std::endl; \
	} catch(std::exception& ex) {};
#define _LOG_TRACE std::cout<<mirror::log::SysTrace::TraceInfo()<<std::endl;
#define _LOG_END mirror::log::LogState::LogLock.clear(); \
	} while(0)

/** 打印信息 */
#define PRINT(p1) std::cout<<##p1##<<std::endl;

/** 输出调试信息 */
#ifdef DEBUG
#define LOG_DEBUG(p1) _LOG_START("  | C ", p1)  _LOG_BODY(green) _LOG_END
#else
#define LOG_DEBUG(p1)
#endif
/** 输出系统信息 */
#define LOG_INFO(p1) _LOG_START(" .| C ", p1)  _LOG_BODY(yellow) _LOG_END
/** 输出警告信息 */
#define LOG_WARN(p1) _LOG_START(" !| C ", p1)  _LOG_BODY(magenta) _LOG_END
/** 输出错误信息 */
#define LOG_ERROR(p1) _LOG_START("><| C ", p1)  _LOG_BODY(red) _LOG_TRACE _LOG_END

#else  //WIN32

#define PRINT(p1)	{ std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<p1<<std::endl; cout<<__FUNCTION____LINE__.str()<<std::flush; }

#ifdef DEBUG
/** 输出调试信息 */
#define LOG_DEBUG(p1)	{ std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<"  | C "<<mirror::utils::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::green<<mirror::utils::StringUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<std::endl<<std::flush; }
#else
/** 输出调试信息 */
#define LOG_DEBUG(p1)
#endif
/** 输出系统信息 */
#define LOG_INFO(p1)	{ std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<" .| C "<<mirror::utils::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::yellow<<mirror::utils::StringUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<std::endl<<std::flush; }
/** 输出警告信息 */
#define LOG_WARN(p1)	{ std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<"><| C "<<mirror::utils::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::magenta<<mirror::utils::StringUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<std::endl<<std::flush; }
/** 输出错误信息 */
#define LOG_ERROR(p1)	{ std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<"><| C "<<mirror::utils::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::red<<mirror::utils::StringUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<std::endl<<std::flush; }
/** 根据条件判断输出信息 */
#define LOG_ASSERT(p1, p2, p3)	{ bool __FUNCTION____LINE__VALUE=(p1==p2); std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<" ?| C "<<mirror::utils::TimeUtils::MSTimeString()<<" | "<<p3<<" ("<<(__FUNCTION____LINE__VALUE?"OK : ":"Fail : ")<<p1<<(__FUNCTION____LINE__VALUE?" == ":" != ")<<p2<<") "; cout<<(__FUNCTION____LINE__VALUE?termcolor::on_green:termcolor::on_red)<<mirror::utils::StringUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<std::endl<<std::flush; }


#endif  //WIN32


