#include <fay_inst.h>

//InstStart

void fay::inst::Nop::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("Nop")
		->endl();
}

void fay::inst::Goto::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("Goto")
		->add(" ")->add(this->target)
		->endl();
}

void fay::inst::PushBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("PushBool")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::PushByte::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("PushByte")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::PushInt::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("PushInt")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::PushLong::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("PushLong")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::PushFloat::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("PushFloat")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::PushDouble::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("PushDouble")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::PushString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("PushString")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::Pop::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("Pop")
		->endl();
}

void fay::inst::PopTo::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("PopTo")
		->add(" ")->add(this->count)
		->endl();
}

void fay::inst::CallStatic::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("CallStatic")
		->add(" ")->add(this->outsideFunIndex)
		->add(" ")->add(this->paramCount)
		->endl();
}

void fay::inst::CallVirtual::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("CallVirtual")
		->add(" ")->add(this->outsideFunIndex)
		->add(" ")->add(this->paramCount)
		->endl();
}

void fay::inst::SetLocal::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("SetLocal")
		->add(" ")->add(this->varIndex)
		->endl();
}

void fay::inst::SetField::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("SetField")
		->add(" ")->add(this->fieldIndex)
		->endl();
}

void fay::inst::SetStatic::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("SetStatic")
		->add(" ")->add(this->typeIndex)
		->add(" ")->add(this->fieldIndex)
		->endl();
}

void fay::inst::LoadLocal::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("LoadLocal")
		->add(" ")->add(this->varIndex)
		->endl();
}

void fay::inst::LoadField::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("LoadField")
		->add(" ")->add(this->fieldIndex)
		->endl();
}

void fay::inst::LoadStatic::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("LoadStatic")
		->add(" ")->add(this->typeIndex)
		->add(" ")->add(this->fieldIndex)
		->endl();
}

void fay::inst::VoidToVoid::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToVoid")
		->endl();
}

void fay::inst::VoidToBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToBool")
		->endl();
}

void fay::inst::VoidToByte::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToByte")
		->endl();
}

void fay::inst::VoidToInt::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToInt")
		->endl();
}

void fay::inst::VoidToLong::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToLong")
		->endl();
}

void fay::inst::VoidToFloat::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToFloat")
		->endl();
}

void fay::inst::VoidToDouble::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToDouble")
		->endl();
}

void fay::inst::VoidToString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToString")
		->endl();
}

void fay::inst::VoidToObject::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToObject")
		->endl();
}

void fay::inst::VoidToFunction::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToFunction")
		->endl();
}

void fay::inst::BoolToVoid::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToVoid")
		->endl();
}

void fay::inst::BoolToBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToBool")
		->endl();
}

void fay::inst::BoolToByte::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToByte")
		->endl();
}

void fay::inst::BoolToInt::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToInt")
		->endl();
}

void fay::inst::BoolToLong::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToLong")
		->endl();
}

void fay::inst::BoolToFloat::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToFloat")
		->endl();
}

void fay::inst::BoolToDouble::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToDouble")
		->endl();
}

void fay::inst::BoolToString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToString")
		->endl();
}

void fay::inst::BoolToObject::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToObject")
		->endl();
}

void fay::inst::BoolToFunction::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToFunction")
		->endl();
}

void fay::inst::ByteToVoid::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToVoid")
		->endl();
}

void fay::inst::ByteToBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToBool")
		->endl();
}

void fay::inst::ByteToByte::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToByte")
		->endl();
}

void fay::inst::ByteToInt::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToInt")
		->endl();
}

void fay::inst::ByteToLong::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToLong")
		->endl();
}

void fay::inst::ByteToFloat::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToFloat")
		->endl();
}

void fay::inst::ByteToDouble::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToDouble")
		->endl();
}

void fay::inst::ByteToString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToString")
		->endl();
}

void fay::inst::ByteToObject::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToObject")
		->endl();
}

