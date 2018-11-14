#pragma once
#include <fay_lang.h>

namespace fay
{
	class ObjectClass : public FayClass
	{
	public:
		PTR(FayClass) init();
	};
}
