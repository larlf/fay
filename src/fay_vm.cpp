﻿#include <fay_vm.h>

using namespace fay;


void fay::FayVM::_run(PTR(FayInstFun) fun)
{
	std::vector<FayValue> localVars(fun->varsSize());
	std::vector<FayInst*>* insts = fun->getPreparedInsts();
	FayInst* inst;

	for(int i = 0; i < insts->size(); ++i)
	{
		inst = (*insts)[i];

		switch(inst->type())
		{
			//InstCodeStart
			case InstType::undefinedNop:
			{
				//DoNothing
				break;
			}
			case InstType::Jump:
			{
				i=((inst::Jump*)inst)->target-1;
				break;
			}
			case InstType::JumpFalse:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				if(!v.boolVal()) i=((inst::JumpFalse*)inst)->target-1;
				break;
			}
			case InstType::Return:
			{
				i=insts->size();
				break;
			}
			case InstType::PushBool:
			{
				this->stack.push(FayValue(((inst::PushBool*)inst)->val));
				break;
			}
			case InstType::PushByte:
			{
				this->stack.push(FayValue(((inst::PushByte*)inst)->val));
				break;
			}
			case InstType::PushInt:
			{
				this->stack.push(FayValue(((inst::PushInt*)inst)->val));
				break;
			}
			case InstType::PushLong:
			{
				this->stack.push(FayValue(((inst::PushLong*)inst)->val));
				break;
			}
			case InstType::PushFloat:
			{
				this->stack.push(FayValue(((inst::PushFloat*)inst)->val));
				break;
			}
			case InstType::PushDouble:
			{
				this->stack.push(FayValue(((inst::PushDouble*)inst)->val));
				break;
			}
			case InstType::PushString:
			{
				this->stack.push(FayValue(((inst::PushString*)inst)->val));
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
				this->_run(fun);
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
			case InstType::CopyLocal:
			{
				localVars[((inst::CopyLocal*)inst)->fieldIndex]=this->stack.top();
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
				this->stack.push(FayValue(false));
				break;
			}
			case InstType::VoidToByte:
			{
				this->stack.pop();
				this->stack.push(FayValue((byte)0));
				break;
			}
			case InstType::VoidToInt:
			{
				this->stack.pop();
				this->stack.push(FayValue((int32_t)0));
				break;
			}
			case InstType::VoidToLong:
			{
				this->stack.pop();
				this->stack.push(FayValue((int64_t)0));
				break;
			}
			case InstType::VoidToFloat:
			{
				this->stack.pop();
				this->stack.push(FayValue((float)0));
				break;
			}
			case InstType::VoidToDouble:
			{
				this->stack.pop();
				this->stack.push(FayValue((double)0));
				break;
			}
			case InstType::VoidToString:
			{
				this->stack.pop();
				this->stack.push(FayValue(""));
				break;
			}
			case InstType::IntToBool:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((bool)v.intVal()));
				break;
			}
			case InstType::IntToByte:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((byte)v.intVal()));
				break;
			}
			case InstType::IntToInt:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((int32_t)v.intVal()));
				break;
			}
			case InstType::IntToLong:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((int64_t)v.intVal()));
				break;
			}
			case InstType::IntToFloat:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((float)v.intVal()));
				break;
			}
			case InstType::IntToDouble:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((double)v.intVal()));
				break;
			}
			case InstType::IntToString:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(std::to_string(v.intVal())));
				break;
			}
			case InstType::LongToBool:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((bool)v.longVal()));
				break;
			}
			case InstType::LongToByte:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((byte)v.longVal()));
				break;
			}
			case InstType::LongToInt:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((int32_t)v.longVal()));
				break;
			}
			case InstType::LongToLong:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((int64_t)v.longVal()));
				break;
			}
			case InstType::LongToFloat:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((float)v.longVal()));
				break;
			}
			case InstType::LongToDouble:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((double)v.longVal()));
				break;
			}
			case InstType::FloatToBool:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((bool)v.floatVal()));
				break;
			}
			case InstType::FloatToByte:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((byte)v.floatVal()));
				break;
			}
			case InstType::FloatToInt:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((int32_t)v.floatVal()));
				break;
			}
			case InstType::FloatToLong:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((int64_t)v.floatVal()));
				break;
			}
			case InstType::FloatToFloat:
			{
				//DoNothing
				break;
			}
			case InstType::FloatToDouble:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((double)v.floatVal()));
				break;
			}
			case InstType::DoubleToBool:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((bool)v.doubleVal()));
				break;
			}
			case InstType::DoubleToByte:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((byte)v.doubleVal()));
				break;
			}
			case InstType::DoubleToInt:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((int32_t)v.doubleVal()));
				break;
			}
			case InstType::DoubleToLong:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((int64_t)v.doubleVal()));
				break;
			}
			case InstType::DoubleToFloat:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((float)v.doubleVal()));
				break;
			}
			case InstType::DoubleToDouble:
			{
				//DoNothing
				break;
			}
			case InstType::AddInt:
			{
				int32_t v=this->stack.top().intVal();
				this->stack.pop();
				this->stack.top().val()->intVal+=v;
				break;
			}
			case InstType::AddLong:
			{
				int64_t v=this->stack.top().longVal();
				this->stack.pop();
				this->stack.top().val()->longVal+=v;
				break;
			}
			case InstType::AddFloat:
			{
				float v=this->stack.top().floatVal();
				this->stack.pop();
				this->stack.top().val()->floatVal+=v;
				break;
			}
			case InstType::AddDouble:
			{
				double v=this->stack.top().doubleVal();
				this->stack.pop();
				this->stack.top().val()->doubleVal+=v;
				break;
			}
			case InstType::AddString:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				(*this->stack.top().strVal())+=*v.strVal();
				break;
			}
			case InstType::EqualBool:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.boolVal()==v2.boolVal()));
				break;
			}
			case InstType::EqualByte:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.byteVal()==v2.byteVal()));
				break;
			}
			case InstType::EqualInt:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.intVal()==v2.intVal()));
				break;
			}
			case InstType::EqualLong:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.longVal()==v2.longVal()));
				break;
			}
			case InstType::EqualFloat:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.floatVal()==v2.floatVal()));
				break;
			}
			case InstType::EqualDouble:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.doubleVal()==v2.doubleVal()));
				break;
			}
			case InstType::NotEqualBool:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.boolVal()!=v2.boolVal()));
				break;
			}
			case InstType::NotEqualByte:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.byteVal()!=v2.byteVal()));
				break;
			}
			case InstType::NotEqualInt:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.intVal()!=v2.intVal()));
				break;
			}
			case InstType::NotEqualLong:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.longVal()!=v2.longVal()));
				break;
			}
			case InstType::NotEqualFloat:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.floatVal()!=v2.floatVal()));
				break;
			}
			case InstType::NotEqualDouble:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.doubleVal()!=v2.doubleVal()));
				break;
			}
			case InstType::GreaterBool:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.boolVal()>v2.boolVal()));
				break;
			}
			case InstType::GreaterByte:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.byteVal()>v2.byteVal()));
				break;
			}
			case InstType::GreaterInt:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.intVal()>v2.intVal()));
				break;
			}
			case InstType::GreaterLong:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.longVal()>v2.longVal()));
				break;
			}
			case InstType::GreaterFloat:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.floatVal()>v2.floatVal()));
				break;
			}
			case InstType::GreaterDouble:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.doubleVal()>v2.doubleVal()));
				break;
			}
			case InstType::GreaterEqualBool:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.boolVal()>=v2.boolVal()));
				break;
			}
			case InstType::GreaterEqualByte:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.byteVal()>=v2.byteVal()));
				break;
			}
			case InstType::GreaterEqualInt:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.intVal()>=v2.intVal()));
				break;
			}
			case InstType::GreaterEqualLong:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.longVal()>=v2.longVal()));
				break;
			}
			case InstType::GreaterEqualFloat:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.floatVal()>=v2.floatVal()));
				break;
			}
			case InstType::GreaterEqualDouble:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.doubleVal()>=v2.doubleVal()));
				break;
			}
			case InstType::LessBool:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.boolVal()<v2.boolVal()));
				break;
			}
			case InstType::LessByte:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.byteVal()<v2.byteVal()));
				break;
			}
			case InstType::LessInt:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.intVal()<v2.intVal()));
				break;
			}
			case InstType::LessLong:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.longVal()<v2.longVal()));
				break;
			}
			case InstType::LessFloat:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.floatVal()<v2.floatVal()));
				break;
			}
			case InstType::LessDouble:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.doubleVal()<v2.doubleVal()));
				break;
			}
			case InstType::LessEqualBool:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.boolVal()<v2.boolVal()));
				break;
			}
			case InstType::LessEqualByte:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.byteVal()<v2.byteVal()));
				break;
			}
			case InstType::LessEqualInt:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.intVal()<v2.intVal()));
				break;
			}
			case InstType::LessEqualLong:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.longVal()<v2.longVal()));
				break;
			}
			case InstType::LessEqualFloat:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.floatVal()<v2.floatVal()));
				break;
			}
			case InstType::LessEqualDouble:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.doubleVal()<v2.doubleVal()));
				break;
			}
			//InstCodeEnd
			default:
				LOG_ERROR("Unrealized inst : " + TypeDict::ToName(inst->type()));
				break;
		}
	}
}

void fay::FayVM::_run(PTR(FayFun) fun)
{
	switch(fun->type())
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

FayValue fay::FayVM::run(PTR(FayFun) fun)
{
	FayValue values;

	if(!fun)
	{
		LOG_ERROR("Fun is null");
		return values;
	}

	this->_run(fun);

	//确定返回值的数量
	if (fun->returnValue())
	{
		if (this->stack.size() !=1)
		{
			LOG_ERROR("Stack size error : " << this->stack.size());
		}

		values = this->stack.top();
		this->stack.pop();
	}

	//清空堆栈
	while(!this->stack.empty())
		this->stack.pop();

	return values;
}
