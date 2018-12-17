#include <fay_inst.h>

//InstStart

void fay::inst::Nop::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("Nop")
		->endl();
}

void fay::inst::Jump::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("Jump")
		->add(" ")->add(this->target)
		->endl();
}

void fay::inst::JumpFalse::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("JumpFalse")
		->add(" ")->add(this->target)
		->endl();
}

void fay::inst::JumpTrue::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("JumpTrue")
		->add(" ")->add(this->target)
		->endl();
}

void fay::inst::Return::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("Return")
		->endl();
}

void fay::inst::PushBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PushBool")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::PushByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PushByte")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::PushInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PushInt")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::PushLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PushLong")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::PushFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PushFloat")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::PushDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PushDouble")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::PushString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PushString")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::Dup::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("Dup")
		->endl();
}

void fay::inst::Pop::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("Pop")
		->endl();
}

void fay::inst::PopTo::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PopTo")
		->add(" ")->add(this->count)
		->endl();
}

void fay::inst::CallStatic::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("CallStatic")
		->add(" ")->add(this->className)
		->add(" ")->add(this->funName)
		->endl();
}

void fay::inst::CallVirtual::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("CallVirtual")
		->add(" ")->add(this->className)
		->add(" ")->add(this->funName)
		->endl();
}

void fay::inst::SetLocal::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("SetLocal")
		->add(" ")->add(this->varIndex)
		->endl();
}

void fay::inst::SetField::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("SetField")
		->add(" ")->add(this->fieldIndex)
		->endl();
}

void fay::inst::SetStatic::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("SetStatic")
		->add(" ")->add(this->className)
		->add(" ")->add(this->varName)
		->endl();
}

void fay::inst::LoadLocal::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LoadLocal")
		->add(" ")->add(this->varIndex)
		->endl();
}

void fay::inst::LoadField::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LoadField")
		->add(" ")->add(this->varIndex)
		->endl();
}

void fay::inst::LoadStatic::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LoadStatic")
		->add(" ")->add(this->className)
		->add(" ")->add(this->varName)
		->endl();
}

void fay::inst::CopyLocal::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("CopyLocal")
		->add(" ")->add(this->fieldIndex)
		->endl();
}

void fay::inst::CopyField::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("CopyField")
		->endl();
}

void fay::inst::CopyStatic::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("CopyStatic")
		->endl();
}

void fay::inst::VoidToVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToVoid")
		->endl();
}

void fay::inst::VoidToBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToBool")
		->endl();
}

void fay::inst::VoidToByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToByte")
		->endl();
}

void fay::inst::VoidToInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToInt")
		->endl();
}

void fay::inst::VoidToLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToLong")
		->endl();
}

void fay::inst::VoidToFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToFloat")
		->endl();
}

void fay::inst::VoidToDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToDouble")
		->endl();
}

void fay::inst::VoidToString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToString")
		->endl();
}

void fay::inst::VoidToObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToObject")
		->endl();
}

void fay::inst::VoidToFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("VoidToFunction")
		->endl();
}

void fay::inst::BoolToVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToVoid")
		->endl();
}

void fay::inst::BoolToBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToBool")
		->endl();
}

void fay::inst::BoolToByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToByte")
		->endl();
}

void fay::inst::BoolToInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToInt")
		->endl();
}

void fay::inst::BoolToLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToLong")
		->endl();
}

void fay::inst::BoolToFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToFloat")
		->endl();
}

void fay::inst::BoolToDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToDouble")
		->endl();
}

void fay::inst::BoolToString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToString")
		->endl();
}

void fay::inst::BoolToObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToObject")
		->endl();
}

void fay::inst::BoolToFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolToFunction")
		->endl();
}

void fay::inst::ByteToVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToVoid")
		->endl();
}

void fay::inst::ByteToBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToBool")
		->endl();
}

void fay::inst::ByteToByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToByte")
		->endl();
}

void fay::inst::ByteToInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToInt")
		->endl();
}

void fay::inst::ByteToLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToLong")
		->endl();
}

void fay::inst::ByteToFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToFloat")
		->endl();
}

void fay::inst::ByteToDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToDouble")
		->endl();
}

void fay::inst::ByteToString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToString")
		->endl();
}

void fay::inst::ByteToObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToObject")
		->endl();
}

void fay::inst::ByteToFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ByteToFunction")
		->endl();
}

void fay::inst::IntToVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToVoid")
		->endl();
}

void fay::inst::IntToBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToBool")
		->endl();
}

void fay::inst::IntToByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToByte")
		->endl();
}

