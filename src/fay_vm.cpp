#include <fay_vm.h>

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
			case InstType::Nop:
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
			case InstType::Dup:
			{
				this->stack.push(this->stack.top());
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
				//DoNothing
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
				//DoNothing
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
			case InstType::MinusByte:
			{
				this->stack.top().val()->byteVal = -this->stack.top().byteVal();
				break;
			}
			case InstType::MinusInt:
			{
				this->stack.top().val()->intVal = -this->stack.top().intVal();
				break;
			}
			case InstType::MinusLong:
			{
				this->stack.top().val()->longVal = -this->stack.top().longVal();
				break;
			}
			case InstType::MinusFloat:
			{
				this->stack.top().val()->floatVal = -this->stack.top().floatVal();
				break;
			}
			case InstType::MinusDouble:
			{
				this->stack.top().val()->doubleVal = -this->stack.top().doubleVal();
				break;
			}
			case InstType::PlusByte:
			{
				this->stack.top().val()->byteVal += (char)((inst::PlusByte*)inst)->val;
				break;
			}
			case InstType::PlusInt:
			{
				this->stack.top().val()->intVal += (int32_t)((inst::PlusInt*)inst)->val;
				break;
			}
			case InstType::PlusLong:
			{
				this->stack.top().val()->longVal += (int64_t)((inst::PlusLong*)inst)->val;
				break;
			}
			case InstType::PlusFloat:
			{
				this->stack.top().val()->floatVal += (float)((inst::PlusFloat*)inst)->val;
				break;
			}
			case InstType::PlusDouble:
			{
				this->stack.top().val()->doubleVal += (double)((inst::PlusDouble*)inst)->val;
				break;
			}
			case InstType::AddByte:
			{
				byte v = this->stack.top().byteVal();
				this->stack.pop();
				this->stack.top().val()->byteVal += v;
				break;
			}
			case InstType::AddInt:
			{
				int32_t v = this->stack.top().intVal();
				this->stack.pop();
				this->stack.top().val()->intVal += v;
				break;
			}
			case InstType::AddLong:
			{
				int64_t v = this->stack.top().longVal();
				this->stack.pop();
				this->stack.top().val()->longVal += v;
				break;
			}
			case InstType::AddFloat:
			{
				float v = this->stack.top().floatVal();
				this->stack.pop();
				this->stack.top().val()->floatVal += v;
				break;
			}
			case InstType::AddDouble:
			{
				double v = this->stack.top().doubleVal();
				this->stack.pop();
				this->stack.top().val()->doubleVal += v;
				break;
			}
			case InstType::AddString:
			{
				std::string v = *this->stack.top().strVal();
				this->stack.pop();
				(*this->stack.top().val()->strVal) += v;
				break;
			}
			case InstType::SubByte:
			{
				byte v=this->stack.top().byteVal();
				this->stack.pop();
				this->stack.top().val()->byteVal -= v;
				break;
			}
			case InstType::SubInt:
			{
				int32_t v=this->stack.top().intVal();
				this->stack.pop();
				this->stack.top().val()->intVal -= v;
				break;
			}
			case InstType::SubLong:
			{
				int64_t v=this->stack.top().longVal();
				this->stack.pop();
				this->stack.top().val()->longVal -= v;
				break;
			}
			case InstType::SubFloat:
			{
				float v=this->stack.top().floatVal();
				this->stack.pop();
				this->stack.top().val()->floatVal -= v;
				break;
			}
			case InstType::SubDouble:
			{
				double v=this->stack.top().doubleVal();
				this->stack.pop();
				this->stack.top().val()->doubleVal -= v;
				break;
			}
			case InstType::MulInt:
			{
				int32_t v=this->stack.top().intVal();
				this->stack.pop();
				this->stack.top().val()->intVal *= v;
				break;
			}
			case InstType::MulLong:
			{
				int64_t v=this->stack.top().longVal();
				this->stack.pop();
				this->stack.top().val()->longVal *= v;
				break;
			}
			case InstType::MulFloat:
			{
				float v=this->stack.top().floatVal();
				this->stack.pop();
				this->stack.top().val()->floatVal *= v;
				break;
			}
			case InstType::MulDouble:
			{
				double v=this->stack.top().doubleVal();
				this->stack.pop();
				this->stack.top().val()->doubleVal *= v;
				break;
			}
			case InstType::DivInt:
			{
				int32_t v=this->stack.top().intVal();
				this->stack.pop();
				this->stack.top().val()->intVal /= v;
				break;
			}
			case InstType::DivLong:
			{
				int64_t v=this->stack.top().longVal();
				this->stack.pop();
				this->stack.top().val()->longVal /= v;
				break;
			}
			case InstType::DivFloat:
			{
				float v=this->stack.top().floatVal();
				this->stack.pop();
				this->stack.top().val()->floatVal /= v;
				break;
			}
			case InstType::DivDouble:
			{
				double v=this->stack.top().doubleVal();
				this->stack.pop();
				this->stack.top().val()->doubleVal /= v;
				break;
			}
			case InstType::ModByte:
			{
				byte v=this->stack.top().byteVal();
				this->stack.pop();
				this->stack.top().val()->byteVal %= v;
				break;
			}
			case InstType::ModInt:
			{
				int32_t v=this->stack.top().intVal();
				this->stack.pop();
				this->stack.top().val()->intVal %= v;
				break;
			}
			case InstType::ModLong:
			{
				int64_t v=this->stack.top().longVal();
				this->stack.pop();
				this->stack.top().val()->longVal %= v;
				break;
			}
			case InstType::BoolNot:
			{
				this->stack.top().val()->boolVal = !this->stack.top().val()->boolVal;
				break;
			}
			case InstType::BoolAnd:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.top().val()->boolVal = this->stack.top().boolVal() && v.boolVal();
				break;
			}
			case InstType::BoolOr:
			{
				FayValue v=this->stack.top();
				this->stack.pop();
				this->stack.top().val()->boolVal = this->stack.top().boolVal() || v.boolVal();
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
			case InstType::EqualString:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue((*v1.strVal())==(*v2.strVal())));
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
				this->stack.push(FayValue(v1.boolVal() >= v2.boolVal()));
				break;
			}
			case InstType::GreaterEqualByte:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.byteVal() >= v2.byteVal()));
				break;
			}
			case InstType::GreaterEqualInt:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.intVal() >= v2.intVal()));
				break;
			}
			case InstType::GreaterEqualLong:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.longVal() >= v2.longVal()));
				break;
			}
			case InstType::GreaterEqualFloat:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.floatVal() >= v2.floatVal()));
				break;
			}
			case InstType::GreaterEqualDouble:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.doubleVal() >= v2.doubleVal()));
				break;
			}
			case InstType::LessBool:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.boolVal() < v2.boolVal()));
				break;
			}
			case InstType::LessByte:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.byteVal() < v2.byteVal()));
				break;
			}
			case InstType::LessInt:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.intVal() < v2.intVal()));
				break;
			}
			case InstType::LessLong:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.longVal() < v2.longVal()));
				break;
			}
			case InstType::LessFloat:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.floatVal() < v2.floatVal()));
				break;
			}
			case InstType::LessDouble:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.doubleVal() < v2.doubleVal()));
				break;
			}
			case InstType::LessEqualBool:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.boolVal() <= v2.boolVal()));
				break;
			}
			case InstType::LessEqualByte:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.byteVal() <= v2.byteVal()));
				break;
			}
			case InstType::LessEqualInt:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.intVal() <= v2.intVal()));
				break;
			}
			case InstType::LessEqualLong:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.longVal() <= v2.longVal()));
				break;
			}
			case InstType::LessEqualFloat:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.floatVal() <= v2.floatVal()));
				break;
			}
			case InstType::LessEqualDouble:
			{
				FayValue v2=this->stack.top();
				this->stack.pop();
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.push(FayValue(v1.doubleVal() <= v2.doubleVal()));
				break;
			}
			case InstType::BitNotByte:
			{
				this->stack.top().val()->byteVal = ~this->stack.top().val()->byteVal;
				break;
			}
			case InstType::BitNotInt:
			{
				this->stack.top().val()->intVal = ~this->stack.top().val()->intVal;
				break;
			}
			case InstType::BitNotLong:
			{
				this->stack.top().val()->longVal = ~this->stack.top().val()->longVal;
				break;
			}
			case InstType::BitAndByte:
			{
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.top().val()->byteVal &= v1.val()->byteVal;
				break;
			}
			case InstType::BitAndInt:
			{
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.top().val()->intVal &= v1.val()->intVal;
				break;
			}
			case InstType::BitAndLong:
			{
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.top().val()->longVal &= v1.val()->longVal;
				break;
			}
			case InstType::BitOrByte:
			{
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.top().val()->byteVal |= v1.val()->byteVal;
				break;
			}
			case InstType::BitOrInt:
			{
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.top().val()->intVal |= v1.val()->intVal;
				break;
			}
			case InstType::BitOrLong:
			{
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.top().val()->longVal |= v1.val()->longVal;
				break;
			}
			case InstType::BitXorByte:
			{
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.top().val()->byteVal ^= v1.val()->byteVal;
				break;
			}
			case InstType::BitXorInt:
			{
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.top().val()->intVal ^= v1.val()->intVal;
				break;
			}
			case InstType::BitXorLong:
			{
				FayValue v1=this->stack.top();
				this->stack.pop();
				this->stack.top().val()->longVal ^= v1.val()->longVal;
				break;
			}
			case InstType::LShiftByte:
			{
				byte v=this->stack.top().byteVal();
				this->stack.pop();
				this->stack.top().val()->byteVal <<= v;
				break;
			}
			case InstType::LShiftInt:
			{
				int32_t v=this->stack.top().intVal();
				this->stack.pop();
				this->stack.top().val()->intVal <<= v;
				break;
			}
			case InstType::LShiftLong:
			{
				int64_t v=this->stack.top().longVal();
				this->stack.pop();
				this->stack.top().val()->longVal <<= v;
				break;
			}
			case InstType::RShiftByte:
			{
				byte v=this->stack.top().byteVal();
				this->stack.pop();
				this->stack.top().val()->byteVal >>= v;
				break;
			}
			case InstType::RShiftInt:
			{
				int32_t v=this->stack.top().intVal();
				this->stack.pop();
				this->stack.top().val()->intVal >>= v;
				break;
			}
			case InstType::RShiftLong:
			{
				int64_t v=this->stack.top().longVal();
				this->stack.pop();
				this->stack.top().val()->longVal >>= v;
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
		else
		{
			values = this->stack.top();
			this->stack.pop();
		}
	}

	//清空堆栈
	while(!this->stack.empty())
		this->stack.pop();

	return values;
}
