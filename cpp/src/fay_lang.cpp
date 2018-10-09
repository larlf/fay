#include <fay_lang.h>

void fay::FayLib::addClass(PTR(FayClass) clazz)
{
	this->_classes.push_back(clazz);
}