void fay::inst::IntToInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToInt")
		->endl();
}

void fay::inst::IntToLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToLong")
		->endl();
}

void fay::inst::IntToFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToFloat")
		->endl();
}

void fay::inst::IntToDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToDouble")
		->endl();
}

void fay::inst::IntToString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToString")
		->endl();
}

void fay::inst::IntToObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToObject")
		->endl();
}

void fay::inst::IntToFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToFunction")
		->endl();
}

void fay::inst::LongToVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToVoid")
		->endl();
}

void fay::inst::LongToBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToBool")
		->endl();
}

void fay::inst::LongToByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToByte")
		->endl();
}

void fay::inst::LongToInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToInt")
		->endl();
}

void fay::inst::LongToLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToLong")
		->endl();
}

void fay::inst::LongToFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToFloat")
		->endl();
}

void fay::inst::LongToDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToDouble")
		->endl();
}

void fay::inst::LongToString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToString")
		->endl();
}

void fay::inst::LongToObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToObject")
		->endl();
}

void fay::inst::LongToFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LongToFunction")
		->endl();
}

void fay::inst::FloatToVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToVoid")
		->endl();
}

void fay::inst::FloatToBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToBool")
		->endl();
}

void fay::inst::FloatToByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToByte")
		->endl();
}

void fay::inst::FloatToInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToInt")
		->endl();
}

void fay::inst::FloatToLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToLong")
		->endl();
}

void fay::inst::FloatToFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToFloat")
		->endl();
}

void fay::inst::FloatToDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToDouble")
		->endl();
}

void fay::inst::FloatToString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToString")
		->endl();
}

void fay::inst::FloatToObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToObject")
		->endl();
}

void fay::inst::FloatToFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FloatToFunction")
		->endl();
}

void fay::inst::DoubleToVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToVoid")
		->endl();
}

void fay::inst::DoubleToBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToBool")
		->endl();
}

void fay::inst::DoubleToByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToByte")
		->endl();
}

void fay::inst::DoubleToInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToInt")
		->endl();
}

void fay::inst::DoubleToLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToLong")
		->endl();
}

void fay::inst::DoubleToFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToFloat")
		->endl();
}

void fay::inst::DoubleToDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToDouble")
		->endl();
}

void fay::inst::DoubleToString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToString")
		->endl();
}

void fay::inst::DoubleToObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToObject")
		->endl();
}

void fay::inst::DoubleToFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DoubleToFunction")
		->endl();
}

void fay::inst::StringToVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToVoid")
		->endl();
}

void fay::inst::StringToBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToBool")
		->endl();
}

void fay::inst::StringToByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToByte")
		->endl();
}

void fay::inst::StringToInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToInt")
		->endl();
}

void fay::inst::StringToLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToLong")
		->endl();
}

void fay::inst::StringToFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToFloat")
		->endl();
}

void fay::inst::StringToDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToDouble")
		->endl();
}

void fay::inst::StringToString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToString")
		->endl();
}

void fay::inst::StringToObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToObject")
		->endl();
}

void fay::inst::StringToFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("StringToFunction")
		->endl();
}

void fay::inst::ObjectToVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToVoid")
		->endl();
}

void fay::inst::ObjectToBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToBool")
		->endl();
}

void fay::inst::ObjectToByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToByte")
		->endl();
}

void fay::inst::ObjectToInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToInt")
		->endl();
}

void fay::inst::ObjectToLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToLong")
		->endl();
}

void fay::inst::ObjectToFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToFloat")
		->endl();
}

void fay::inst::ObjectToDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToDouble")
		->endl();
}

void fay::inst::ObjectToString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToString")
		->endl();
}

void fay::inst::ObjectToObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToObject")
		->endl();
}

void fay::inst::ObjectToFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ObjectToFunction")
		->endl();
}

void fay::inst::FunctionToVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToVoid")
		->endl();
}

void fay::inst::FunctionToBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToBool")
		->endl();
}

void fay::inst::FunctionToByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToByte")
		->endl();
}

void fay::inst::FunctionToInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToInt")
		->endl();
}

void fay::inst::FunctionToLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToLong")
		->endl();
}

void fay::inst::FunctionToFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToFloat")
		->endl();
}

void fay::inst::FunctionToDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToDouble")
		->endl();
}

void fay::inst::FunctionToString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToString")
		->endl();
}

void fay::inst::FunctionToObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToObject")
		->endl();
}

