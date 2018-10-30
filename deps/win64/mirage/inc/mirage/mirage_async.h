#pragma once

#include <mirage/mirage_utils.h>
#include <uv.h>
#include <functional>
#include <unordered_map>

using namespace mirage;
using namespace mirage::utils;

namespace mirage
{
	namespace async
	{
		class AsyncData
		{
		public:
			uv_async_t* handle;
			uint64 startTime;
			void* param;
			function<void(void*, void*)> callback;
			
			AsyncData(uv_loop_t* loop, uv_async_t* handle, function<void(void*, void*)> callback, void* param);
			~AsyncData();
		};

		class AsyncBus
		{
		private:
			static MAP<uv_async_t*, PTR(AsyncData)> AsyncCallbacks;
			static void _callback(uv_async_t* handle);
			static void _close(uv_handle_t* handle);

		public:
			static uv_async_t* CreateHandle(function<void(void*, void*)> callback, void* param);
			static void Callback(uv_async_t* handle, void* data);
		};

	}
}