void fay::inst::ByteToFunction::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToFunction")
		->endl();
}

void fay::inst::IntToVoid::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToVoid")
		->endl();
}

void fay::inst::IntToBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToBool")
		->endl();
}

void fay::inst::IntToByte::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToByte")
		->endl();
}

void fay::inst::IntToInt::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToInt")
		->endl();
}

void fay::inst::IntToLong::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToLong")
		->endl();
}

void fay::inst::IntToFloat::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToFloat")
		->endl();
}

void fay::inst::IntToDouble::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToDouble")
		->endl();
}

void fay::inst::IntToString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToString")
		->endl();
}

void fay::inst::IntToObject::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToObject")
		->endl();
}

void fay::inst::IntToFunction::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToFunction")
		->endl();
}

void fay::inst::LongToVoid::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToVoid")
		->endl();
}

void fay::inst::LongToBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToBool")
		->endl();
}

void fay::inst::LongToByte::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToByte")
		->endl();
}

void fay::inst::LongToInt::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToInt")
		->endl();
}

void fay::inst::LongToLong::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToLong")
		->endl();
}

void fay::inst::LongToFloat::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToFloat")
		->endl();
}

void fay::inst::LongToDouble::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToDouble")
		->endl();
}

void fay::inst::LongToString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToString")
		->endl();
}

void fay::inst::LongToObject::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToObject")
		->endl();
}

void fay::inst::LongToFunction::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToFunction")
		->endl();
}

void fay::inst::FloatToVoid::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToVoid")
		->endl();
}

void fay::inst::FloatToBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToBool")
		->endl();
}

void fay::inst::FloatToByte::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToByte")
		->endl();
}

void fay::inst::FloatToInt::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToInt")
		->endl();
}

void fay::inst::FloatToLong::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToLong")
		->endl();
}

void fay::inst::FloatToFloat::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToFloat")
		->endl();
}

void fay::inst::FloatToDouble::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToDouble")
		->endl();
}

void fay::inst::FloatToString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToString")
		->endl();
}

void fay::inst::FloatToObject::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToObject")
		->endl();
}

void fay::inst::FloatToFunction::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToFunction")
		->endl();
}

void fay::inst::DoubleToVoid::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToVoid")
		->endl();
}

void fay::inst::DoubleToBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToBool")
		->endl();
}

void fay::inst::DoubleToByte::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToByte")
		->endl();
}

void fay::inst::DoubleToInt::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToInt")
		->endl();
}

void fay::inst::DoubleToLong::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToLong")
		->endl();
}

void fay::inst::DoubleToFloat::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToFloat")
		->endl();
}

void fay::inst::DoubleToDouble::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToDouble")
		->endl();
}

void fay::inst::DoubleToString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToString")
		->endl();
}

void fay::inst::DoubleToObject::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToObject")
		->endl();
}

void fay::inst::DoubleToFunction::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToFunction")
		->endl();
}

void fay::inst::StringToVoid::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToVoid")
		->endl();
}

void fay::inst::StringToBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToBool")
		->endl();
}

void fay::inst::StringToByte::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToByte")
		->endl();
}

void fay::inst::StringToInt::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToInt")
		->endl();
}

void fay::inst::StringToLong::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToLong")
		->endl();
}

void fay::inst::StringToFloat::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToFloat")
		->endl();
}

void fay::inst::StringToDouble::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToDouble")
		->endl();
}

void fay::inst::StringToString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToString")
		->endl();
}

void fay::inst::StringToObject::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToObject")
		->endl();
}

void fay::inst::StringToFunction::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToFunction")
		->endl();
}

void fay::inst::ObjectToVoid::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToVoid")
		->endl();
}

void fay::inst::ObjectToBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToBool")
		->endl();
}

void fay::inst::ObjectToByte::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToByte")
		->endl();
}

void fay::inst::ObjectToInt::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToInt")
		->endl();
}