void fay::inst::FunctionToFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("FunctionToFunction")
		->endl();
}

void fay::inst::MinusVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MinusVoid")
		->endl();
}

void fay::inst::MinusBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MinusBool")
		->endl();
}

void fay::inst::MinusByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MinusByte")
		->endl();
}

void fay::inst::MinusInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MinusInt")
		->endl();
}

void fay::inst::MinusLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MinusLong")
		->endl();
}

void fay::inst::MinusFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MinusFloat")
		->endl();
}

void fay::inst::MinusDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MinusDouble")
		->endl();
}

void fay::inst::MinusString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MinusString")
		->endl();
}

void fay::inst::MinusObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MinusObject")
		->endl();
}

void fay::inst::MinusFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MinusFunction")
		->endl();
}

void fay::inst::PlusVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PlusVoid")
		->endl();
}

void fay::inst::PlusBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PlusBool")
		->endl();
}

void fay::inst::PlusByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PlusByte")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::PlusInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PlusInt")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::PlusLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PlusLong")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::PlusFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PlusFloat")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::PlusDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PlusDouble")
		->add(" ")->add(this->val)
		->endl();
}

void fay::inst::PlusString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PlusString")
		->endl();
}

void fay::inst::PlusObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PlusObject")
		->endl();
}

void fay::inst::PlusFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("PlusFunction")
		->endl();
}

void fay::inst::AddVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddVoid")
		->endl();
}

void fay::inst::AddBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddBool")
		->endl();
}

void fay::inst::AddByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddByte")
		->endl();
}

void fay::inst::AddInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddInt")
		->endl();
}

void fay::inst::AddLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddLong")
		->endl();
}

void fay::inst::AddFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddFloat")
		->endl();
}

void fay::inst::AddDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddDouble")
		->endl();
}

void fay::inst::AddString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddString")
		->endl();
}

void fay::inst::AddObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddObject")
		->endl();
}

void fay::inst::AddFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddFunction")
		->endl();
}

void fay::inst::SubVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubVoid")
		->endl();
}

void fay::inst::SubBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubBool")
		->endl();
}

void fay::inst::SubByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubByte")
		->endl();
}

void fay::inst::SubInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubInt")
		->endl();
}

void fay::inst::SubLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubLong")
		->endl();
}

void fay::inst::SubFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubFloat")
		->endl();
}

void fay::inst::SubDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubDouble")
		->endl();
}

void fay::inst::SubString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubString")
		->endl();
}

void fay::inst::SubObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubObject")
		->endl();
}

void fay::inst::SubFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubFunction")
		->endl();
}

void fay::inst::MulVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulVoid")
		->endl();
}

void fay::inst::MulBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulBool")
		->endl();
}

void fay::inst::MulByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulByte")
		->endl();
}

void fay::inst::MulInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulInt")
		->endl();
}

void fay::inst::MulLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulLong")
		->endl();
}

void fay::inst::MulFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulFloat")
		->endl();
}

void fay::inst::MulDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulDouble")
		->endl();
}

void fay::inst::MulString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulString")
		->endl();
}

void fay::inst::MulObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulObject")
		->endl();
}

void fay::inst::MulFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("MulFunction")
		->endl();
}

void fay::inst::DivVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivVoid")
		->endl();
}

void fay::inst::DivBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivBool")
		->endl();
}

void fay::inst::DivByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivByte")
		->endl();
}

void fay::inst::DivInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivInt")
		->endl();
}

void fay::inst::DivLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivLong")
		->endl();
}

void fay::inst::DivFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivFloat")
		->endl();
}

void fay::inst::DivDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivDouble")
		->endl();
}

void fay::inst::DivString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivString")
		->endl();
}

void fay::inst::DivObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivObject")
		->endl();
}

void fay::inst::DivFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("DivFunction")
		->endl();
}

void fay::inst::ModVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ModVoid")
		->endl();
}

void fay::inst::ModBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ModBool")
		->endl();
}

void fay::inst::ModByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ModByte")
		->endl();
}

void fay::inst::ModInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ModInt")
		->endl();
}

void fay::inst::ModLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ModLong")
		->endl();
}

void fay::inst::ModFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ModFloat")
		->endl();
}

void fay::inst::ModDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ModDouble")
		->endl();
}

void fay::inst::ModString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ModString")
		->endl();
}

void fay::inst::ModObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ModObject")
		->endl();
}

void fay::inst::ModFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("ModFunction")
		->endl();
}

void fay::inst::BoolNot::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolNot")
		->endl();
}

