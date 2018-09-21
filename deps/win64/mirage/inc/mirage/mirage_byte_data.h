#pragma once
#include "mirage_const.h"

namespace mirage
{
	class ByteData
	{
	private:
		byte * _data;
		uint _size;

	public:
		ByteData() : _data(nullptr), _size(0) {}
		ByteData(void *data, uint size);
		ByteData(const ByteData &data);

		~ByteData()
		{
			this->clear();
		}

		byte *data()
		{
			return this->_data;
		}

		uint size()
		{
			return this->_size;
		}

		void clear();
		void set(const void *data, uint size);
	};
}
