#pragma once

#include <mirage/mirage_utils.h>
#include <mirage/mirage_event.h>
#include <mirage/mirage_net_packet.h>
#include <mirage/mirage_obj.h>
#include <uv.h>
#include <memory>
#include <queue>
#include <unordered_map>
#include <functional>

namespace mirage
{
	namespace net
	{
		class IConn : public obj::IObject
		{
		public:
			virtual void close() = 0;
		};

		/**
		 * 代表一个Socket连接<br>
		 * 数据的Buffer层在这里处理，然后交付业务层。
		 * 注意，Conn在关闭会会自动进行delete，所有的引用需要在onClose()中进行处理。
		 */
		class TCPConn : public IConn, public enable_shared_from_this<TCPConn>
		{
		private:
			//申请缓存的回调事件
			static void _Alloc(uv_handle_t* socket, size_t suggested_size, uv_buf_t* buf);
			//读到数据的回调事件
			static void _Read(uv_stream_t* socket, ssize_t nread, const uv_buf_t* buf);
			//写入数据的回调事件
			static void _Send(uv_write_t* req, int status);
			//关闭连接的回调事件
			static void _Close(uv_handle_t* socket);
			//清理发送的数据
			static void DeleteSendReq(uv_write_t *req);
			static void DeleteBuf(uv_buf_t *buf);

		protected:
			uv_tcp_t* socket;  //绑定读取数据的句柄
			es::UVBuffer readBuf;  //读取数据的缓存区
			queue<uv_buf_t*> sendQueue;  //发送数据队列
			bool sending;  //发送数据时的锁

			/** 发送Buffer数据 */
			virtual void sendBuf(uv_buf_t *buf);
			/** 检查并发送队列中的数据 */
			virtual void checkSendQueue();

		public:
			TCPConn() : socket(nullptr), readBuf(SOCKET_BUFFER_SIZE), sending(false) 
			{
				//先检查下网络是不是有初始化
				if (!NetConfig::IsInited())
				{
					LOG_ERROR("Please initialize the network system first");
					exit(0);
				}
			}

			virtual ~TCPConn();

			/** 用于显示此对象的信息 */
			virtual string info();

			/**
			 * 启动这个Socket
			 * @param socket 传入的TCP连接句柄
			 */
			void open(uv_tcp_t *socket);

			/** 停止这个Socket */
			virtual void close() override;

			/**
			 * 向这个连接发送数据
			 */
			virtual void send(const byte* data, uint size);

			/**
			 * 发送字符串数据
			 */
			virtual void sendStr(const string& str);

			/**
			* 有输入的数据需要读取时的回调
			* @param data 数据地址
			* @param size 数据度度
			*/
			virtual void onData(const byte* data, uint size) = 0;

			/**
			 * 发生读数据错误时的回调
			 * @param status 错误代码
			 * @param msg 错误信息
			 */
			virtual void onReadError(int status, const char* msg);

			/** 
			* 连接开启事件，它表示传入新的连接并保证可用 
			*/
			virtual void onOpen() {}

			/**
			 * 关闭事件，表示连接不可用并将被回收
			 */
			virtual void onClose() {}
		};

		/**
		 * 可以处理Packet的Socket连接
		 */
		class TCPPacketConn : public TCPConn
		{
		private:
			PacketModule mPacket;

		public:
			TCPPacketConn()
			{
				//绑定事件
				this->mPacket.onPacketHandler = BIND1(TCPPacketConn::onPacket, this);
				this->mPacket.onOutputHandler = BIND2(TCPPacketConn::send, this);
			}

			//处理模块间的调用
			virtual void onData(const byte* data, uint size) override
			{
				this->mPacket.write(data, size);
			}

			/**
			 * 提供的事件
			 */
			virtual void onPacket(DataPacket& packet) {}

			/**
			 * 提供发送Packet的方法
			 */
			virtual void sendPacket(DataPacket& packet)
			{
				this->mPacket.send(packet);
			}
		};
	
	}
}