void fay::inst::BoolAnd::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolAnd")
		->endl();
}

void fay::inst::BoolOr::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BoolOr")
		->endl();
}

void fay::inst::EqualVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("EqualVoid")
		->endl();
}

void fay::inst::EqualBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("EqualBool")
		->endl();
}

void fay::inst::EqualByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("EqualByte")
		->endl();
}

void fay::inst::EqualInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("EqualInt")
		->endl();
}

void fay::inst::EqualLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("EqualLong")
		->endl();
}

void fay::inst::EqualFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("EqualFloat")
		->endl();
}

void fay::inst::EqualDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("EqualDouble")
		->endl();
}

void fay::inst::EqualString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("EqualString")
		->endl();
}

void fay::inst::EqualObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("EqualObject")
		->endl();
}

void fay::inst::EqualFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("EqualFunction")
		->endl();
}

void fay::inst::NotEqualVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("NotEqualVoid")
		->endl();
}

void fay::inst::NotEqualBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("NotEqualBool")
		->endl();
}

void fay::inst::NotEqualByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("NotEqualByte")
		->endl();
}

void fay::inst::NotEqualInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("NotEqualInt")
		->endl();
}

void fay::inst::NotEqualLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("NotEqualLong")
		->endl();
}

void fay::inst::NotEqualFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("NotEqualFloat")
		->endl();
}

void fay::inst::NotEqualDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("NotEqualDouble")
		->endl();
}

void fay::inst::NotEqualString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("NotEqualString")
		->endl();
}

void fay::inst::NotEqualObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("NotEqualObject")
		->endl();
}

void fay::inst::NotEqualFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("NotEqualFunction")
		->endl();
}

void fay::inst::GreaterVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterVoid")
		->endl();
}

void fay::inst::GreaterBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterBool")
		->endl();
}

void fay::inst::GreaterByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterByte")
		->endl();
}

void fay::inst::GreaterInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterInt")
		->endl();
}

void fay::inst::GreaterLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterLong")
		->endl();
}

void fay::inst::GreaterFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterFloat")
		->endl();
}

void fay::inst::GreaterDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterDouble")
		->endl();
}

void fay::inst::GreaterString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterString")
		->endl();
}

void fay::inst::GreaterObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterObject")
		->endl();
}

void fay::inst::GreaterFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterFunction")
		->endl();
}

void fay::inst::GreaterEqualVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterEqualVoid")
		->endl();
}

void fay::inst::GreaterEqualBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterEqualBool")
		->endl();
}

void fay::inst::GreaterEqualByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterEqualByte")
		->endl();
}

void fay::inst::GreaterEqualInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterEqualInt")
		->endl();
}

void fay::inst::GreaterEqualLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterEqualLong")
		->endl();
}

void fay::inst::GreaterEqualFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterEqualFloat")
		->endl();
}

void fay::inst::GreaterEqualDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterEqualDouble")
		->endl();
}

void fay::inst::GreaterEqualString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterEqualString")
		->endl();
}

void fay::inst::GreaterEqualObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterEqualObject")
		->endl();
}

void fay::inst::GreaterEqualFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("GreaterEqualFunction")
		->endl();
}

void fay::inst::LessVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessVoid")
		->endl();
}

void fay::inst::LessBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessBool")
		->endl();
}

void fay::inst::LessByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessByte")
		->endl();
}

void fay::inst::LessInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessInt")
		->endl();
}

void fay::inst::LessLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessLong")
		->endl();
}

void fay::inst::LessFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessFloat")
		->endl();
}

void fay::inst::LessDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessDouble")
		->endl();
}

void fay::inst::LessString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessString")
		->endl();
}

void fay::inst::LessObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessObject")
		->endl();
}

void fay::inst::LessFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessFunction")
		->endl();
}

void fay::inst::LessEqualVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessEqualVoid")
		->endl();
}

void fay::inst::LessEqualBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessEqualBool")
		->endl();
}

void fay::inst::LessEqualByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessEqualByte")
		->endl();
}

void fay::inst::LessEqualInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessEqualInt")
		->endl();
}

void fay::inst::LessEqualLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessEqualLong")
		->endl();
}

void fay::inst::LessEqualFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessEqualFloat")
		->endl();
}

void fay::inst::LessEqualDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessEqualDouble")
		->endl();
}

void fay::inst::LessEqualString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessEqualString")
		->endl();
}

void fay::inst::LessEqualObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessEqualObject")
		->endl();
}

void fay::inst::LessEqualFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LessEqualFunction")
		->endl();
}

void fay::inst::BitNotVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitNotVoid")
		->endl();
}

