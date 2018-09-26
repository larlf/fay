#pragma once

#include <iostream>
#include <sstream>
#include "mirror_sys_termcolor.h"
#include "mirror_util_string.h"
#include "mirror_util_time.h"

#ifdef WIN32

/** 打印信息 */
#define PRINT(p1)	{ std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<p1<<std::endl; cout<<mirror::util::StringUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "GBK")<<std::flush; }

#ifdef DEBUG
/** 输出调试信息 */
#define LOG_DEBUG(p1)	{ std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<"  | C "<<mirror::util::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::green<<mirror::util::StringUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "GBK")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<std::endl<<std::flush; }
#else
/** 输出调试信息 */
#define LOG_DEBUG(p1)
#endif
/** 输出系统信息 */
#define LOG_INFO(p1)	{ std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<" .| C "<<mirror::util::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::yellow<<mirror::util::StringUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "GBK")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<std::endl<<std::flush; }
/** 输出警告信息 */
#define LOG_WARN(p1)	{ std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<" !| C "<<mirror::util::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::magenta<<mirror::util::StringUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "GBK")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<std::endl<<std::flush; }
/** 输出错误信息 */
#define LOG_ERROR(p1)	{ std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<"><| C "<<mirror::util::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::red<<mirror::util::StringUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "GBK")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<std::endl<<std::flush; }
/** 根据条件判断输出信息 */
#define LOG_ASSERT(p1, p2, p3)	{ bool __FUNCTION____LINE__VALUE=(p1==p2); std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<" ?| C "<<mirror::util::TimeUtils::MSTimeString()<<" | "<<p3<<" ("<<(__FUNCTION____LINE__VALUE?"OK : ":"Fail : ")<<p1<<(__FUNCTION____LINE__VALUE?" == ":" != ")<<p2<<") "; cout<<(__FUNCTION____LINE__VALUE?termcolor::on_green:termcolor::on_red)<<mirror::util::StringUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "GBK")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<std::endl<<std::flush; }

#else  //WIN32

#define PRINT(p1)	{ std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<p1<<std::endl; cout<<__FUNCTION____LINE__.str()<<std::flush; }

#ifdef DEBUG
/** 输出调试信息 */
#define LOG_DEBUG(p1)	{ std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<"  | C "<<mirror::util::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::green<<mirror::util::StringUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<std::endl<<std::flush; }
#else
/** 输出调试信息 */
#define LOG_DEBUG(p1)
#endif
/** 输出系统信息 */
#define LOG_INFO(p1)	{ std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<" .| C "<<mirror::util::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::yellow<<mirror::util::StringUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<std::endl<<std::flush; }
/** 输出警告信息 */
#define LOG_WARN(p1)	{ std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<"><| C "<<mirror::util::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::magenta<<mirror::util::StringUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<std::endl<<std::flush; }
/** 输出错误信息 */
#define LOG_ERROR(p1)	{ std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<"><| C "<<mirror::util::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::red<<mirror::util::StringUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<std::endl<<std::flush; }
/** 根据条件判断输出信息 */
#define LOG_ASSERT(p1, p2, p3)	{ bool __FUNCTION____LINE__VALUE=(p1==p2); std::ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<" ?| C "<<mirror::util::TimeUtils::MSTimeString()<<" | "<<p3<<" ("<<(__FUNCTION____LINE__VALUE?"OK : ":"Fail : ")<<p1<<(__FUNCTION____LINE__VALUE?" == ":" != ")<<p2<<") "; cout<<(__FUNCTION____LINE__VALUE?termcolor::on_green:termcolor::on_red)<<mirror::util::StringUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<std::endl<<std::flush; }


#endif  //WIN32


