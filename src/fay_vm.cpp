#include <fay_vm.h>

using namespace fay;


void fay::FayVM::_run(PTR(FayInstFun) fun)
{
	std::vector<FayValue> localVars(fun->varsSize());
	std::vector<FayInst*>* insts = fun->getPreparedInsts();

	for each(FayInst* inst in *insts)
	{
		switch (inst->type())
		{
			//InstCodeStart
			case InstType::PushBool:
			{
				stack.push(FayValue(((inst::PushBool*)inst)->val));
				break;
			}
			case InstType::PushByte:
			{
				stack.push(FayValue(((inst::PushByte*)inst)->val));
				break;
			}
			case InstType::PushInt:
			{
				stack.push(FayValue(((inst::PushInt*)inst)->val));
				break;
			}
			case InstType::PushLong:
			{
				stack.push(FayValue(((inst::PushLong*)inst)->val));
				break;
			}
			case InstType::PushFloat:
			{
				stack.push(FayValue(((inst::PushFloat*)inst)->val));
				break;
			}
			case InstType::PushDouble:
			{
				stack.push(FayValue(((inst::PushDouble*)inst)->val));
				break;
			}
			case InstType::PushString:
			{
				stack.push(FayValue(((inst::PushString*)inst)->val));
				break;
			}
			case InstType::Pop:
			{
				stack.pop();
				break;
			}
			case InstType::PopTo:
			{
				break;
			}
			case InstType::AddByte:
			{
				break;
			}
			case InstType::AddInt:
			{
				int32_t v1=stack.top().intVal();
				stack.pop();
				int32_t v2=stack.top().intVal();
				stack.pop();
				stack.push(FayValue(v1+v2));
				break;
			}
			case InstType::AddLong:
			{
				break;
			}
			case InstType::AddFloat:
			{
				break;
			}
			case InstType::AddDouble:
			{
				break;
			}
			case InstType::AddBool:
			{
				break;
			}
			case InstType::AddString:
			{
				break;
			}
			case InstType::SubByte:
			{
				break;
			}
			case InstType::SubInt:
			{
				break;
			}
			case InstType::SubLong:
			{
				break;
			}
			case InstType::SubFloat:
			{
				break;
			}
			case InstType::SubDouble:
			{
				break;
			}
			case InstType::SubBool:
			{
				break;
			}
			case InstType::SubString:
			{
				break;
			}
			case InstType::CallStatic:
			{
				PTR(FayFun) fun=this->_domain->findFun(((inst::CallStatic*)inst)->typeIndex,((inst::CallStatic*)inst)->funIndex,true);
				this->run(fun);
				break;
			}
			case InstType::CallVirtual:
			{
				break;
			}
			case InstType::SetLocal:
			{
				localVars[((inst::SetLocal*)inst)->varIndex]=stack.top();
				stack.pop();
				break;
			}
			case InstType::SetField:
			{
				break;
			}
			case InstType::SetStatic:
			{
				break;
			}
			case InstType::LoadLocal:
			{
				stack.push(localVars[((inst::LoadLocal*)inst)->varIndex]);
				break;
			}
			case InstType::LoadField:
			{
				break;
			}
			case InstType::LoadStatic:
			{
				break;
			}
			case InstType::VoidToVoid:
			{
				break;
			}
			case InstType::VoidToBool:
			{
				break;
			}
			case InstType::VoidToByte:
			{
				break;
			}
			case InstType::VoidToInt:
			{
				break;
			}
			case InstType::VoidToLong:
			{
				break;
			}
			case InstType::VoidToFloat:
			{
				break;
			}
			case InstType::VoidToDouble:
			{
				break;
			}
			case InstType::VoidToString:
			{
				break;
			}
			case InstType::VoidToObject:
			{
				break;
			}
			case InstType::VoidToFunction:
			{
				break;
			}
			case InstType::BoolToVoid:
			{
				break;
			}
			case InstType::BoolToBool:
			{
				break;
			}
			case InstType::BoolToByte:
			{
				break;
			}
			case InstType::BoolToInt:
			{
				break;
			}
			case InstType::BoolToLong:
			{
				break;
			}
			case InstType::BoolToFloat:
			{
				break;
			}
			case InstType::BoolToDouble:
			{
				break;
			}
			case InstType::BoolToString:
			{
				break;
			}
			case InstType::BoolToObject:
			{
				break;
			}
			case InstType::BoolToFunction:
			{
				break;
			}
			case InstType::ByteToVoid:
			{
				break;
			}
			case InstType::ByteToBool:
			{
				break;
			}
			case InstType::ByteToByte:
			{
				break;
			}
			case InstType::ByteToInt:
			{
				break;
			}
			case InstType::ByteToLong:
			{
				break;
			}
			case InstType::ByteToFloat:
			{
				break;
			}
			case InstType::ByteToDouble:
			{
				break;
			}
			case InstType::ByteToString:
			{
				break;
			}
			case InstType::ByteToObject:
			{
				break;
			}
			case InstType::ByteToFunction:
			{
				break;
			}
			case InstType::IntToVoid:
			{
				break;
			}
			case InstType::IntToBool:
			{
				break;
			}
			case InstType::IntToByte:
			{
				break;
			}
			case InstType::IntToInt:
			{
				break;
			}
			case InstType::IntToLong:
			{
				break;
			}
			case InstType::IntToFloat:
			{
				break;
			}
			case InstType::IntToDouble:
			{
				break;
			}
			case InstType::IntToString:
			{
				break;
			}
			case InstType::IntToObject:
			{
				break;
			}
			case InstType::IntToFunction:
			{
				break;
			}
			case InstType::LongToVoid:
			{
				break;
			}
			case InstType::LongToBool:
			{
				break;
			}
			case InstType::LongToByte:
			{
				break;
			}
			case InstType::LongToInt:
			{
				break;
			}
			case InstType::LongToLong:
			{
				break;
			}
			case InstType::LongToFloat:
			{
				break;
			}
			case InstType::LongToDouble:
			{
				break;
			}
			case InstType::LongToString:
			{
				break;
			}
			case InstType::LongToObject:
			{
				break;
			}
			case InstType::LongToFunction:
			{
				break;
			}
			case InstType::FloatToVoid:
			{
				break;
			}
			case InstType::FloatToBool:
			{
				break;
			}
			case InstType::FloatToByte:
			{
				break;
			}
			case InstType::FloatToInt:
			{
				break;
			}
			case InstType::FloatToLong:
			{
				break;
			}
			case InstType::FloatToFloat:
			{
				break;
			}
			case InstType::FloatToDouble:
			{
				break;
			}
			case InstType::FloatToString:
			{
				break;
			}
			case InstType::FloatToObject:
			{
				break;
			}
			case InstType::FloatToFunction:
			{
				break;
			}
			case InstType::DoubleToVoid:
			{
				break;
			}
			case InstType::DoubleToBool:
			{
				break;
			}
			case InstType::DoubleToByte:
			{
				break;
			}
			case InstType::DoubleToInt:
			{
				break;
			}
			case InstType::DoubleToLong:
			{
				break;
			}
			case InstType::DoubleToFloat:
			{
				break;
			}
			case InstType::DoubleToDouble:
			{
				break;
			}
			case InstType::DoubleToString:
			{
				break;
			}
			case InstType::DoubleToObject:
			{
				break;
			}
			case InstType::DoubleToFunction:
			{
				break;
			}
			case InstType::StringToVoid:
			{
				break;
			}
			case InstType::StringToBool:
			{
				break;
			}
			case InstType::StringToByte:
			{
				break;
			}
			case InstType::StringToInt:
			{
				break;
			}
			case InstType::StringToLong:
			{
				break;
			}
			case InstType::StringToFloat:
			{
				break;
			}
			case InstType::StringToDouble:
			{
				break;
			}
			case InstType::StringToString:
			{
				break;
			}
			case InstType::StringToObject:
			{
				break;
			}
			case InstType::StringToFunction:
			{
				break;
			}
			case InstType::ObjectToVoid:
			{
				break;
			}
			case InstType::ObjectToBool:
			{
				break;
			}
			case InstType::ObjectToByte:
			{
				break;
			}
			case InstType::ObjectToInt:
			{
				break;
			}
			case InstType::ObjectToLong:
			{
				break;
			}
			case InstType::ObjectToFloat:
			{
				break;
			}
			case InstType::ObjectToDouble:
			{
				break;
			}
			case InstType::ObjectToString:
			{
				break;
			}
			case InstType::ObjectToObject:
			{
				break;
			}
			case InstType::ObjectToFunction:
			{
				break;
			}
			case InstType::FunctionToVoid:
			{
				break;
			}
			case InstType::FunctionToBool:
			{
				break;
			}
			case InstType::FunctionToByte:
			{
				break;
			}
			case InstType::FunctionToInt:
			{
				break;
			}
			case InstType::FunctionToLong:
			{
				break;
			}
			case InstType::FunctionToFloat:
			{
				break;
			}
			case InstType::FunctionToDouble:
			{
				break;
			}
			case InstType::FunctionToString:
			{
				break;
			}
			case InstType::FunctionToObject:
			{
				break;
			}
			case InstType::FunctionToFunction:
			{
				break;
			}
			//InstCodeEnd
		}
	}
}

void fay::FayVM::run(PTR(FayFun) fun)
{
	switch (fun->type())
	{
		case FunType::Code:
		{
			PTR(FayInstFun) f = TOPTR(FayInstFun, fun);
			this->_run(f);
			break;
		}
		case FunType::Internal:
		{
			PTR(FayInternalFun) f = TOPTR(FayInternalFun, fun);
			f->Invoke(this->stack);
			break;
		}
		default:
			LOG_ERROR("Unknow function type : " << (int)fun->type());
			break;
	}
}
