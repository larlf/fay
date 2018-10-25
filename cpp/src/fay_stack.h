#pragma once
#include <mirror_utils_log.h>
#include <fay_type.h>


namespace fay
{
	class VMStack
	{
	private:
		FayValue** _container = nullptr;
		size_t _capacity = 0;
		size_t _size = 0;

		//修改容器大小
		void _resize(size_t capacity);
		//对容器进行扩展
		void _expand();
		//使容器收缩
		void _shrink();

	public:
		VMStack() : _capacity(1024), _container(new FayValue*[1024]) {}
		~VMStack();

		size_t size() { return this->_size; }

		void push(FayValue* value)
		{
			if (this->_size >= this->_capacity)
				this->_expand();

			this->_container[this->_size++] = value;
		}

		FayValue* pop()
		{
			if (this->_size > 0)
				return this->_container[this->_size-- - 1];

			LOG_ERROR("Stack size is zero");

			return nullptr;
		}

		FayValue* operator[](size_t index)
		{
			if (index < this->_size)
				return this->_container[this->_size - index - 1];

			LOG_ERROR("Overflow stack index : " << index << " size : " << this->_size);

			return nullptr;
		}
	};
}


