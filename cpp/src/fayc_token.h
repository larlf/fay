#pragma once

#include <mirror_sys_const.h>
#include "fayc_const.h"

namespace fayc
{
	class Token
	{
	public:
	};

	class ITokenRule
	{
	public:
		//在什么模式下有效
		virtual LexMode mode() = 0;
		//是否生成Token
		virtual PTR(Token) match(char* data, int pos, int line, int col) = 0;
	};
}