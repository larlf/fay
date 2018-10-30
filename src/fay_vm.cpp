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
			case InstType::IntToBool:
			{
				break;
			}
			case InstType::IntToString:
			{
				break;
			}
			case InstType::AddByte:
			{
				break;
			}
			case InstType::AddInt:
			{
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