void fay::inst::ObjectToLong::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToLong")
		->endl();
}

void fay::inst::ObjectToFloat::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToFloat")
		->endl();
}

void fay::inst::ObjectToDouble::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToDouble")
		->endl();
}

void fay::inst::ObjectToString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToString")
		->endl();
}

void fay::inst::ObjectToObject::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToObject")
		->endl();
}

void fay::inst::ObjectToFunction::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToFunction")
		->endl();
}

void fay::inst::FunctionToVoid::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToVoid")
		->endl();
}

void fay::inst::FunctionToBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToBool")
		->endl();
}

void fay::inst::FunctionToByte::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToByte")
		->endl();
}

void fay::inst::FunctionToInt::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToInt")
		->endl();
}

void fay::inst::FunctionToLong::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToLong")
		->endl();
}

void fay::inst::FunctionToFloat::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToFloat")
		->endl();
}

void fay::inst::FunctionToDouble::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToDouble")
		->endl();
}

void fay::inst::FunctionToString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToString")
		->endl();
}

void fay::inst::FunctionToObject::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToObject")
		->endl();
}

void fay::inst::FunctionToFunction::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToFunction")
		->endl();
}

void fay::inst::AddVoid::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddVoid")
		->endl();
}

void fay::inst::AddBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddBool")
		->endl();
}

void fay::inst::AddByte::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddByte")
		->endl();
}

void fay::inst::AddInt::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddInt")
		->endl();
}

void fay::inst::AddLong::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddLong")
		->endl();
}

void fay::inst::AddFloat::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddFloat")
		->endl();
}

void fay::inst::AddDouble::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddDouble")
		->endl();
}

void fay::inst::AddString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddString")
		->endl();
}

void fay::inst::AddObject::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddObject")
		->endl();
}

void fay::inst::AddFunction::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddFunction")
		->endl();
}

void fay::inst::SubVoid::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubVoid")
		->endl();
}

void fay::inst::SubBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubBool")
		->endl();
}

void fay::inst::SubByte::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubByte")
		->endl();
}

void fay::inst::SubInt::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubInt")
		->endl();
}

void fay::inst::SubLong::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubLong")
		->endl();
}

void fay::inst::SubFloat::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubFloat")
		->endl();
}

void fay::inst::SubDouble::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubDouble")
		->endl();
}

void fay::inst::SubString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubString")
		->endl();
}

void fay::inst::SubObject::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubObject")
		->endl();
}

void fay::inst::SubFunction::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubFunction")
		->endl();
}

void fay::inst::MulVoid::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulVoid")
		->endl();
}

void fay::inst::MulBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulBool")
		->endl();
}

void fay::inst::MulByte::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulByte")
		->endl();
}

void fay::inst::MulInt::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulInt")
		->endl();
}

void fay::inst::MulLong::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulLong")
		->endl();
}

void fay::inst::MulFloat::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulFloat")
		->endl();
}

void fay::inst::MulDouble::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulDouble")
		->endl();
}

void fay::inst::MulString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulString")
		->endl();
}

void fay::inst::MulObject::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulObject")
		->endl();
}

void fay::inst::MulFunction::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulFunction")
		->endl();
}

void fay::inst::DivVoid::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivVoid")
		->endl();
}

void fay::inst::DivBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivBool")
		->endl();
}

void fay::inst::DivByte::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivByte")
		->endl();
}

void fay::inst::DivInt::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivInt")
		->endl();
}

void fay::inst::DivLong::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivLong")
		->endl();
}

void fay::inst::DivFloat::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivFloat")
		->endl();
}

void fay::inst::DivDouble::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivDouble")
		->endl();
}

void fay::inst::DivString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivString")
		->endl();
}

void fay::inst::DivObject::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivObject")
		->endl();
}

void fay::inst::DivFunction::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivFunction")
		->endl();
}

//InstEnd
