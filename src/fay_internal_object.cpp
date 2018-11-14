#include "fay_internal_object.h"

using namespace fay;

PTR(FayClass) fay::ObjectClass::init()
{
	return this->shared_from_this();
}



