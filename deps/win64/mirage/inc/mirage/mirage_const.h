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
�����������Ͷ���
*******************************************/

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

	//���¶������������
	typedef unsigned char		byte;		///< �ֽ���������
	typedef unsigned int		uint;		///< �޷�������
	typedef unsigned long long	uint64;		///< 64λ���޷�����
	typedef long long			int64;		///< 64λ����

	static double PI = 3.14159265358979323846;	///< Բ����
	static char HEX[] = "0123456789ABCDEF";  ///< ת���ɴ�д16���Ƶ��ַ���
	static char hex[] = "0123456789abcdef";  ///< ת����Сд16���Ƶ��ַ���
	static char SF[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz~";  ///< ת����64���Ƶ��ַ���

#ifdef __cplusplus
}
#endif

/************************************
C++���� Start
************************************/

const size_t FILE_BUFFER_SIZE = 1024;  ///< �����ļ�ʱ�Ļ����С

//����ϵͳ����
const int OS_WIN32 = 1;
const int OS_LINUX = 2;

/************************************
C++���� End
************************************/

#ifndef MAP
#if TEST
//����״̬����map����Ϊ��unordered_map�Ļ�����Linux����static�����ʱ��,valgrind�ᱨ�ڴ�й¶
#define MAP map
#else
#define MAP unordered_map
#endif
#endif

#ifndef PTR
/** ����ָ�� */
#define PTR(T) shared_ptr<T>
#endif

#ifndef MKPTR
/** ����ָ�� */
#define MKPTR(T) make_shared<T>
#endif

#ifndef WPTR
/** ����������ָ�� */
#define WPTR weak_ptr<T>
#endif

#ifndef TOPTR
/** ����ָ�������ת�� */
#define TOPTR(T1, T2) static_pointer_cast<T1>(T2)
#endif

#ifndef ASSERT
/** ����C���õĶ��Ժ� */
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

//C��������Ҫ���������
#ifndef __cplusplus

#if defined(WIN32)
#define inline __inline
#endif

#endif  //__cplusplus

//��̬��
#ifndef BIND
#define BIND(p1, p2, ...)  bind(&p1, p2, ##__VA_ARGS__)
#define BIND1(p1, p2, ...) bind(&p1, p2, placeholders::_1, ##__VA_ARGS__)
#define BIND2(p1, p2, ...) bind(&p1, p2, placeholders::_1, placeholders::_2, ##__VA_ARGS__)
#define BIND3(p1, p2, ...) bind(&p1, p2, placeholders::_1, placeholders::_2, placeholders::_3, ##__VA_ARGS__)
#define BIND4(p1, p2, ...) bind(&p1, p2, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, ##__VA_ARGS__)
#define BIND5(p1, p2, ...) bind(&p1, p2, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5, ##__VA_ARGS__)
#endif  //BIND

//��̬��
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

/** ��ӡ��Ϣ */
#define PRINT(p1)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<p1<<endl; cout<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "GBK")<<flush; }

#ifdef DEBUG
/** ���������Ϣ */
#define LOG_DEBUG(p1)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<"  | C "<<mirage::utils::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::green<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "GBK")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<endl<<flush; }
#else
/** ���������Ϣ */
#define LOG_DEBUG(p1)
#endif
/** ���ϵͳ��Ϣ */
#define LOG_INFO(p1)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<" .| C "<<mirage::utils::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::yellow<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "GBK")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<endl<<flush; }
/** ���������Ϣ */
#define LOG_ERROR(p1)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<"><| C "<<mirage::utils::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::red<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "GBK")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<endl<<flush; }
/** ���������ж������Ϣ */
#define LOG_ASSERT(p1, p2, p3)	{ bool __FUNCTION____LINE__VALUE=(p1==p2); ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<" ?| C "<<mirage::utils::TimeUtils::MSTimeString()<<" | "<<p3<<" ("<<(__FUNCTION____LINE__VALUE?"OK : ":"Fail : ")<<p1<<(__FUNCTION____LINE__VALUE?" == ":" != ")<<p2<<") "; cout<<(__FUNCTION____LINE__VALUE?termcolor::on_green:termcolor::on_red)<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "GBK")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<endl<<flush; }

#else  //WIN32

#include <sys/time.h>
#include <unistd.h>
#include <mirage/mirage_termcolor.h>

#define C_DLL_API extern "C" 
#define STDCALL __attribute__((__stdcall__))

#define PRINT(p1)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<p1<<endl; cout<<__FUNCTION____LINE__.str()<<flush; }


#ifdef DEBUG
/** ���������Ϣ */
#define LOG_DEBUG(p1)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<"  | C "<<mirage::utils::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::green<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<endl<<flush; }
#else
/** ���������Ϣ */
#define LOG_DEBUG(p1)
#endif
/** ���ϵͳ��Ϣ */
#define LOG_INFO(p1)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<" .| C "<<mirage::utils::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::yellow<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<endl<<flush; }
/** ���������Ϣ */
#define LOG_ERROR(p1)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<"><| C "<<mirage::utils::TimeUtils::MSTimeString()<<" | "<<p1; cout<<termcolor::red<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<endl<<flush; }
/** ���������ж������Ϣ */
#define LOG_ASSERT(p1, p2, p3)	{ bool __FUNCTION____LINE__VALUE=(p1==p2); ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<" ?| C "<<mirage::utils::TimeUtils::MSTimeString()<<" | "<<p3<<" ("<<(__FUNCTION____LINE__VALUE?"OK : ":"Fail : ")<<p1<<(__FUNCTION____LINE__VALUE?" == ":" != ")<<p2<<") "; cout<<(__FUNCTION____LINE__VALUE?termcolor::on_green:termcolor::on_red)<<mirage::utils::TextUtils::Encoding(__FUNCTION____LINE__.str(), "UTF-8", "UTF-8")<<termcolor::reset<<" ... "<< __FUNCTION__<<"() "<<__FILE__<<"("<<__LINE__<<")"<<endl<<flush; }


#endif  //WIN32

/*******************************************
�������ú�
*******************************************/

/** ת�����ַ��� */
#define TOSTR(p1, p2)	{ ostringstream __FUNCTION____LINE__; __FUNCTION____LINE__<<p2; p1+=__FUNCTION____LINE__.str(); }

/** ����ID */
#define MKID(p1, p2) (++p1>p2?(p1=1,p1):p1)




