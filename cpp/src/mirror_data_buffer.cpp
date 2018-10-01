#include "mirror_data_buffer.h"

using namespace mirror::data;

ByteData::ByteData(void* data, uint32_t size)
{
	this->set(data, size);
}

ByteData::ByteData(const ByteData &data)
{
	this->set(data._data, data._size);
}

void ByteData::clear()
{
	if (this->_data != nullptr)
	{
		delete[] this->_data;
		this->_data = nullptr;
	}

	this->_size = 0;
}

void ByteData::set(const void* data, uint32_t size)
{
	this->clear();

	if (size > 0)
	{
		this->_size = size;
		this->_data = new byte[size];
#ifdef WIN32
		memcpy_s(this->_data, size, data, size);
#else
		memcpy(this->_data, data, size);
#endif
	}

}

byte mirror::data::ByteData::_NullByte = 0;

ByteBuffer::ByteBuffer()
{
	new (this)ByteBuffer(init_length);
}

ByteBuffer::~ByteBuffer()
{
	delete[] this->_buffer;
}

ByteBuffer::ByteBuffer(uint32_t length)
{
	this->_buffer = new byte[length];
	this->_length = length;
	this->_pos = 0;
	this->_size = 0;
}

ByteBuffer::ByteBuffer(ByteData* data)
{
	if (data != nullptr && data->size() > 0)
	{
		this->_buffer = new byte[data->size()];
		memcpy(this->_buffer, data->data(), data->size());
		this->_length = data->size();
		this->_pos = 0;
		this->_size = data->size();
	}
	else
		new (this)ByteBuffer(init_length);
}

/**
* 对数据空间进行扩展
*/
void ByteBuffer::expand(uint32_t length)
{
	//计算新空间的大小
	uint32_t newLength = this->_length * 2;
	while (newLength < length)
		newLength *= 2;

	//分配新空间
	byte* newBuffer = new byte[newLength];

	//复制数据
	memcpy(newBuffer, this->_buffer, this->_size * sizeof(byte));

	//替换现有的Buffer
	delete[] this->_buffer;
	this->_buffer = newBuffer;
	this->_length = newLength;
}

/**
* 向Buffer中添加数据
*/
void ByteBuffer::write(const byte* data, uint32_t size)
{
	//检查当前空间是否足够
	if (this->_size + size > this->_length)
		this->expand(this->_size + size);

	memcpy(this->_buffer + this->_size, data, size * sizeof(byte));
	this->_size += size;
}

/**
* 从字符串中添加数据
*/
void ByteBuffer::write(const std::string data)
{
	this->write((const byte*)data.data(), data.length());
}

void ByteBuffer::write(int data)
{
	byte buffer[4];

	for (int i = 0; i < 4; i++)
		buffer[3 - i] = ((byte*)&data)[i];

	this->write(buffer, 4);
}

void ByteBuffer::write(uint32_t data)
{
	byte buffer[4];

	for (int i = 0; i < 4; i++)
		buffer[3 - i] = ((byte*)&data)[i];

	this->write(buffer, 4);
}

void ByteBuffer::write(byte data)
{
	this->write(&data, 1);
}

void ByteBuffer::write(ByteBuffer &buffer)
{
	if (this->_size + buffer.size() > this->_length)
		this->expand(this->_size + buffer.size());

	buffer.readBytes(this->_buffer + this->_size, buffer.size());
	this->_size += buffer.size();
}

int ByteBuffer::readInt()
{
	if (this->_size >= 4)
	{
		int v, i;

		v = 0;
		for (i = 0; i < 4; i++)
		{
			v = v << 8;
			v += this->_buffer[this->_pos + i];
		}

		this->_pos += 4;
		return v;
	}

	return 0;
}

uint32_t ByteBuffer::readUInt()
{
	if (this->_size >= 4)
	{
		uint32_t v, i;

		v = 0;
		for (i = 0; i < 4; i++)
		{
			v = v << 8;
			v += this->_buffer[this->_pos + i];
		}

		this->_pos += 4;
		return v;
	}

	return 0;
}

byte ByteBuffer::readByte()
{
	byte v;

	if (this->_size > 0)
	{
		v = this->_buffer[this->_pos];
		this->_pos++;
		return v;
	}

	return 0;
}

/**
* 读取固定长度的二进制数据
*/
void ByteBuffer::readBytes(byte* data, uint32_t size)
{
	if (size + this->_pos > this->_size) size = this->_size - this->_pos;

	if (size > 0)
	{
		memcpy(data, this->_buffer + this->_pos, size);
		this->_pos += size;
	}

}

/**
* 读取字符串
*/
std::string ByteBuffer::readString(uint32_t size)
{
	byte* data = new byte[size];
	this->readBytes(data, size);
	std::string str((char*)data, size);
	delete[] data;
	return str;
}

void ByteBuffer::remove(uint32_t size)
{
	//检查尺寸是否正确
	if (size > this->_size) size = this->_size;

	//计算新空间的大小
	uint32_t newLength = init_length;
	while (newLength < this->_size - size)
		newLength *= 2;

	//分配新空间
	byte* newBuffer = new byte[newLength];

	//复制数据
	memcpy(newBuffer, this->_buffer + size, (this->_size - size) * sizeof(byte));

	//替换现有的Buffer
	delete[] this->_buffer;
	this->_buffer = newBuffer;
	this->_length = newLength;
	this->_size -= size;

	//重新计算指针位置
	if (this->_pos < size) this->_pos = 0;
	else this->_pos -= size;
}

void ByteBuffer::removeEnd(uint32_t size)
{
	if (this->_size > size) this->_size -= size;
	else this->_size = 0;
	if (this->_pos >= this->_size) this->_pos = this->_size - 1;
}

void ByteBuffer::clear()
{
	byte* newBuffer = new byte[init_length];
	delete[] this->_buffer;
	this->_buffer = newBuffer;

	this->_length = init_length;
	this->_size = 0;
	this->_pos = 0;
}

/**
* 显示PackageBuffer的内部结构
*/
void ByteBuffer::debug()
{
	//TODO
	//std::cout << "[PacketBuffer] Length:" << this->_length << " Pos:" << this->_pos << " Size:" << this->_size << std::endl;
	//GameUtils::debugBytes(this->_buffer, this->_length);
}

std::string ByteBuffer::toString()
{
	std::string str((char*)this->_buffer, this->_size);
	return str;
}

PTR(ByteData) ByteBuffer::toBytes()
{
	return MKPTR(ByteData)(this->_buffer, this->_size);
}