void fay::inst::BitNotBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitNotBool")
		->endl();
}

void fay::inst::BitNotByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitNotByte")
		->endl();
}

void fay::inst::BitNotInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitNotInt")
		->endl();
}

void fay::inst::BitNotLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitNotLong")
		->endl();
}

void fay::inst::BitNotFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitNotFloat")
		->endl();
}

void fay::inst::BitNotDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitNotDouble")
		->endl();
}

void fay::inst::BitNotString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitNotString")
		->endl();
}

void fay::inst::BitNotObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitNotObject")
		->endl();
}

void fay::inst::BitNotFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitNotFunction")
		->endl();
}

void fay::inst::BitAndVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitAndVoid")
		->endl();
}

void fay::inst::BitAndBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitAndBool")
		->endl();
}

void fay::inst::BitAndByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitAndByte")
		->endl();
}

void fay::inst::BitAndInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitAndInt")
		->endl();
}

void fay::inst::BitAndLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitAndLong")
		->endl();
}

void fay::inst::BitAndFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitAndFloat")
		->endl();
}

void fay::inst::BitAndDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitAndDouble")
		->endl();
}

void fay::inst::BitAndString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitAndString")
		->endl();
}

void fay::inst::BitAndObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitAndObject")
		->endl();
}

void fay::inst::BitAndFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitAndFunction")
		->endl();
}

void fay::inst::BitOrVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitOrVoid")
		->endl();
}

void fay::inst::BitOrBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitOrBool")
		->endl();
}

void fay::inst::BitOrByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitOrByte")
		->endl();
}

void fay::inst::BitOrInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitOrInt")
		->endl();
}

void fay::inst::BitOrLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitOrLong")
		->endl();
}

void fay::inst::BitOrFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitOrFloat")
		->endl();
}

void fay::inst::BitOrDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitOrDouble")
		->endl();
}

void fay::inst::BitOrString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitOrString")
		->endl();
}

void fay::inst::BitOrObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitOrObject")
		->endl();
}

void fay::inst::BitOrFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitOrFunction")
		->endl();
}

void fay::inst::BitXorVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitXorVoid")
		->endl();
}

void fay::inst::BitXorBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitXorBool")
		->endl();
}

void fay::inst::BitXorByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitXorByte")
		->endl();
}

void fay::inst::BitXorInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitXorInt")
		->endl();
}

void fay::inst::BitXorLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitXorLong")
		->endl();
}

void fay::inst::BitXorFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitXorFloat")
		->endl();
}

void fay::inst::BitXorDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitXorDouble")
		->endl();
}

void fay::inst::BitXorString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitXorString")
		->endl();
}

void fay::inst::BitXorObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitXorObject")
		->endl();
}

void fay::inst::BitXorFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("BitXorFunction")
		->endl();
}

void fay::inst::LShiftVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LShiftVoid")
		->endl();
}

void fay::inst::LShiftBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LShiftBool")
		->endl();
}

void fay::inst::LShiftByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LShiftByte")
		->endl();
}

void fay::inst::LShiftInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LShiftInt")
		->endl();
}

void fay::inst::LShiftLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LShiftLong")
		->endl();
}

void fay::inst::LShiftFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LShiftFloat")
		->endl();
}

void fay::inst::LShiftDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LShiftDouble")
		->endl();
}

void fay::inst::LShiftString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LShiftString")
		->endl();
}

void fay::inst::LShiftObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LShiftObject")
		->endl();
}

void fay::inst::LShiftFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("LShiftFunction")
		->endl();
}

void fay::inst::RShiftVoid::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("RShiftVoid")
		->endl();
}

void fay::inst::RShiftBool::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("RShiftBool")
		->endl();
}

void fay::inst::RShiftByte::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("RShiftByte")
		->endl();
}

void fay::inst::RShiftInt::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("RShiftInt")
		->endl();
}

void fay::inst::RShiftLong::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("RShiftLong")
		->endl();
}

void fay::inst::RShiftFloat::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("RShiftFloat")
		->endl();
}

void fay::inst::RShiftDouble::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("RShiftDouble")
		->endl();
}

void fay::inst::RShiftString::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("RShiftString")
		->endl();
}

void fay::inst::RShiftObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("RShiftObject")
		->endl();
}

void fay::inst::RShiftFunction::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("RShiftFunction")
		->endl();
}

void fay::inst::NewObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("NewObject")
		->add(" ")->add(this->className)
		->endl();
}

void fay::inst::NullObject::buildString(mirror::utils::StringBuilder * sb)
{
	sb->add("NullObject")
		->add(" ")->add(this->className)
		->endl();
}

//InstEnd
