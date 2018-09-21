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
			string name;  //����
			DataType type;  //����
			ByteBuffer _data;  //��������

		public:
			DBPropData(const string &name, int value) : name(name), type(IntType)
			{
				DataUtils::Write(&this->_data, value);
			}
		};

		/**
		 * ���ӵ�DB���Ŀͻ���
		 */
		class DBClient : public TCPPacketClient
		{
		private:
			//��������BatchID
			uint _batchIDIndex;
			ByteBuffer _data;
			int _dataSize;

			//�����ݽ��г�ʼ��
			void clearData();

		public:
			//��Ҫ��OP��������ʹ��ǰ�ȳ�ʼ��
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

			//���ص�ǰ��BatchID
			uint batchID() { return this->_batchIDIndex; }
			//�������е����ݼ�¼
			void sendRecords();
			//����DB������������
			virtual void onPacket(DataPacket& packet) override;
			//�������ݳ־û��ɹ�����Ϣ
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