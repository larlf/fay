#include <fay_internal_fun.h>

void fay::InternalFun::Print(VMStack * stack)
{
	PTR(FayValue) val(stack->push());
	std::cout << val->strVal() << std::endl;
}
