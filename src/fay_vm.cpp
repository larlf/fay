#include <fay_vm.h>
#include <fay_internal_fun.h>
#include <fay_lang.h>

using namespace fay;
using namespace fay::internal;

void fay::FayVM::_run(PTR(std::stack<FayValue>) stack, PTR(FayValue) &error, PTR(FayInstFun) fun)
{
	//LOG_DEBUG("Fun : " << fun->name() << ", " << fun->varsSize());

	//记录下初始的堆栈的长度
	size_t oldStackSize = stack->size();

	//初始化本地变量
	std::vector<FayValue> localVars(fun->varsSize());
	size_t paramCount = fun->isStatic ? 0 : 1;
	paramCount += fun->params.size();
	for(size_t i = 0; i < paramCount; ++i)
	{
		localVars[paramCount - i - 1] = stack->top();
		stack->pop();
	}

	//LOG_DEBUG("Stack Size : " << stack->size());

	//取得方法的代码
	std::vector<FayInst*>* insts = fun->getPreparedInsts();
	FayInst* inst;
	size_t ip = 0;

label1:

	try
	{
		for(; ip < insts->size(); ++ip)
		{
			inst = (*insts)[ip];

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
					ip = ((inst::Jump*)inst)->target - 1;
					break;
				}
				case InstType::JumpFalse:
				{
					FayValue v = stack->top();
					stack->pop();
					if(!v.boolVal()) ip = ((inst::JumpFalse*)inst)->target - 1;
					break;
				}
				case InstType::JumpTrue:
				{
					FayValue v = stack->top();
					stack->pop();
					if(v.boolVal()) ip = ((inst::JumpTrue*)inst)->target - 1;
					break;
				}
				case InstType::Return:
				{
					ip = insts->size();
					break;
				}
				case InstType::LeaveTry:
				{
					//DoNothing
					break;
				}
				case InstType::LeaveCatch:
				{
					error.reset();
					break;
				}
				case InstType::LeaveFinally:
				{
					//DoNothing
					break;
				}
				case InstType::PushBool:
				{
					stack->push(FayValue(((inst::PushBool*)inst)->val));
					break;
				}
				case InstType::PushByte:
				{
					stack->push(FayValue(((inst::PushByte*)inst)->val));
					break;
				}
				case InstType::PushInt:
				{
					stack->push(FayValue(((inst::PushInt*)inst)->val));
					break;
				}
				case InstType::PushLong:
				{
					stack->push(FayValue(((inst::PushLong*)inst)->val));
					break;
				}
				case InstType::PushFloat:
				{
					stack->push(FayValue(((inst::PushFloat*)inst)->val));
					break;
				}
				case InstType::PushDouble:
				{
					stack->push(FayValue(((inst::PushDouble*)inst)->val));
					break;
				}
				case InstType::PushString:
				{
					stack->push(FayValue(((inst::PushString*)inst)->val));
					break;
				}
				case InstType::Dup:
				{
					stack->push(stack->top());
					break;
				}
				case InstType::Pop:
				{
					stack->pop();
					break;
				}
				case InstType::CallStatic:
				{
					PTR(FayClass) clazz = FayDomain::UseClass(((inst::CallStatic*)inst)->libIndex, ((inst::CallStatic*)inst)->classIndex);
					PTR(FayFun) fun = clazz->findFun(((inst::CallStatic*)inst)->funIndex, true);
					FayVM::_run(stack, error, fun);
					break;
				}
				case InstType::CallVirtual:
				{
					FayVM::chkObj(stack);
					PTR(FayClass) clazz = stack->top().objVal<FayObject>()->clazz();
					PTR(FayFun) fun = clazz->findFun(((inst::CallVirtual*)inst)->funIndex, false);
					FayVM::_run(stack, error, fun);
					break;
				}
				case InstType::SetLocal:
				{
					localVars[((inst::SetLocal*)inst)->varIndex] = stack->top();
					stack->pop();
					break;
				}
				case InstType::SetField:
				{
					FayValue obj = stack->top();
					stack->pop();
					FayValue val = stack->top();
					stack->pop();
					obj.objVal<FayObject>()->vars()[((inst::SetField*)inst)->fieldIndex] = val;
					break;
				}
				case InstType::SetStatic:
				{
					FayDomain::UseClass(((inst::SetStatic*)inst)->libIndex, ((inst::SetStatic*)inst)->classIndex)->staticVar(((inst::SetStatic*)inst)->varIndex) = stack->top();
					stack->pop();
					break;
				}
				case InstType::LoadLocal:
				{
					stack->push(localVars[((inst::LoadLocal*)inst)->varIndex]);
					break;
				}
				case InstType::LoadField:
				{
					FayValue obj = stack->top();
					stack->pop();
					stack->push(obj.objVal<FayObject>()->vars()[((inst::LoadField*)inst)->varIndex]);
					break;
				}
				case InstType::LoadStatic:
				{
					FayValue v = FayDomain::UseClass(((inst::LoadStatic*)inst)->libIndex, ((inst::LoadStatic*)inst)->classIndex)->staticVar(((inst::LoadStatic*)inst)->varIndex);
					stack->push(v);
					break;
				}
				case InstType::LoadError:
				{
					stack->push(*error);
					break;
				}
				case InstType::CopyLocal:
				{
					localVars[((inst::CopyLocal*)inst)->fieldIndex] = stack->top();
					break;
				}
				case InstType::VoidToVoid:
				{
					//DoNothing
					break;
				}
				case InstType::VoidToBool:
				{
					stack->pop();
					stack->push(FayValue(false));
					break;
				}
				case InstType::VoidToByte:
				{
					stack->pop();
					stack->push(FayValue((byte)0));
					break;
				}
				case InstType::VoidToInt:
				{
					stack->pop();
					stack->push(FayValue((int32_t)0));
					break;
				}
				case InstType::VoidToLong:
				{
					stack->pop();
					stack->push(FayValue((int64_t)0));
					break;
				}
				case InstType::VoidToFloat:
				{
					stack->pop();
					stack->push(FayValue((float)0));
					break;
				}
				case InstType::VoidToDouble:
				{
					stack->pop();
					stack->push(FayValue((double)0));
					break;
				}
				case InstType::VoidToString:
				{
					stack->pop();
					stack->push(FayValue(""));
					break;
				}
				case InstType::ByteToString:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(v.boolVal() ? FayValue("true") : FayValue("false"));
					break;
				}
				case InstType::IntToBool:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((bool)v.intVal()));
					break;
				}
				case InstType::IntToByte:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((byte)v.intVal()));
					break;
				}
				case InstType::IntToInt:
				{
					//DoNothing
					break;
				}
				case InstType::IntToLong:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((int64_t)v.intVal()));
					break;
				}
				case InstType::IntToFloat:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((float)v.intVal()));
					break;
				}
				case InstType::IntToDouble:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((double)v.intVal()));
					break;
				}
				case InstType::IntToString:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue(std::to_string(v.intVal())));
					break;
				}
				case InstType::LongToBool:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((bool)v.longVal()));
					break;
				}
				case InstType::LongToByte:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((byte)v.longVal()));
					break;
				}
				case InstType::LongToInt:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((int32_t)v.longVal()));
					break;
				}
				case InstType::LongToLong:
				{
					//DoNothing
					break;
				}
				case InstType::LongToFloat:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((float)v.longVal()));
					break;
				}
				case InstType::LongToDouble:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((double)v.longVal()));
					break;
				}
				case InstType::LongToString:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue(std::to_string(v.longVal())));
					break;
				}
				case InstType::FloatToBool:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((bool)v.floatVal()));
					break;
				}
				case InstType::FloatToByte:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((byte)v.floatVal()));
					break;
				}
				case InstType::FloatToInt:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((int32_t)v.floatVal()));
					break;
				}
				case InstType::FloatToLong:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((int64_t)v.floatVal()));
					break;
				}
				case InstType::FloatToFloat:
				{
					//DoNothing
					break;
				}
				case InstType::FloatToDouble:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((double)v.floatVal()));
					break;
				}
				case InstType::FloatToString:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue(std::to_string(v.floatVal())));
					break;
				}
				case InstType::DoubleToBool:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((bool)v.doubleVal()));
					break;
				}
				case InstType::DoubleToByte:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((byte)v.doubleVal()));
					break;
				}
				case InstType::DoubleToInt:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((int32_t)v.doubleVal()));
					break;
				}
				case InstType::DoubleToLong:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((int64_t)v.doubleVal()));
					break;
				}
				case InstType::DoubleToFloat:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue((float)v.doubleVal()));
					break;
				}
				case InstType::DoubleToDouble:
				{
					//DoNothing
					break;
				}
				case InstType::DoubleToString:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->push(FayValue(std::to_string(v.doubleVal())));
					break;
				}
				case InstType::StringToBool:
				{
					InternalFun::StringToBool(stack);
					break;
				}
				case InstType::StringToByte:
				{
					InternalFun::StringToByte(stack);
					break;
				}
				case InstType::StringToInt:
				{
					InternalFun::StringToInt(stack);
					break;
				}
				case InstType::StringToLong:
				{
					InternalFun::StringToLong(stack);
					break;
				}
				case InstType::StringToFloat:
				{
					InternalFun::StringToFloat(stack);
					break;
				}
				case InstType::StringToDouble:
				{
					InternalFun::StringToDouble(stack);
					break;
				}
				case InstType::StringToString:
				{
					//DoNothing
					break;
				}
				case InstType::MinusByte:
				{
					stack->top().val()->byteVal = -stack->top().byteVal();
					break;
				}
				case InstType::MinusInt:
				{
					stack->top().val()->intVal = -stack->top().intVal();
					break;
				}
				case InstType::MinusLong:
				{
					stack->top().val()->longVal = -stack->top().longVal();
					break;
				}
				case InstType::MinusFloat:
				{
					stack->top().val()->floatVal = -stack->top().floatVal();
					break;
				}
				case InstType::MinusDouble:
				{
					stack->top().val()->doubleVal = -stack->top().doubleVal();
					break;
				}
				case InstType::PlusByte:
				{
					stack->top().val()->byteVal += (char)((inst::PlusByte*)inst)->val;
					break;
				}
				case InstType::PlusInt:
				{
					stack->top().val()->intVal += (int32_t)((inst::PlusInt*)inst)->val;
					break;
				}
				case InstType::PlusLong:
				{
					stack->top().val()->longVal += (int64_t)((inst::PlusLong*)inst)->val;
					break;
				}
				case InstType::PlusFloat:
				{
					stack->top().val()->floatVal += (float)((inst::PlusFloat*)inst)->val;
					break;
				}
				case InstType::PlusDouble:
				{
					stack->top().val()->doubleVal += (double)((inst::PlusDouble*)inst)->val;
					break;
				}
				case InstType::AddByte:
				{
					byte v = stack->top().byteVal();
					stack->pop();
					stack->top().val()->byteVal += v;
					break;
				}
				case InstType::AddInt:
				{
					int32_t v = stack->top().intVal();
					stack->pop();
					stack->top().val()->intVal += v;
					break;
				}
				case InstType::AddLong:
				{
					int64_t v = stack->top().longVal();
					stack->pop();
					stack->top().val()->longVal += v;
					break;
				}
				case InstType::AddFloat:
				{
					float v = stack->top().floatVal();
					stack->pop();
					stack->top().val()->floatVal += v;
					break;
				}
				case InstType::AddDouble:
				{
					double v = stack->top().doubleVal();
					stack->pop();
					stack->top().val()->doubleVal += v;
					break;
				}
				case InstType::AddString:
				{
					std::string v = *stack->top().strVal();
					stack->pop();
					(*stack->top().val()->strVal) += v;
					break;
				}
				case InstType::SubByte:
				{
					byte v = stack->top().byteVal();
					stack->pop();
					stack->top().val()->byteVal -= v;
					break;
				}
				case InstType::SubInt:
				{
					int32_t v = stack->top().intVal();
					stack->pop();
					stack->top().val()->intVal -= v;
					break;
				}
				case InstType::SubLong:
				{
					int64_t v = stack->top().longVal();
					stack->pop();
					stack->top().val()->longVal -= v;
					break;
				}
				case InstType::SubFloat:
				{
					float v = stack->top().floatVal();
					stack->pop();
					stack->top().val()->floatVal -= v;
					break;
				}
				case InstType::SubDouble:
				{
					double v = stack->top().doubleVal();
					stack->pop();
					stack->top().val()->doubleVal -= v;
					break;
				}
				case InstType::MulInt:
				{
					int32_t v = stack->top().intVal();
					stack->pop();
					stack->top().val()->intVal *= v;
					break;
				}
				case InstType::MulLong:
				{
					int64_t v = stack->top().longVal();
					stack->pop();
					stack->top().val()->longVal *= v;
					break;
				}
				case InstType::MulFloat:
				{
					float v = stack->top().floatVal();
					stack->pop();
					stack->top().val()->floatVal *= v;
					break;
				}
				case InstType::MulDouble:
				{
					double v = stack->top().doubleVal();
					stack->pop();
					stack->top().val()->doubleVal *= v;
					break;
				}
				case InstType::DivInt:
				{
					int32_t v = stack->top().intVal();
					stack->pop();
					stack->top().val()->intVal /= v;
					break;
				}
				case InstType::DivLong:
				{
					int64_t v = stack->top().longVal();
					stack->pop();
					stack->top().val()->longVal /= v;
					break;
				}
				case InstType::DivFloat:
				{
					float v = stack->top().floatVal();
					stack->pop();
					stack->top().val()->floatVal /= v;
					break;
				}
				case InstType::DivDouble:
				{
					double v = stack->top().doubleVal();
					stack->pop();
					stack->top().val()->doubleVal /= v;
					break;
				}
				case InstType::ModByte:
				{
					byte v = stack->top().byteVal();
					stack->pop();
					stack->top().val()->byteVal %= v;
					break;
				}
				case InstType::ModInt:
				{
					int32_t v = stack->top().intVal();
					stack->pop();
					stack->top().val()->intVal %= v;
					break;
				}
				case InstType::ModLong:
				{
					int64_t v = stack->top().longVal();
					stack->pop();
					stack->top().val()->longVal %= v;
					break;
				}
				case InstType::BoolNot:
				{
					stack->top().val()->boolVal = !stack->top().val()->boolVal;
					break;
				}
				case InstType::BoolAnd:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->top().val()->boolVal = stack->top().boolVal() && v.boolVal();
					break;
				}
				case InstType::BoolOr:
				{
					FayValue v = stack->top();
					stack->pop();
					stack->top().val()->boolVal = stack->top().boolVal() || v.boolVal();
					break;
				}
				case InstType::EqualBool:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.boolVal() == v2.boolVal()));
					break;
				}
				case InstType::EqualByte:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.byteVal() == v2.byteVal()));
					break;
				}
				case InstType::EqualInt:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.intVal() == v2.intVal()));
					break;
				}
				case InstType::EqualLong:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.longVal() == v2.longVal()));
					break;
				}
				case InstType::EqualFloat:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.floatVal() == v2.floatVal()));
					break;
				}
				case InstType::EqualDouble:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.doubleVal() == v2.doubleVal()));
					break;
				}
				case InstType::EqualString:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue((*v1.strVal()) == (*v2.strVal())));
					break;
				}
				case InstType::NotEqualBool:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.boolVal() != v2.boolVal()));
					break;
				}
				case InstType::NotEqualByte:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.byteVal() != v2.byteVal()));
					break;
				}
				case InstType::NotEqualInt:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.intVal() != v2.intVal()));
					break;
				}
				case InstType::NotEqualLong:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.longVal() != v2.longVal()));
					break;
				}
				case InstType::NotEqualFloat:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.floatVal() != v2.floatVal()));
					break;
				}
				case InstType::NotEqualDouble:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.doubleVal() != v2.doubleVal()));
					break;
				}
				case InstType::GreaterBool:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.boolVal() > v2.boolVal()));
					break;
				}
				case InstType::GreaterByte:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.byteVal() > v2.byteVal()));
					break;
				}
				case InstType::GreaterInt:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.intVal() > v2.intVal()));
					break;
				}
				case InstType::GreaterLong:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.longVal() > v2.longVal()));
					break;
				}
				case InstType::GreaterFloat:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.floatVal() > v2.floatVal()));
					break;
				}
				case InstType::GreaterDouble:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.doubleVal() > v2.doubleVal()));
					break;
				}
				case InstType::GreaterEqualBool:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.boolVal() >= v2.boolVal()));
					break;
				}
				case InstType::GreaterEqualByte:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.byteVal() >= v2.byteVal()));
					break;
				}
				case InstType::GreaterEqualInt:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.intVal() >= v2.intVal()));
					break;
				}
				case InstType::GreaterEqualLong:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.longVal() >= v2.longVal()));
					break;
				}
				case InstType::GreaterEqualFloat:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.floatVal() >= v2.floatVal()));
					break;
				}
				case InstType::GreaterEqualDouble:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.doubleVal() >= v2.doubleVal()));
					break;
				}
				case InstType::LessBool:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.boolVal() < v2.boolVal()));
					break;
				}
				case InstType::LessByte:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.byteVal() < v2.byteVal()));
					break;
				}
				case InstType::LessInt:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.intVal() < v2.intVal()));
					break;
				}
				case InstType::LessLong:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.longVal() < v2.longVal()));
					break;
				}
				case InstType::LessFloat:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.floatVal() < v2.floatVal()));
					break;
				}
				case InstType::LessDouble:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.doubleVal() < v2.doubleVal()));
					break;
				}
				case InstType::LessEqualBool:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.boolVal() <= v2.boolVal()));
					break;
				}
				case InstType::LessEqualByte:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.byteVal() <= v2.byteVal()));
					break;
				}
				case InstType::LessEqualInt:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.intVal() <= v2.intVal()));
					break;
				}
				case InstType::LessEqualLong:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.longVal() <= v2.longVal()));
					break;
				}
				case InstType::LessEqualFloat:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.floatVal() <= v2.floatVal()));
					break;
				}
				case InstType::LessEqualDouble:
				{
					FayValue v2 = stack->top();
					stack->pop();
					FayValue v1 = stack->top();
					stack->pop();
					stack->push(FayValue(v1.doubleVal() <= v2.doubleVal()));
					break;
				}
				case InstType::BitNotByte:
				{
					stack->top().val()->byteVal = ~stack->top().val()->byteVal;
					break;
				}
				case InstType::BitNotInt:
				{
					stack->top().val()->intVal = ~stack->top().val()->intVal;
					break;
				}
				case InstType::BitNotLong:
				{
					stack->top().val()->longVal = ~stack->top().val()->longVal;
					break;
				}
				case InstType::BitAndByte:
				{
					FayValue v1 = stack->top();
					stack->pop();
					stack->top().val()->byteVal &= v1.val()->byteVal;
					break;
				}
				case InstType::BitAndInt:
				{
					FayValue v1 = stack->top();
					stack->pop();
					stack->top().val()->intVal &= v1.val()->intVal;
					break;
				}
				case InstType::BitAndLong:
				{
					FayValue v1 = stack->top();
					stack->pop();
					stack->top().val()->longVal &= v1.val()->longVal;
					break;
				}
				case InstType::BitOrByte:
				{
					FayValue v1 = stack->top();
					stack->pop();
					stack->top().val()->byteVal |= v1.val()->byteVal;
					break;
				}
				case InstType::BitOrInt:
				{
					FayValue v1 = stack->top();
					stack->pop();
					stack->top().val()->intVal |= v1.val()->intVal;
					break;
				}
				case InstType::BitOrLong:
				{
					FayValue v1 = stack->top();
					stack->pop();
					stack->top().val()->longVal |= v1.val()->longVal;
					break;
				}
				case InstType::BitXorByte:
				{
					FayValue v1 = stack->top();
					stack->pop();
					stack->top().val()->byteVal ^= v1.val()->byteVal;
					break;
				}
				case InstType::BitXorInt:
				{
					FayValue v1 = stack->top();
					stack->pop();
					stack->top().val()->intVal ^= v1.val()->intVal;
					break;
				}
				case InstType::BitXorLong:
				{
					FayValue v1 = stack->top();
					stack->pop();
					stack->top().val()->longVal ^= v1.val()->longVal;
					break;
				}
				case InstType::LShiftByte:
				{
					byte v = stack->top().byteVal();
					stack->pop();
					stack->top().val()->byteVal <<= v;
					break;
				}
				case InstType::LShiftInt:
				{
					int32_t v = stack->top().intVal();
					stack->pop();
					stack->top().val()->intVal <<= v;
					break;
				}
				case InstType::LShiftLong:
				{
					int64_t v = stack->top().longVal();
					stack->pop();
					stack->top().val()->longVal <<= v;
					break;
				}
				case InstType::RShiftByte:
				{
					byte v = stack->top().byteVal();
					stack->pop();
					stack->top().val()->byteVal >>= v;
					break;
				}
				case InstType::RShiftInt:
				{
					int32_t v = stack->top().intVal();
					stack->pop();
					stack->top().val()->intVal >>= v;
					break;
				}
				case InstType::RShiftLong:
				{
					int64_t v = stack->top().longVal();
					stack->pop();
					stack->top().val()->longVal >>= v;
					break;
				}
				case InstType::NewObject:
				{
					PTR(fay::FayObject) obj = MKPTR(fay::FayObject)(FayDomain::UseClass(((inst::NewObject*)inst)->libIndex, ((inst::NewObject*)inst)->classIndex));
					obj->init();
					stack->push(FayValue(obj));
					break;
				}
				case InstType::NullObject:
				{
					PTR(fay::FayObject) obj;
					stack->push(FayValue(obj));
					break;
				}
				//InstCodeEnd
				default:
					LOG_ERROR("Unrealized inst : " + TypeDict::ToName(inst->type()));
					break;
			}
		}
	}
	catch(std::exception e)
	{
		//检查有没有Catch
		TryHandler* handler = fun->findHandler(ip);
		if(handler != nullptr)
		{
			//生成错误对象
			PTR(FayObject) errObj = MKPTR(FayObject)(fun->clazz.lock()->lib.lock()->findClass("fay.Error", true));
			errObj->setVar("code", FayValue(1));
			errObj->setVar("message", FayValue(e.what()));
			error = MKPTR(FayValue)(errObj);

			//进入异常处理流程
			ip = handler->target;
			goto label1;
		}
		else
		{
			LOG_ERROR(e.what());
			throw e;
		}
	}
}

