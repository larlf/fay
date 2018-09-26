#pragma once

/*********************************
 基于LibUV的各种事件相关处理
**********************************/

#include <uv.h>
#include <iostream>
#include <string>
#include <functional>
#include <unordered_map>
#include <mirage/mirage_utils.h>

//using namespace mirage;

namespace mirage
{
	//因为event是关键字，这里用es=Event System
	namespace es
	{
		/** 对LibUV里Buffer的封装 */
		class UVBuffer
		{
		public:
			uv_buf_t buffer;  ///< LibUV里的Buffer结构

			/**
			 * @param size 缓冲区的长度
			 */
			UVBuffer(size_t size)
			{
				//cout << "Create UVBuffer" << endl;
				//this->buffer = uv_buf_init(new char[size], size);
				this->buffer.base = new char[size];
				this->buffer.len = size;
			}

			virtual ~UVBuffer()
			{
				//cout << "Destory UVBuffer" << endl;
				if (this->buffer.base)
				{
					delete[] this->buffer.base;
					this->buffer.base = nullptr;
				}
			}
		};

		///////////////////////////////////////////////////////////////////////////////////////////////

		/**
		 * 事件总线
		 */
		class EventBus
		{
		public:
			static void Start()
			{
				uv_loop_t* loop = uv_default_loop();
				uv_run(loop, UV_RUN_DEFAULT);
			}

			static void Stop();
		};

		///////////////////////////////////////////////////////////////////////////////////////////////

		const size_t TTYBufferSize = 10240;  ///< 输入处理时的缓存大小

		/**
		 * 用于处理键盘输入的事件总线
		 */
		class TTYBus
		{
		private:
			static uv_tty_t tty;  ///<TTY事件句柄
			static bool IsRuning;  ///<是否已经运行
			static UVBuffer* buf;  ///<缓存管理器

			TTYBus() {}

			/** 申请内存时的回调 */
			static void _Alloc(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
			/** 有输入事件时的回调味 */
			static void _Input(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);

		public:
			///当有数据事件发时的回调方法
			static function<void(const string&)> OnInputHandler;
			///当有错误事件发生时的回调方法
			static function<void(int, const char*)> OnErrorHandler;

			/** 开启输入事件的监听 */
			static void Start();
			/** 关闭输入事件的监听 */
			static void Stop();
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////

		class TimerBus;

		/** 定时器回调时传送的数据结构 */
		struct TimerData
		{
			uv_timer_t *handle;  ///<uv中的handle
			function<void(TimerData*)> callbackFun;  ///<回调函数
			function<void(TimerData*)> completeFun;  ///<结束时的回调函数
			uint64 time;  ///<回调间隔
			uint times;  ///<回调的次数
			uint realTimes;  ///<实际已经执行的次数
			void* param;  ///<附加参数
			bool isCancel;  ///<已经被取消
		};

		/** 定时器总线 */
		class TimerBus
		{
		private:
			static MAP<TimerData*, void*> _Timers;  ///<所有定时器的实例，用于统一的管理和结束后清理资源

			TimerBus() {}

			/** 定时回调事件 */
			static void _OnTimer(uv_timer_t *handle);
			/** 关闭定时器的回调事件 */
			static void _OnClose(uv_handle_t* handle);

		public:

			/**
			 * 启动一个定时器事件
			 * @param callback 到时的回调
			 * @param time 回调时间，毫秒
			 * @param times 回调次数，0表示永远执行
			 * @param complete 定时器结束时的回调
			 * @param param 回调时需要回传的参数
			 * @return 定时器的句柄
			 */
			static void* Start(function<void(TimerData*)> callback, uint64 time, uint times = 0, function<void(TimerData*)> complete = nullptr, void* param = nullptr);

			/**
			* 启动一个只调用一次的定时器
			* @param callback 到时的回调
			* @param time 回调时间，毫秒
			* @param param 回调时需要回传的参数
			* @return 定时器的句柄
			*/
			static void* Once(function<void(TimerData*)> callback, uint64 time, void* param = nullptr)
			{
				return Start(callback, time, 1, nullptr, param);
			}

			/**
			 * 结束一个定时器事件
			 * @param handle 要结束的定时器的句柄
			 */
			static void Stop(void* handle);

			/**
			 * 结束所有的定时器事件
			 */
			static void StopAll();
		};
	}
}




