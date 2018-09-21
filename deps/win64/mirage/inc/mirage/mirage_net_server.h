#pragma once

#include <mirage/mirage_net_socket.h>

using namespace mirage;
using namespace mirage::net;

namespace mirage
{
	namespace net
	{
		/**
		 * TCP服务器，和UDP不同，这种服务器会生成并管理多个连接
		 */
		class TCPServer : public enable_shared_from_this<TCPServer>
		{
		protected:
			uv_loop_t *loop;  ///<事件总线
			uint port;  ///<端口
			uv_stream_t* handle;  ///<服务器的Handle

			/**
			 * 新开启连接后的回调
			 */
			static void _OpenConn(uv_stream_t *hServer, int status);

			/**
			 * 关闭服务器的回调
			 */
			static void _Stop(uv_handle_t *socket);

		public:
			TCPServer(uv_loop_t *loop, uint port)
				: loop(loop), port(port), handle(nullptr) 
			{
				//先检查下网络系统是否有初化
				if (!NetConfig::IsInited())
				{
					LOG_ERROR("Please initialize the network system first");
					exit(0);
				}
			}

			virtual ~TCPServer() 
			{
				//如果还有Handle，要通知关闭
				if (this->handle)
				{
					this->shutdown();
				}
			}

			/** 启动服务器 */
			virtual void startup();
			/** 关闭服务器 */
			virtual void shutdown();

			/**
			 * 这个方法决定了创建哪种类型的Conn
			 * 如果要创建自定义类型的需要重载此方法
			 */
			virtual void onOpenConn(uv_tcp_t* socket) = 0;

			/** 启动服务器事件 */
			virtual void onStartup() {}
			/** 关闭服务器事件 */
			virtual void onShutdown() {}
		};
	
	}
}

