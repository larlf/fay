#pragma once

#include <mirage/mirage_net_socket.h>

using namespace mirage;
using namespace mirage::net;

namespace mirage
{
	namespace net
	{
		/// 客户端的状态
		enum ClientState
		{
			Idle,  ///< 空闲
			Connecting,  ///< 连接中
			Connected,  ///< 已经连接
			Retry  ///< 重试中
		};

		/**
		* TCP的客户端
		*/
		class TCPClinet : public TCPConn
		{
		private:
			string host;
			uint port;
			ClientState state;
			uint retryTime;

			void* timerHandle;

			//连接事件的回调
			static void _Connect(uv_connect_t* connect, int status);
			//关闭连接的回调事件，这里只对不需要删除连接的回调进行处理
			static void _Close(uv_handle_t* socket);

			void doRetry();
			void onRetry(es::TimerData* td);
			void doConnect();

		public:
			TCPClinet(const string &host, uint port)
				: host(host), port(port), state(ClientState::Idle), retryTime(2000), timerHandle(nullptr) {}

			virtual ~TCPClinet() 
			{
				//如果有定时器，要停止
				if (this->timerHandle)
				{
					es::TimerBus::Stop(this->timerHandle);
				}
			}

			/**
			* 连接到服务器
			*/
			virtual void connect();
			/**
			* 从服务器断开
			*/
			virtual void disconnect();

			virtual void onClose() override;

			virtual void onConnectError(int status, const char* msg);
			virtual void onConnect() {}
		};

		/**
		 * 支持Packet的客户端
		 */
		class TCPPacketClient : public TCPClinet
		{
		private:
			PacketModule mPacket;

		public:
			TCPPacketClient(const string &host, uint port)
				: TCPClinet(host, port) 
			{
				//绑定事件
				this->mPacket.onPacketHandler = BIND1(TCPPacketClient::onPacket, this);
				this->mPacket.onOutputHandler = BIND2(TCPPacketClient::send, this);
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

