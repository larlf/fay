#include <fay_vm.h>

using namespace fay;

void fay::FayVM::_run(std::vector<FayInst*>* insts)
{
	for each(FayInst* inst in *insts)
	{
		switch (inst->type())
		{
			//InstCodeStart
			case InstType::PushBool:
			{
				stack.push(new FayValue(((inst::PushBool*)inst)->val));
				break;
			}
			case InstType::PushByte:
			{
				stack.push(new FayValue(((inst::PushByte*)inst)->val));
				break;
			}
			case InstType::PushInt:
			{
				stack.push(new FayValue(((inst::PushInt*)inst)->val));
				break;
			}
			case InstType::PushLong:
			{
				stack.push(new FayValue(((inst::PushLong*)inst)->val));
				break;
			}
			case InstType::PushFloat:
			{
				stack.push(new FayValue(((inst::PushFloat*)inst)->val));
				break;
			}
			case InstType::PushDouble:
			{
				stack.push(new FayValue(((inst::PushDouble*)inst)->val));
				break;
			}
			case InstType::PushString:
			{
				stack.push(new FayValue(((inst::PushString*)inst)->val));
				break;
			}
			case InstType::Pop:
			{
				delete stack.pop();
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
				PTR(FayFun) fun=this->_domain->findFun(((inst::CallStatic*)inst)->typeIndex,((inst::CallStatic*)inst)->funIndex, true);
				this->run(fun);
				break;
			}
			case InstType::CallFun:
			{
				break;
			}
			case InstType::CallVirtual:
			{
				break;
			}
			case InstType::CallInterface:
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
			this->_run(f->getPreparedInsts());
			break;
		}
		case FunType::Internal:
		{
			PTR(FayInternalFun) f = TOPTR(FayInternalFun, fun);
			f->Invoke(&this->stack);
			break;
		}
		default:
			LOG_ERROR("Unknow function type : " << (int)fun->type());
			break;
	}
}