void fay::FayVM::_run(PTR(std::stack<FayValue>) stack, PTR(FayValue) &error, PTR(FayFun) fun)
{
	switch(fun->type)
	{
		case FunType::Code:
		{
			PTR(FayInstFun) f = TOPTR(FayInstFun, fun);
			FayVM::_run(stack, error, f);
			break;
		}
		case FunType::Internal:
		{
			PTR(FayInternalFun) f = TOPTR(FayInternalFun, fun);
			f->Invoke(stack);
			break;
		}
		default:
			LOG_ERROR("Unknow function type : " << (int)fun->type);
			break;
	}
}

FayValue fay::FayVM::Run(PTR(FayFun) fun, FayValue &self, std::vector<FayValue> &params)
{
	FayValue values;
	PTR(std::stack<FayValue>) stack = MKPTR(std::stack<FayValue>)();
	PTR(FayValue) error;

	if(!fun)
	{
		LOG_ERROR("Fun is null");
		return values;
	}

	if(self.type() != ValueType::Void)
		stack->push(self);

	for(const auto &it : params)
		stack->push(it);

	FayVM::_run(stack, error, fun);

	//确定返回值的数量
	if(!fun->returnValue.expired() && fun->returnValue.lock()->valueType() != ValueType::Void)
	{
		if(stack->size() != 1)
			LOG_ERROR("Stack size error : " << stack->size());
		else
		{
			values = stack->top();
			stack->pop();
		}
	}

	//清空堆栈
	while(!stack->empty())
		stack->pop();

	return values;
}
