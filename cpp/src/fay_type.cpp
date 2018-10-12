#include <fay_type.h>

using namespace fay;

MAP<fay::ValueType, PTR(FayType)> fay::SimpleType::_Types;

PTR(FayType) fay::SimpleType::Get(ValueType valueType)
{
	auto it = _Types.find(valueType);
	if (it != _Types.end())
		return it->second;

	//没有这个类型，新建一个
	PTR(SimpleType) type(new SimpleType(valueType));
	_Types[valueType] = type;
	return type;
}

const std::string & fay::SimpleType::fullname()
{
	return TypeDict::ToName(this->_type);
}
