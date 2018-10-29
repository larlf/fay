#include <fay_internal_fun.h>

void fay::InternalFun::Print_String(VMStack * stack)
{
	PTR(FayValue) val(stack->pop());
	std::cout << *(val->strVal()) << std::endl;
}

void fay::InternalFun::Print_Int(VMStack * stack)
{
	PTR(FayValue) val(stack->pop());
	std::cout << val->intVal() << std::endl;
}
