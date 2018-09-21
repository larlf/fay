#pragma once

#include <string.h>
#include <vector>
#include <memory>
#include <mirage/mirage_utils.h>
#include <mirage/mirage_byte_buffer.h>

namespace mirage
{
	class DataPropDesc;

	const int ReadError = -1;

	//数据类型
	enum DataType
	{
		NullType = 0,
		ByteType,
		IntType,
		LongType,
		FloatType,
		DoubleType,
		BoolType,
		StringType,
		ArrayType,
		MapType,
		ObjType,
		MaxValue
	};

	//操作类型
	enum DataOperationType
	{
		Unknow = 0,
		Create = 1,
		Modify = 2,
		Remove = 3
	};

	//所有具体数据的父类
	class IDataObj
	{
	private:
		string _type;
		int _ver;

	public:
		IDataObj(const string &type, int ver)
			: _type(type), _ver(ver) {}
		string getType()
		{
			return this->_type;
		}
		int getVer()
		{
			return this->_ver;
		}

		//从数据中解析对象
		//返回值是读取到的数据长度
		virtual int readFromBytes(ByteBuffer *data, int pos) = 0;
		//根据对象生成数据
		virtual int writeToBytes(ByteBuffer *buffer) = 0;
	};

	//数据处理的一些公用方法
	class DataUtils
	{
	private:
		template<typename T>
		static int _Read(T *v, ByteBuffer *bytes, int pos, int size)
		{
			//数据是否足够多
			if (bytes->size() < (pos + size))
			{
				LOG_ERROR("Buffer Size Error");
				return ReadError;
			}

			memcpy(v, bytes->buffer() + pos, size);
			return size;
		}

	public:
		//检查是否是读取错误，如果不是，pos累加
		static bool IsReadError(int r, int *pos)
		{
			if (r == mirage::ReadError)
				return true;

			(*pos) += r;
			return false;
		}

		static int Read(int *v, ByteBuffer *bytes, int pos)
		{
			return _Read<int>(v, bytes, pos, 4);
		}

		static int Read(float *v, ByteBuffer *bytes, int pos)
		{
			return _Read<float>(v, bytes, pos, 4);
		}

		static int Read(uint *v, ByteBuffer *bytes, int pos)
		{
			return _Read<uint>(v, bytes, pos, 4);
		}

		static int Read(int64 *v, ByteBuffer *bytes, int pos)
		{
			return _Read<int64>(v, bytes, pos, 8);
		}

		static int Read(double *v, ByteBuffer *bytes, int pos)
		{
			return _Read<double>(v, bytes, pos, 8);
		}

		static int Read(bool *v, ByteBuffer *bytes, int pos)
		{
			return _Read<bool>(v, bytes, pos, 1);
		}

		static int Read(byte *v, ByteBuffer *bytes, int pos)
		{
			return _Read<byte>(v, bytes, pos, 1);
		}

		static int Read(string *v, ByteBuffer *data, int pos)
		{
			int size;
			pos += mirage::DataUtils::Read(&size, data, pos);

			if (size < 0)
			{
				LOG_ERROR("String size less zero : " << size);
				return ReadError;
			}

			if (data->size() < (pos + size))
			{
				LOG_ERROR("String's Buffer Size Error");
				return ReadError;
			}

			//读取字符串数据
			v->clear();
			v->append((char *)(data->buffer() + pos), size);
			return size + 4;
		}

		//读取对象
		template<typename T>
		static int Read(shared_ptr<T> &obj, ByteBuffer *data, int pos)
		{
			int version = 0;
			int size = Read(&version, data, pos);
			if (ReadError == size)
			{
				LOG_ERROR("Read object version error");
				return ReadError;
			}

			//对象是空
			if (version == 0)
			{
				if (obj != nullptr)
					obj.reset();
				return size;
			}

			//创建对象
			obj = shared_ptr<T>(new T());
			if (version != obj->getVer())
			{
				LOG_ERROR("Object version error for " << obj->getType() << " wish : " << version << " real : " << obj->getVer());
				return ReadError;
			}

			//读取对象数据
			size += obj->readFromBytes(data, pos + size);
			return size;
		}

		template<typename T>
		static int ReadArray(vector<T> &values, ByteBuffer *data, int pos)
		{
			int length = 0;
			int size = Read(&length, data, pos);
			if (ReadError == size)
			{
				LOG_ERROR("Read array length error");
				return ReadError;
			}

			//添加的所有的对象
			int r = 0;
			for (auto i = 0; i < length; ++i)
			{
				T value;
				if (ReadError == (r = Read(&value, data, pos + size)))
					return ReadError;
				size += r;
				values.push_back(value);
			}

			return size;
		}

		template<typename T>
		static int ReadObjArray(vector<shared_ptr<T>> &values, ByteBuffer *data, int pos)
		{
			int length = 0;
			int size = Read(&length, data, pos);
			if (ReadError == size)
			{
				LOG_ERROR("Read array length error");
				return ReadError;
			}

			//添加的所有的对象
			int r = 0;
			for (auto i = 0; i < length; ++i)
			{
				shared_ptr<T> value;
				if (ReadError == (r = Read<T>(value, data, pos + size)))
					return ReadError;
				size += r;
				values.push_back(value);
			}

			return size;
		}

