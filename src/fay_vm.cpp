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
			case InstType::Nop:
			{
				//Do Nothing
				break;
			}
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
			case InstType::AddInt:
			{
				int32_t v1=stack.top().intVal();
				stack.pop();
				int32_t v2=stack.top().intVal();
				stack.pop();
				stack.push(FayValue(v1+v2));
				break;
			}
			case InstType::CallStatic:
			{
				PTR(FayFun) fun=this->_domain->findFun(((inst::CallStatic*)inst)->typeIndex,((inst::CallStatic*)inst)->funIndex,true);
				this->run(fun);
				break;
			}
			case InstType::SetLocal:
			{
				localVars[((inst::SetLocal*)inst)->varIndex]=stack.top();
				stack.pop();
				break;
			}
			case InstType::LoadLocal:
			{
				stack.push(localVars[((inst::LoadLocal*)inst)->varIndex]);
				break;
			}
			case InstType::VoidToVoid:
			{
				//Do Nothing
				break;
			}
			case InstType::VoidToBool:
			{
				stack.pop();
				stack.push(FayValue(false));
				break;
			}
			case InstType::VoidToByte:
			{
				stack.pop();
				stack.push(FayValue((byte)0));
				break;
			}
			case InstType::VoidToInt:
			{
				stack.pop();
				stack.push(FayValue((int32_t)0));
				break;
			}
			case InstType::VoidToLong:
			{
				stack.pop();
				stack.push(FayValue((int64_t)0));
				break;
			}
			case InstType::VoidToFloat:
			{
				stack.pop();
				stack.push(FayValue((float)0));
				break;
			}
			case InstType::VoidToDouble:
			{
				stack.pop();
				stack.push(FayValue((double)0));
				break;
			}
			case InstType::VoidToString:
			{
				stack.pop();
				stack.push(FayValue(""));
				break;
			}
			//InstCodeEnd
			default:
				LOG_ERROR("Unrealized inst : "+TypeDict::ToName(inst->type()));
				break;
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
