#pragma once

#include <mirror_sys_const.h>

namespace fay
{
	class I18N
	{
	private:
		static MAP<std::string, std::string> _dict;

	public:
		static void Init(const std::string &text);
	};
}