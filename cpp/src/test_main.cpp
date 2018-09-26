#include <iostream>
#include <mirror_sys_api.h>

void main()
{
	std::cout << mirror::sys::SystemInterface::Time() << std::endl;
}