		template<typename T>
		static int ReadMap(unordered_map<string, T> &values, ByteBuffer *data, int pos)
		{
			int length = 0;
			int size = Read(&length, data, pos);
			if (ReadError == size)
			{
				LOG_ERROR("Read map length error");
				return ReadError;
			}

			int r = 0;
			for (auto i = 0; i < length; ++i)
			{
				string key;
				if (ReadError == (r = Read(&key, data, pos + size)))
					return ReadError;
				size += r;

				T value;
				if (ReadError == (r = Read(&value, data, pos + size)))
					return ReadError;
				size += r;

				values[key] = value;
			}

			return size;
		}

		template<typename T>
		static int ReadObjMap(unordered_map<string, shared_ptr<T>> &values, ByteBuffer *data, int pos)
		{
			int length = 0;
			int size = Read(&length, data, pos);
			if (ReadError == size)
			{
				LOG_ERROR("Read map length error");
				return ReadError;
			}

			int r = 0;
			for (auto i = 0; i < length; ++i)
			{
				string key;
				if (ReadError == (r = Read(&key, data, pos + size)))
					return ReadError;
				size += r;

				shared_ptr<T> value;
				if (ReadError == (r = Read<T>(value, data, pos + size)))
					return ReadError;
				size += r;

				values[key] = value;
			}

			return size;
		}

		static int Write(ByteBuffer *buffer, const byte *data, uint size)
		{
			//g_byte_array_append(buffer, data, size);
			buffer->write(data, size);
			return size;
		}

		static int Write(ByteBuffer *buffer, byte v)
		{
			//g_byte_array_append(buffer, &v, 1);
			buffer->write(&v, 1);
			return 1;
		}

		static int Write(ByteBuffer *buffer, const int v)
		{
			//g_byte_array_append(buffer, (byte *)&v, 4);
			buffer->write((byte*)&v, 4);
			return 4;
		}

		static int Write(ByteBuffer *buffer, const uint v)
		{
			//g_byte_array_append(buffer, (byte *)&v, 4);
			buffer->write((byte*)&v, 4);
			return 4;
		}

		static int Write(ByteBuffer *buffer, const uint64 v)
		{
			//g_byte_array_append(buffer, (byte *)&v, 8);
			buffer->write((byte*)&v, 8);
			return 8;
		}

		static int Write(ByteBuffer *buffer, const double v)
		{
			//g_byte_array_append(buffer, (byte *)&v, 8);
			buffer->write((byte*)&v, 8);
			return 8;
		}

		static int Write(ByteBuffer *buffer, const string &v)
		{
			int size = v.size();
			//g_byte_array_append(buffer, (byte *)&size, 4);
			buffer->write((byte*)&size, 4);
			//g_byte_array_append(buffer, (byte *)v.data(), size);
			buffer->write((byte*)v.data(), size);
			return size + 4;
		}

		static int Write(ByteBuffer *buffer, ByteBuffer *data)
		{
			if (data)
			{
				//g_byte_array_append(buffer, data->data, data->len);
				buffer->write(data->buffer(), data->size());
				return data->size();
			}

			return 0;
		}

		template<typename T>
		static int Write(ByteBuffer *buffer, const shared_ptr<T> &v)
		{
			int size = 0;
			if (v != nullptr)
			{
				int version = v->getVer();
				size += Write(buffer, version);
				size += v->writeToBytes(buffer);
			}
			else
			{
				size += Write(buffer, (int)0);
			}

			return size;
		}

		template<typename T>
		static int WriteArray(ByteBuffer *buffer, const vector<T> &values)
		{
			int length = values.size();
			int size = Write(buffer, length);

			for (auto i = 0; i < length; ++i)
			{
				size += Write(buffer, values[i]);
			}

			return size;
		}

		template<typename T>
		static int WriteObjArray(ByteBuffer *buffer, const vector<shared_ptr<T>> &values)
		{
			int length = values.size();
			int size = Write(buffer, length);

			for (auto i = 0; i < length; ++i)
			{
				size += Write<T>(buffer, values[i]);
			}

			return size;
		}

		template<typename T>
		static int WriteMap(ByteBuffer *buffer, const unordered_map<string, T> &values)
		{
			int length = values.size();
			int size = Write(buffer, length);

			for (auto it = values.begin(); it != values.end(); ++it)
			{
				size += Write(buffer, it->first);
				size += Write(buffer, it->second);
			}

			return size;
		}

		template<typename T>
		static int WriteObjMap(ByteBuffer *buffer, const unordered_map<string, shared_ptr<T>> &values)
		{
			int length = values.size();
			int size = Write(buffer, length);

			for (auto it = values.begin(); it != values.end(); ++it)
			{
				size += Write(buffer, it->first);
				size += Write<T>(buffer, it->second);
			}

			return size;
		}
	};

	//字段说明信息
	class DataPropDesc
	{
	public:
		string name;
		DataType type;
		DataType subType;
	};

	//数据说明信息
	class DataDesc
	{
	public:
		uint id;
		string name;
		vector<DataPropDesc *> props;
	};

}
