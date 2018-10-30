#include "fay_stack.h"
#include <fay_stack.h>
#include <memory>

void fay::VMStack::_resize(size_t capacity)
{
	if (capacity < this->_size)
		return;

	FayValue **container = new FayValue*[capacity];
	memcpy(container, this->_container, sizeof(FayValue)*this->_size);
	delete[] this->_container;
	this->_container = container;
	this->_capacity = capacity;
}

void fay::VMStack::_expand()
{
	size_t capacity = this->_capacity * 2;
	this->_resize(capacity);
}

void fay::VMStack::_shrink()
{
	//检查是否需要收缩
	if (this->_capacity <= 1024 || this->_size >= this->_capacity / 2)
		return;

	//计算新的size
	size_t capacity = this->_capacity;
	while (capacity > 1024 && capacity > this->_size * 2)
		capacity /= 2;

	this->_resize(capacity);
}

fay::VMStack::~VMStack()
{
	//删除没有用到的元素
	for (size_t i = 0; i < this->_size; ++i)
		delete this->_container[i];

	delete[] this->_container;
	this->_capacity = 0;
	this->_size = 0;
}
