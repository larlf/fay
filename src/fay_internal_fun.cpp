#include <fay_internal_fun.h>
#include <mirror_utils_time.h>

using namespace mirror;

void fay::InternalFun::Print_String(std::stack<FayValue> &stack)
{
	std::cout << stack.top().strVal()->c_str() << std::endl;
	stack.pop();
}

void fay::InternalFun::MSTime(std::stack<FayValue> &stack)
{
	int64_t ms = utils::TimeUtils::MSTime();
	stack.push(FayValue(ms));
}

void fay::InternalFun::Print_Int(std::stack<FayValue> &stack)
{
	std::cout << stack.top().intVal() << std::endl;
	stack.pop();
}

void fay::InternalFun::Print_Long(std::stack<FayValue> &stack)
{
	std::cout << stack.top().longVal() << std::endl;
	stack.pop();
}

void fay::InternalFun::Print_Float(std::stack<FayValue> &stack)
{
	std::cout << stack.top().floatVal() << std::endl;
	stack.pop();
}

void fay::InternalFun::Print_Double(std::stack<FayValue> &stack)
{
	std::cout << stack.top().doubleVal() << std::endl;
	stack.pop();
}

void fay::InternalFun::Print_Bool(std::stack<FayValue> &stack)
{
	std::cout << (stack.top().boolVal() ? "true" : "false") << std::endl;
	stack.pop();
}

void fay::InternalFun::Print_Byte(std::stack<FayValue> &stack)
{
	std::cout << stack.top().byteVal() << std::endl;
	stack.pop();
}
