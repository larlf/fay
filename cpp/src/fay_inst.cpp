#include <fay_inst.h>

void fay::InstPushString::toString(mirror::utils::StringBuilder* sb)
{
	sb->add("InstPushString ")->add(this->_p1)->endl();
}

void fay::InstCall::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("InstCall ")->add(this->_p1)->add(" ")->add(this->_p2)->endl();
}
