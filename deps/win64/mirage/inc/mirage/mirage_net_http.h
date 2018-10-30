#pragma once
//这个文件主要用于网络部分对Http的支持
//By Larlf.Wang 2017.5

#include <mirage/mirage_net_server.h>
#include "http_parser/http_parser.h"
#include "test/test_utils.h"

using namespace mirage;
using namespace mirage::net;

namespace mirage
{
	namespace net
	{
		class HttpServer;

		class HttpRequest
		{
		public:
			uint port;
			string url;
			string path;
			string urlData;
			//unordered_map<string, string> data;

			void parseUrl(const string& url);
		};

		class HttpConst
		{
			//是否已经初始化过
			static bool isInited;

			static int OnComplete(http_parser*);
			static int OnUrl(http_parser*, const char *at, size_t length);
			static int OnHeadField(http_parser*, const char *at, size_t length);
			static int OnHeadValue(http_parser*, const char *at, size_t length);
			static int OnBody(http_parser*, const char *at, size_t length);

		public:
			static http_parser_settings ParserSettings;
			static char* Head;

			static void Init()
			{
				//在这里进行一个防止重复初始化的检查
				if (!HttpConst::isInited)
				{
					HttpConst::isInited = true;
					http_parser_settings_init(&HttpConst::ParserSettings);
					ParserSettings.on_message_complete = OnComplete;
					ParserSettings.on_url = OnUrl;
					ParserSettings.on_header_field = OnHeadField;
					ParserSettings.on_header_value = OnHeadValue;
					ParserSettings.on_body = OnBody;
				}

			}
		};

		//一个Http的连接
		class HttpConn : public TCPConn
		{
		private:
			http_parser parser;
			int _t;
			bool _isResponse;
			PTR(HttpServer) server;
			HttpRequest request;

		public:

			HttpConn(PTR(HttpServer) server) : _isResponse(false), server(server)
			{
				LOG_DEBUG("Create HTTP Conn ... " << reinterpret_cast<size_t>(this));
				http_parser_init(&this->parser, HTTP_REQUEST);
				this->parser.data = this;
				this->_t = utils::TimeUtils::TimeStamp();
			}

			~HttpConn()
			{
				LOG_DEBUG("Destory HTTP Conn ... " << reinterpret_cast<size_t>(this));
			}

			int startTime() const { return this->_t; }

			void onData(const byte* data, uint size) override;
			void onClose() override;

			//数据解析时的回调
			void onComplete();
			void onUrl(const string& url);
			void onHeadField(const string& field);
			void onHeadValue(const string& value);
			void onBody(const string& body);

			//返回文字内容
			void response(const string& text);

			string toString() const;
		};

		class HttpServer : public TCPServer
		{
		private:
			unordered_map<void*, PTR(HttpConn)> conns;
			void* beatHandler;

		public:
			HttpServer(uv_loop_t *loop, uint port) : TCPServer(loop, port), beatHandler(nullptr)
			{
				HttpConst::Init();
			}

			virtual void onOpenConn(uv_tcp_t* socket) override;
			virtual void onShutdown() override;

			void onBeat(es::TimerData *data);

			//关闭一个连接
			void onConnClose(void* connId);

			virtual void onRequest(void* connId, const HttpRequest& request) = 0;

			void response(void* connId, const string& text);

			virtual void onStartup() override;

		};
	}
}



