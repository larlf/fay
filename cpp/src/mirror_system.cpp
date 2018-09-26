#include "mirror_system.h"

uint64_t mirror::SystemInterface::MSTime()
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
