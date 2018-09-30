#pragma once

#include "mirror_sys_const.h"

typedef unsigned char byte;		///< 字节数据类型

namespace mirror
{
	namespace data
	{
		class ByteData
		{
		private:
			byte* _data = nullptr;
			uint32_t _size = 0;

		public:
			ByteData() {}
			ByteData(void* data, uint32_t size);
			ByteData(const ByteData &data);

			~ByteData()
			{
				this->clear();
			}

			/**
			 * 取得数据
			 */
			inline byte* data()
			{
				return this->_data;
			}

			/**
			 * 取得长度
			 */
			inline uint32_t size()
			{
				return this->_size;
			}

			/**
			 * 取按定的位置的数据
			 */
			inline byte at(uint32_t index)
			{
				return (index < 0 || index >= this->_size) ? 0 : this->_data[index];
			}

			void clear();
			void set(const void* data, uint32_t size);
		};

		/**
		* 用于对发送过来的网络数据进行缓存
		*/
		class ByteBuffer
		{
		private:
			byte* _buffer;						///< 指向数据缓冲区
			uint32_t _length;					///< 缓冲区的长度
			uint32_t _pos;						///< 当前数据在缓冲区的位置
			uint32_t _size;						///< 当前数据的长度
			enum { init_length = 8 };			///< 初始化时的长度

			void expand(uint32_t length);

		public:
			ByteBuffer();
			~ByteBuffer();
			ByteBuffer(uint32_t length);
			ByteBuffer(ByteData* data);

			byte operator [](uint32_t pos)
			{
				return this->_buffer[pos];
			}

			byte* buffer()
			{
				return this->_buffer;
			}

			uint32_t length()
			{
				return this->_length;
			}

			uint32_t size()
			{
				return this->_size;
			}

			uint32_t unreadSize()
			{
				return this->_size - this->_pos;
			}

			uint32_t freeSize()
			{
				return this->_length - this->_size;
			}

			uint32_t pos()
			{
				return this->_pos;
			}

			void setPos(uint32_t pos)
			{
				this->_pos = (pos >= this->_size) ? this->_size - 1 : pos;
			}

			void write(const byte* data, uint32_t size);
			void write(const std::string data);
			void write(int data);
			void write(uint32_t data);
			void write(const byte data);
			void write(ByteBuffer &buffer);

			int readInt();
			uint32_t readUInt();
			byte readByte();
			void readBytes(byte* data, uint32_t size);
			std::string readString(uint32_t size);

			void remove(uint32_t size);
			void removeEnd(uint32_t size);
			void clear();
			void debug();
			std::string toString();
			PTR(ByteData) toBytes();
		};
	}
}