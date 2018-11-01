#include <fay_internal_fun.h>

void fay::InternalFun::Print_String(std::stack<PTR(FayValue)> &stack)
{
	std::cout << stack.top()->strVal()->c_str() << std::endl;
	stack.pop();
}

void fay::InternalFun::Print_Int(std::stack<PTR(FayValue)> &stack)
{
	std::cout << stack.top()->intVal() << std::endl;
	stack.pop();
}

void fay::InternalFun::Print_Long(std::stack<PTR(FayValue)> &stack)
{
	std::cout << stack.top()->longVal() << std::endl;
	stack.pop();
}

void fay::InternalFun::Print_Float(std::stack<PTR(FayValue)> &stack)
{
	std::cout << stack.top()->floatVal() << std::endl;
	stack.pop();
}

void fay::InternalFun::Print_Double(std::stack<PTR(FayValue)> &stack)
{
	std::cout << stack.top()->doubleVal() << std::endl;
	stack.pop();
}

void fay::InternalFun::Print_Bool(std::stack<PTR(FayValue)> &stack)
{
	std::cout << (stack.top()->boolVal() ? "true" : "false") << std::endl;
	stack.pop();
}

void fay::InternalFun::Print_Byte(std::stack<PTR(FayValue)> &stack)
{
	std::cout << stack.top()->byteVal() << std::endl;
	stack.pop();
}
