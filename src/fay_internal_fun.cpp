#include <fay_internal_fun.h>

void fay::InternalFun::Print_String(std::stack<PTR(FayValue)>& stack)
{
	std::string* val = stack.top()->strVal();
	std::cout << val->c_str() << std::endl;
	stack.pop();
}

void fay::InternalFun::Print_Int(std::stack<PTR(FayValue)>& stack)
{
	int val = stack.top()->intVal();
	std::cout << val << std::endl;
	stack.pop();
}
