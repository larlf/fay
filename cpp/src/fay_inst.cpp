#include <fay_inst.h>

void fay::InstPushString::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("InstPushString ")->add(this->_p1)->endl();
}

