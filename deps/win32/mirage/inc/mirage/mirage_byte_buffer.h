#pragma once
#include "mirage_utils.h"
#include "mirage_byte_data.h"
#include <string>

namespace mirage
{
	/**
	* 用于对发送过来的网络数据进行缓存
	*/
	class ByteBuffer
	{
	private:
		byte * _buffer;					///< 指向数据缓冲区
		uint _length;					///< 缓冲区的长度
		uint _pos;						///< 当前数据在缓冲区的位置
		uint _size;						///< 当前数据的长度
		enum { init_length = 8 };		///< 初始化时的长度

		void expand(uint length);

	public:
		ByteBuffer();
		~ByteBuffer();
		ByteBuffer(uint length);

		byte operator [](uint pos)
		{
			return this->_buffer[pos];
		}

		byte *buffer()
		{
			return this->_buffer;
		}

		uint length()
		{
			return this->_length;
		}

		uint size()
		{
			return this->_size;
		}

		uint unreadSize()
		{
			return this->_size - this->_pos;
		}

		uint freeSize()
		{
			return this->_length - this->_size;
		}

		uint pos()
		{
			return this->_pos;
		}

		void setPos(uint pos)
		{
			this->_pos = (pos >= this->_size) ? this->_size - 1 : pos;
		}

		void write(const byte *data, uint size);
		void write(const std::string data);
		void write(int data);
		void write(uint data);
		void write(const byte data);
		void write(ByteBuffer &buffer);
		//void write(Json::Value &json);

		int readInt();
		uint readUInt();
		byte readByte();
		void readBytes(byte *data, uint size);
		std::string readString(uint size);

		//void encode(EncodeFun fun);
		//void encode(std::vector<EncodeFun>& funs);
		//void decode(EncodeFun fun);
		//void decode(std::vector<EncodeFun>& funs);

		void remove(uint size);
		void removeEnd(uint size);
		void clear();
		void debug();
		std::string toString();
		PTR(ByteData) toBytes();
		//Json::Value toJSON();
	};
}
