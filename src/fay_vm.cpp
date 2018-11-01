#include <fay_vm.h>

using namespace fay;


void fay::FayVM::_run(PTR(FayInstFun) fun)
{
	std::vector<PTR(FayValue)> localVars(fun->varsSize());
	std::vector<FayInst*>* insts = fun->getPreparedInsts();

	for each(FayInst* inst in *insts)
	{
		switch (inst->type())
		{
			//InstCodeStart
			case InstType::Nop:
			{
				//DoNothing
				break;
			}
			case InstType::PushBool:
			{
				this->stack.push(MKPTR(FayValue)(((inst::PushBool*)inst)->val));
				break;
			}
			case InstType::PushByte:
			{
				this->stack.push(MKPTR(FayValue)(((inst::PushByte*)inst)->val));
				break;
			}
			case InstType::PushInt:
			{
				this->stack.push(MKPTR(FayValue)(((inst::PushInt*)inst)->val));
				break;
			}
			case InstType::PushLong:
			{
				this->stack.push(MKPTR(FayValue)(((inst::PushLong*)inst)->val));
				break;
			}
			case InstType::PushFloat:
			{
				this->stack.push(MKPTR(FayValue)(((inst::PushFloat*)inst)->val));
				break;
			}
			case InstType::PushDouble:
			{
				this->stack.push(MKPTR(FayValue)(((inst::PushDouble*)inst)->val));
				break;
			}
			case InstType::PushString:
			{
				this->stack.push(MKPTR(FayValue)(((inst::PushString*)inst)->val));
				break;
			}
			case InstType::Pop:
			{
				this->stack.pop();
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
				localVars[((inst::SetLocal*)inst)->varIndex]=this->stack.top();
				this->stack.pop();
				break;
			}
			case InstType::LoadLocal:
			{
				this->stack.push(localVars[((inst::LoadLocal*)inst)->varIndex]);
				break;
			}
			case InstType::VoidToVoid:
			{
				//DoNothing
				break;
			}
			case InstType::VoidToBool:
			{
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)(false));
				break;
			}
			case InstType::VoidToByte:
			{
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((byte)0));
				break;
			}
			case InstType::VoidToInt:
			{
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((int32_t)0));
				break;
			}
			case InstType::VoidToLong:
			{
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((int64_t)0));
				break;
			}
			case InstType::VoidToFloat:
			{
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((float)0));
				break;
			}
			case InstType::VoidToDouble:
			{
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((double)0));
				break;
			}
			case InstType::VoidToString:
			{
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)(""));
				break;
			}
			case InstType::IntToBool:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((bool)v->intVal()));
				break;
			}
			case InstType::IntToByte:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((byte)v->intVal()));
				break;
			}
			case InstType::IntToInt:
			{
				//DoNothing
				break;
			}
			case InstType::IntToLong:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((int64_t)v->intVal()));
				break;
			}
			case InstType::IntToFloat:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((float)v->intVal()));
				break;
			}
			case InstType::IntToDouble:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((double)v->intVal()));
				break;
			}
			case InstType::IntToString:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)(std::to_string(v->intVal())));
				break;
			}
			case InstType::LongToBool:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((bool)v->longVal()));
				break;
			}
			case InstType::LongToByte:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((byte)v->longVal()));
				break;
			}
			case InstType::LongToInt:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((int32_t)v->longVal()));
				break;
			}
			case InstType::LongToLong:
			{
				//DoNothing
				break;
			}
			case InstType::LongToFloat:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((float)v->longVal()));
				break;
			}
			case InstType::LongToDouble:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((double)v->longVal()));
				break;
			}
			case InstType::FloatToBool:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((bool)v->floatVal()));
				break;
			}
			case InstType::FloatToByte:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((byte)v->floatVal()));
				break;
			}
			case InstType::FloatToInt:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((int32_t)v->floatVal()));
				break;
			}
			case InstType::FloatToLong:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((int64_t)v->floatVal()));
				break;
			}
			case InstType::FloatToFloat:
			{
				//DoNothing
				break;
			}
			case InstType::FloatToDouble:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((double)v->floatVal()));
				break;
			}
			case InstType::DoubleToBool:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((bool)v->doubleVal()));
				break;
			}
			case InstType::DoubleToByte:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((byte)v->doubleVal()));
				break;
			}
			case InstType::DoubleToInt:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((int32_t)v->doubleVal()));
				break;
			}
			case InstType::DoubleToLong:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((int64_t)v->doubleVal()));
				break;
			}
			case InstType::DoubleToFloat:
			{
				PTR(FayValue) v=this->stack.top();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)((float)v->doubleVal()));
				break;
			}
			case InstType::DoubleToDouble:
			{
				//DoNothing
				break;
			}
			case InstType::AddInt:
			{
				int32_t v=this->stack.top()->intVal();
				this->stack.pop();
				v+=this->stack.top()->intVal();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)(v));
				break;
			}
			case InstType::AddLong:
			{
				int64_t v=this->stack.top()->longVal();
				this->stack.pop();
				v+=this->stack.top()->longVal();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)(v));
				break;
			}
			case InstType::AddFloat:
			{
				float v=this->stack.top()->floatVal();
				this->stack.pop();
				v+=this->stack.top()->floatVal();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)(v));
				break;
			}
			case InstType::AddDouble:
			{
				double v=this->stack.top()->doubleVal();
				this->stack.pop();
				v+=this->stack.top()->doubleVal();
				this->stack.pop();
				this->stack.push(MKPTR(FayValue)(v));
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
