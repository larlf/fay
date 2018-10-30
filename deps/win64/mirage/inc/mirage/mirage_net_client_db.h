#pragma once

#include "mirage_net_client.h"
#include <functional>
#include <unordered_map>

using namespace mirage;
using namespace mirage::net;

namespace mirage
{
	namespace net
	{
		class DBPropData
		{
		private:
			string name;  //名称
			DataType type;  //类型
			ByteBuffer _data;  //具体数据

		public:
			DBPropData(const string &name, int value) : name(name), type(IntType)
			{
				DataUtils::Write(&this->_data, value);
			}
		};

		/**
		 * 连接到DB服的客户端
		 */
		class DBClient : public TCPPacketClient
		{
		private:
			//用于生成BatchID
			uint _batchIDIndex;
			ByteBuffer _data;
			int _dataSize;

			//对数据进行初始化
			void clearData();

		public:
			//需要的OP，必需在使用前先初始化
			uint OP_db_records;
			uint OP_db_save_ok;

			DBClient(const string &host, uint port)
				: TCPPacketClient(host, port),
				OP_db_records(0),
				OP_db_save_ok(0),
				_batchIDIndex(1),
				_dataSize(0)
			{}

			~DBClient()
			{
			}

			//返回当前的BatchID
			uint batchID() { return this->_batchIDIndex; }
			//发送所有的数据记录
			void sendRecords();
			//处理DB服发来的数据
			virtual void onPacket(DataPacket& packet) override;
			//处理数据持久化成功的信息
			virtual void onDataSave(uint batchID);

			void addCreateRecord(const string &id)
			{
				DataUtils::Write(&this->_data, id);
				DataUtils::Write(&this->_data, (byte)DataOperationType::Create);
				this->_dataSize++;
			}

			void addRemoveRecord(const string &id)
			{
				DataUtils::Write(&this->_data, id);
				DataUtils::Write(&this->_data, (byte)DataOperationType::Remove);
				this->_dataSize++;
			}

			void addModifyRecord(const string &id, const string &name, DataType type, byte* data, uint size)
			{
				DataUtils::Write(&this->_data, id);
				DataUtils::Write(&this->_data, (byte)DataOperationType::Modify);
				DataUtils::Write(&this->_data, name);
				DataUtils::Write(&this->_data, (byte)type);
				DataUtils::Write(&this->_data, data, size);
				//LOG_DEBUG("Modify : "<<type<<", "<<size);
				this->_dataSize++;
			}

			void addModifyRecord(const string &id, const string &name, int value)
			{
				DataUtils::Write(&this->_data, id);
				DataUtils::Write(&this->_data, (byte)DataOperationType::Modify);
				DataUtils::Write(&this->_data, name);
				DataUtils::Write(&this->_data, (byte)DataType::IntType);
				DataUtils::Write(&this->_data, value);
				this->_dataSize++;
			}

			void addModifyRecord(const string &id, const string &name, const string &value)
			{
				DataUtils::Write(&this->_data, id);
				DataUtils::Write(&this->_data, (byte)DataOperationType::Modify);
				DataUtils::Write(&this->_data, name);
				DataUtils::Write(&this->_data, (byte)DataType::StringType);
				DataUtils::Write(&this->_data, value);
				this->_dataSize++;
			}
		};
	}
}