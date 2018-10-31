#include <fay_internal_fun.h>

void fay::InternalFun::Print_String(std::stack<FayValue>& stack)
{
	std::string val = stack.top().strVal();
	stack.pop();
	std::cout << val << std::endl;
}

void fay::InternalFun::Print_Int(std::stack<FayValue>& stack)
{
	int val = stack.top().intVal();
	stack.pop();
	std::cout << val << std::endl;
}
