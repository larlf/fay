#include "mirror_utils_time.h"

#ifdef WIN32
#include <sys/timeb.h>
#include <Winsock2.h>
#include <Windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif
#include <time.h>

using namespace std;

uint32_t mirror::utils::TimeUtils::Time()
{
	return time(0);
}

uint64_t mirror::utils::TimeUtils::MSTime()
{
#ifdef WIN32
	struct timeb tp;
	ftime(&tp);
	return tp.time * 1000 + tp.millitm;
#else
	struct timeval t;
	gettimeofday(&t, 0);
	uint64 s = time(0);
	return s * 1000 + (t.tv_usec / 1000);
#endif
}

std::string mirror::utils::TimeUtils::TimeString(uint32_t t /*= 0*/)
{
	if (t < 1) t = TimeUtils::Time();

	char str[32];
	struct tm *ptr;
	time_t time = (size_t)t;

	ptr = localtime(&time);
	strftime(str, 32, "%y%m%d %H%M%S", ptr);

	return string(str);
}

std::string mirror::utils::TimeUtils::MSTimeString(uint64_t t /*= 0*/)
{
	if (t < 1) t = TimeUtils::MSTime();
	long time = long(t / 1000);
	string r = TimeUtils::TimeString(time);

	//加入毫秒数
	char str[5];
#ifdef __linux__
	sprintf(str, "%-3d", int(t % 1000));
#else
	sprintf_s(str, 5, "%-3d", int(t % 1000));
#endif
	r.append(".");
	r.append(str);

	return r;
}