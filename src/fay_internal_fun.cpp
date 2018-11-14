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

void fay::InternalFun::ToString(std::stack<FayValue> &stack)
{
	std::string str;
	switch(stack.top().type())
	{
		case fay::ValueType::Void:
			str = "void<" + std::to_string((int64_t)stack.top().ptrValue<void>()) + ">";
			break;
		case fay::ValueType::Bool:
			str = std::to_string(stack.top().boolVal());
			break;
		case fay::ValueType::Byte:
			str = std::to_string(stack.top().byteVal());
			break;
		case fay::ValueType::Int:
			str = std::to_string(stack.top().intVal());
			break;
		case fay::ValueType::Long:
			str = std::to_string(stack.top().longVal());
			break;
		case fay::ValueType::Float:
			str = std::to_string(stack.top().floatVal());
			break;
		case fay::ValueType::Double:
			str = std::to_string(stack.top().doubleVal());
			break;
		case fay::ValueType::String:
			str = *stack.top().strVal();
			break;
		case fay::ValueType::Object:
			str = "object<" + std::to_string((int64_t)stack.top().ptrValue<void>()) + ">";
			break;
		case fay::ValueType::Function:
			str = "fun<" + std::to_string((int64_t)stack.top().ptrValue<void>()) + ">";
			break;
		default:
			break;
	}

	stack.pop();
	stack.push(FayValue(str));
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
