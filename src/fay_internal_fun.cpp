#include <fay_internal_fun.h>
#include <mirror_utils_time.h>

using namespace mirror;

void fay::internal::InternalFun::Print_String(PTR(std::stack<FayValue>) stack)
{
	std::cout << stack->top().strVal()->c_str() << std::endl;
	stack->pop();
}

void fay::internal::InternalFun::MSTime(PTR(std::stack<FayValue>) stack)
{
	int64_t ms = utils::TimeUtils::MSTime();
	stack->push(FayValue(ms));
}

void fay::internal::InternalFun::StringToBool(PTR(std::stack<FayValue>) stack)
{
	FayValue v = stack->top();
	stack->pop();

	try
	{
		if((*v.strVal()).empty())
			stack->push(FayValue(false));
		else
			stack->push(FayValue(true));
	}
	catch(std::exception &ex)
	{
		LOG_ERROR("Convert string to bool error : " << *v.strVal());
		stack->push(FayValue(false));
	}
}

void fay::internal::InternalFun::StringToByte(PTR(std::stack<FayValue>) stack)
{
	FayValue v = stack->top();
	stack->pop();

	try
	{
		stack->push(FayValue((byte)std::stoi(*v.strVal())));
	}
	catch(std::exception &ex)
	{
		LOG_ERROR("Convert string to byte error : " << *v.strVal());
		stack->push(FayValue((byte)0));
	}
}

void fay::internal::InternalFun::StringToInt(PTR(std::stack<FayValue>) stack)
{
	FayValue v = stack->top();
	stack->pop();

	try
	{
		stack->push(FayValue(std::stoi(*v.strVal())));
	}
	catch(std::exception &ex)
	{
		LOG_ERROR("Convert string to int error : " << *v.strVal());
		stack->push(FayValue((int32_t)0));
	}
}

void fay::internal::InternalFun::StringToLong(PTR(std::stack<FayValue>) stack)
{
	FayValue v = stack->top();
	stack->pop();

	try
	{
		stack->push(FayValue((int64_t)std::stol(*v.strVal())));
	}
	catch(std::exception &ex)
	{
		LOG_ERROR("Convert string to long error : " << *v.strVal());
		stack->push(FayValue((int64_t)0));
	}
}

void fay::internal::InternalFun::StringToFloat(PTR(std::stack<FayValue>) stack)
{
	FayValue v = stack->top();
	stack->pop();

	try
	{
		stack->push(FayValue((float)std::stof(*v.strVal())));
	}
	catch(std::exception &ex)
	{
		LOG_ERROR("Convert string to float error : " << *v.strVal());
		stack->push(FayValue((float)0));
	}
}

void fay::internal::InternalFun::StringToDouble(PTR(std::stack<FayValue>) stack)
{
	FayValue v = stack->top();
	stack->pop();

	try
	{
		stack->push(FayValue((double)std::stod(*v.strVal())));
	}
	catch(std::exception &ex)
	{
		LOG_ERROR("Convert string to double error : " << *v.strVal());
		stack->push(FayValue((double)0));
	}
}

void fay::internal::InternalFun::Print_Int(PTR(std::stack<FayValue>) stack)
{
	std::cout << stack->top().intVal() << std::endl;
	stack->pop();
}

void fay::internal::InternalFun::Print_Long(PTR(std::stack<FayValue>) stack)
{
	std::cout << stack->top().longVal() << std::endl;
	stack->pop();
}

void fay::internal::InternalFun::Print_Float(PTR(std::stack<FayValue>) stack)
{
	std::cout << stack->top().floatVal() << std::endl;
	stack->pop();
}

void fay::internal::InternalFun::Print_Double(PTR(std::stack<FayValue>) stack)
{
	std::cout << stack->top().doubleVal() << std::endl;
	stack->pop();
}

void fay::internal::InternalFun::ToString(PTR(std::stack<FayValue>) stack)
{
	std::string str;
	switch(stack->top().type())
	{
		case fay::ValueType::Void:
			str = "void<" + std::to_string((int64_t)stack->top().ptrValue<void>()) + ">";
			break;
		case fay::ValueType::Bool:
			str = std::to_string(stack->top().boolVal());
			break;
		case fay::ValueType::Byte:
			str = std::to_string(stack->top().byteVal());
			break;
		case fay::ValueType::Int:
			str = std::to_string(stack->top().intVal());
			break;
		case fay::ValueType::Long:
			str = std::to_string(stack->top().longVal());
			break;
		case fay::ValueType::Float:
			str = std::to_string(stack->top().floatVal());
			break;
		case fay::ValueType::Double:
			str = std::to_string(stack->top().doubleVal());
			break;
		case fay::ValueType::String:
			str = *stack->top().strVal();
			break;
		case fay::ValueType::Object:
			str = "object<" + std::to_string((int64_t)stack->top().ptrValue<void>()) + ">";
			break;
		case fay::ValueType::Function:
			str = "fun<" + std::to_string((int64_t)stack->top().ptrValue<void>()) + ">";
			break;
		default:
			break;
	}

	stack->pop();
	stack->push(FayValue(str));
}

void fay::internal::InternalFun::Print_Bool(PTR(std::stack<FayValue>) stack)
{
	std::cout << (stack->top().boolVal() ? "true" : "false") << std::endl;
	stack->pop();
}

void fay::internal::InternalFun::Print_Byte(PTR(std::stack<FayValue>) stack)
{
	std::cout << stack->top().byteVal() << std::endl;
	stack->pop();
}
