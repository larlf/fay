#pragma once

#include <string>
#include "mirage_net_const.h"
#include "mirage_byte_buffer.h"

using namespace mirage;

namespace mirage
{
	namespace net
	{
		///四字节的类型
		union T_OP
		{
			T_OP() { this->uintVal = 0; }
			T_OP(uint value) { this->uintVal = value; }
			T_OP(uint server, uint line, uint operation)
			{
				this->opVals.server = server;
				this->opVals.line = line;
				this->opVals.operation = operation;
			}

			uint uintVal;  ///< 无符号整数值
			byte byteVals[4];  ///< 字节值
			struct  
			{
				uint operation : 16;  ///< 操作编号
				uint line : 8;  ///< 分线编号
				uint server : 8;  ///< 服务器编号
			} opVals;  ///< 用在OP时的含义
		};

		//Info的类型
		union T_Info
		{
			T_Info() { this->uintVal = 0; }
			T_Info(uint value) { this->uintVal = value; }

			uint uintVal;  ///< 无符号整数值
			byte byteVals[4];  ///< 字节值
		};

		//验证码的类型
		union T_CheckCode
		{
			T_CheckCode() { this->uintVal = 0; }
			T_CheckCode(uint value) { this->uintVal = value; }

			uint uintVal;  ///< 无符号整数值
			byte byteVals[4];  ///< 字节值
		};

		///客户端ID的类型
		union T_ClientID
		{
			T_ClientID() { this->uintVal = 0; }
			T_ClientID(uint value) { this->uintVal = value; }
			T_ClientID(uint line, uint num)
			{
				this->connVals.line = line;
				this->connVals.num = num;
			}

			uint uintVal;  ///< 无符号整数值
			struct
			{
				uint num : 28;  ///< 在当前线的具体值
				uint line : 4;  ///< 哪条线，最多支持16条线
			} connVals;  ///< 用在连接服的含义
		};

		/// 数据包
		class DataPacket
		{
		public:
			uint index;   ///< 自增序列，用于区分包的先后，在TCP下用处不大，在UDP下可以用于判断包的先后顺序
			T_OP op;  ///< 操作代码
			T_ClientID clientID;  ///< 客户端的ID，此值由连接服填充，和客户端通讯的时候为加密辅助数据
			T_Info info;  ///< 附加信息
			ByteBuffer data;  ///< 数据内容

			DataPacket() : index(0)
			{
				//初始化所有的值为空
				this->op.uintVal = 0;
				this->clientID.uintVal = 0;
				this->info.uintVal = 0;
			}

			~DataPacket()
			{
			}

			/** 取包的长度 */
			inline int size()
			{
				return this->data.size() + SOCKET_OTHER_LEN;
			}

			/**
			 * 生成数据包
			 * @return 二进制数据流
			 */
			ByteBuffer* build();

			/**
			 * 对数据进行解析<br>
			 * 注意此操作不会修改数据，对数据的候和回收到都放在上一级处理
			 * @param buffer 数据包
			 * @return 返回读取数据的长度，如果出错返回0
			 */
			uint parse(ByteBuffer* buffer);

			/** 写入二进制数据 */
			void setData(const byte *data, size_t size);
			/** 写入字符串数据 */
			void setData(const string &str);
			/** 清理现在所有的数据 */
			void clearData();
			/** 从数据包获取字符串数据 */
			string getDataToString();
			/** 生成验证码 */
			T_CheckCode makeCheckCode();
			/** 对数据进行加密 */
			void encode(byte* key, size_t keySize);
			/** 对数据进行解密 */
			void decode(byte* key, size_t keySize);

			/** 显示内容部数据 */
			string dump();
		};

		/**
		 * 对数据进行分隔的模块
		 */
		class PackModule
		{
		private:
			ByteBuffer _buffer;  ///<缓存

			uint _pos;  ///<当前指向的位置
			uint _size;  ///<当前要读取的数据长度
			bool _isError;  ///<是否是在错误状态

			/** 读取数据包 */
			void readPack();
			/** 读取错误的数据包 */
			void readErrorPack();
			/** 判断是否是分隔符 */
			bool isDelimiter(const byte* data);
			/** 从缓存中删除数据 */
			void removeData(uint size);

		public:
			function<void(const byte*, uint)> onPackHandler;  ///<分隔数据后的处理
			function<void(const byte*, uint)> onErrorDataHandler;  ///<处理错误的数据
			function<void(const byte*, uint)> onOutputHandler;  ///<发送数据的处理

			PackModule() :
				_pos(0), _size(0), _isError(false),
				onPackHandler(nullptr), onErrorDataHandler(nullptr)
			{}

			virtual ~PackModule()
			{
			}

			/**
			 * 向模块中输入数据
			 */
			void input(const byte* data, uint size);

			/**
			 * 通过模块发送数据
			 */
			void send(const byte* data, uint size);
		};


		/**
		 * 用于处理Packet的网络模块
		 */
		class PacketModule
		{
		private:
			ByteBuffer _buffer;  ///<数据缓存，读取的内容如果没有处理，都会放在这里
			uint readPacektSize;  ///<当前需要读取的数据包的长度
			bool isErrorPacket;  ///<是否是错误的数据

			/**
			 * 从Buffer里读取数据包
			 */
			void readPacket();

		public:
			function<void(const byte*, uint)> onOutputHandler;  ///<发送数据时调用的方法
			function<void(DataPacket&)> onPacketHandler;  ///<当接收到数据包时调用的方法

			PacketModule()
				: isErrorPacket(false), readPacektSize(0), onOutputHandler(nullptr), onPacketHandler(nullptr)
			{
			}

			virtual ~PacketModule()
			{
			}

			/**
			 * 向Packet模块写数据，提供给外部调用
			 */
			void write(const byte* data, uint size);
			/**
			 * 发送数据
			 * @param packet 要发送的数据包
			 */
			virtual void send(DataPacket &packet);
		};

	}
}
