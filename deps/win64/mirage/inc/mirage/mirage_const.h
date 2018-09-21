#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <functional>

/*******************************************
特殊数据类型定义
*******************************************/

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

	//重新定义的数据类型
	typedef unsigned char		byte;		///< 字节数据类型
	typedef unsigned int		uint;		///< 无符号整数
	typedef unsigned long long	uint64;		///< 64位的无符号数
	typedef long long			int64;		///< 64位数字

	static double PI = 3.14159265358979323846;	///< 圆周率
	static char HEX[] = "0123456789ABCDEF";  ///< 转换成大写16进制的字符表
	static char hex[] = "0123456789abcdef";  ///< 转换成小写16进制的字符表
	static char SF[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz~";  ///< 转换成64进制的字符表

#ifdef __cplusplus
}
#endif

/************************************
C++常量 Start
************************************/

const size_t FILE_BUFFER_SIZE = 1024;  ///< 操作文件时的缓存大小

//操作系统类型
const int OS_WIN32 = 1;
const int OS_LINUX = 2;

/************************************
C++常量 End
************************************/

#ifndef MAP
#if TEST
//测试状态下用map，因为用unordered_map的话，在Linux下有static对象的时候,valgrind会报内存泄露
#define MAP map
#else
#define MAP unordered_map
#endif
#endif

#ifndef PTR
/** 智能指针 */
#define PTR(T) shared_ptr<T>
#endif

#ifndef MKPTR
/** 智能指针 */
#define MKPTR(T) make_shared<T>
#endif

#ifndef WPTR
/** 弱引用智能指针 */
#define WPTR weak_ptr<T>
#endif

#ifndef TOPTR
/** 智能指针的类型转换 */
#define TOPTR(T1, T2) static_pointer_cast<T1>(T2)
#endif

#ifndef ASSERT
/** 定义C中用的断言宏 */
#define ASSERT(expr)                                          \
do {                                                          \
	if (!(expr)) {                                            \
		fprintf(stderr,                                       \
			"Assertion failed in %s on line %d: %s\n",        \
			__FILE__,                                         \
			__LINE__,                                         \
				#expr);                                       \
		abort();                                              \
	  	}                                                     \
	} while (0)
#endif

//C语言下需要处理的内容
#ifndef __cplusplus

#if defined(WIN32)
#define inline __inline
#endif

#endif  //__cplusplus

//动态绑定
#ifndef BIND
#define BIND(p1, p2, ...)  bind(&p1, p2, ##__VA_ARGS__)
#define BIND1(p1, p2, ...) bind(&p1, p2, placeholders::_1, ##__VA_ARGS__)
#define BIND2(p1, p2, ...) bind(&p1, p2, placeholders::_1, placeholders::_2, ##__VA_ARGS__)
#define BIND3(p1, p2, ...) bind(&p1, p2, placeholders::_1, placeholders::_2, placeholders::_3, ##__VA_ARGS__)
#define BIND4(p1, p2, ...) bind(&p1, p2, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, ##__VA_ARGS__)
#define BIND5(p1, p2, ...) bind(&p1, p2, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5, ##__VA_ARGS__)
#endif  //BIND

//静态绑定
#ifndef BIND_S
#define BIND_S(p1, ...)  bind(p1, ##__VA_ARGS__)
#define BIND1_S(p1, ...) bind(p1, placeholders::_1, ##__VA_ARGS__)
#define BIND2_S(p1, ...) bind(p1, placeholders::_1, placeholders::_2, ##__VA_ARGS__)
#define BIND3_S(p1, ...) bind(p1, placeholders::_1, placeholders::_2, placeholders::_3, ##__VA_ARGS__)
#define BIND4_S(p1, ...) bind(p1, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, ##__VA_ARGS__)
#define BIND5_S(p1, ...) bind(p1, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5, ##__VA_ARGS__)
#endif //SBIND

#ifdef WIN32

#include <sys/timeb.h>
#include <Winsock2.h>
#include <Windows.h>
#include <mirage/mirage_termcolor.h>

#ifdef DLL_IMPORT
#define C_DLL_API extern "C" __declspec(dllimport)
#else
#define C_DLL_API extern "C" __declspec(dllexport)
#endif
#define STDCALL __stdcall

/** 打印信息 */
#define PRINT(p1)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<p1<<endl; cout<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "GBK")<<flush; }

#ifdef DEBUG
/** 输出调试信息 */
#define LOG_DEBUG(p1)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<"  | C "<<mirage::utils::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::green<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "GBK")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<endl<<flush; }
#else
/** 输出调试信息 */
#define LOG_DEBUG(p1)
#endif
/** 输出系统信息 */
#define LOG_INFO(p1)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<" .| C "<<mirage::utils::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::yellow<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "GBK")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<endl<<flush; }
/** 输出错误信息 */
#define LOG_ERROR(p1)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<"><| C "<<mirage::utils::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::red<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "GBK")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<endl<<flush; }
/** 根据条件判断输出信息 */
#define LOG_ASSERT(p1, p2, p3)	{ bool __FUNCTION____LINE__VALUE=(p1==p2); ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<" ?| C "<<mirage::utils::TimeUtils::MSTimeString()<<" | "<<p3<<" ("<<(__FUNCTION____LINE__VALUE?"OK : ":"Fail : ")<<p1<<(__FUNCTION____LINE__VALUE?" == ":" != ")<<p2<<") "; cout<<(__FUNCTION____LINE__VALUE?termcolor::on_green:termcolor::on_red)<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "GBK")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<endl<<flush; }

#else  //WIN32

#include <sys/time.h>
#include <unistd.h>
#include <mirage/mirage_termcolor.h>

#define C_DLL_API extern "C" 
#define STDCALL __attribute__((__stdcall__))

#define PRINT(p1)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<p1<<endl; cout<<__FUNCTION____LINE__.str()<<flush; }


#ifdef DEBUG
/** 输出调试信息 */
#define LOG_DEBUG(p1)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<"  | C "<<mirage::utils::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::green<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<endl<<flush; }
#else
/** 输出调试信息 */
#define LOG_DEBUG(p1)
#endif
/** 输出系统信息 */
#define LOG_INFO(p1)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<" .| C "<<mirage::utils::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::yellow<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<endl<<flush; }
/** 输出错误信息 */
#define LOG_ERROR(p1)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<"><| C "<<mirage::utils::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::red<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<endl<<flush; }
/** 根据条件判断输出信息 */
#define LOG_ASSERT(p1, p2, p3)	{ bool __FUNCTION____LINE__VALUE=(p1==p2); ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<" ?| C "<<mirage::utils::TimeUtils::MSTimeString()<<" | "<<p3<<" ("<<(__FUNCTION____LINE__VALUE?"OK : ":"Fail : ")<<p1<<(__FUNCTION____LINE__VALUE?" == ":" != ")<<p2<<") "; cout<<(__FUNCTION____LINE__VALUE?termcolor::on_green:termcolor::on_red)<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<endl<<flush; }


#endif  //WIN32

/*******************************************
其它公用宏
*******************************************/

/** 转换成字符串 */
#define TOSTR(p1, p2)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<p2; p1+=__FUNCTION____LINE__.str(); }

/** 生成ID */
#define MKID(p1, p2) (++p1>p2?(p1=1,p1):p1)




