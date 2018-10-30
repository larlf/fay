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
		typedef function<void(T_OP op, const byte* data, uint size, void* param)> T_BUS_CALLBACK;

		/**
		 * Bus服进行回调的信息
		 */
		class BusCallbackInfo
		{
		private:
			static uint IDIndex;

		public:
			uint id;
			T_OP op;
			void* param;
			T_BUS_CALLBACK callback;

			BusCallbackInfo(T_OP op, void* param, T_BUS_CALLBACK callback)
				: id(MKID(BusCallbackInfo::IDIndex, 0xFFFFFFFF)), op(op), param(param), callback(callback) {}
		};

		class BusClient : public TCPPacketClient
		{
		private:
			unordered_map<uint, BusCallbackInfo*> callbackMap;
			uint server;
			uint line;

		public:
			BusClient(const string &host, uint port, uint server, uint line)
				: TCPPacketClient(host, port), server(server), line(line) {}

			void call(T_OP op, const byte* data, uint size, void* param, T_BUS_CALLBACK callback);

			virtual void onPacket(DataPacket& packet) override;
			virtual void onBusCall(uint callbackID, T_OP op, const byte* data, uint size);
			virtual void callback(uint callbackID, const byte* data, uint size);

			virtual void onConnect() override;

		};
	}
}