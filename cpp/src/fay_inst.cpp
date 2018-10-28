#include <fay_inst.h>

//InstStart

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

void fay::inst::IntToBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToBool")
		->endl();
}

void fay::inst::IntToString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("IntToString")
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

void fay::inst::AddBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddBool")
		->endl();
}

void fay::inst::AddString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("AddString")
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

void fay::inst::SubBool::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubBool")
		->endl();
}

void fay::inst::SubString::toString(mirror::utils::StringBuilder * sb)
{
	sb->add("SubString")
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

//InstEnd
