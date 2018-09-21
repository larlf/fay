#pragma once

/******************************************
* 网络相关的基础设置
******************************************/

#include <mirage/mirage_data.h>

namespace mirage
{
	namespace net
	{
		//数据包里的一些常量
		const byte SOCKET_DELIMITER[] = { 0xFF, 0xFF, 0xFF, 0xFF };  ///< 数据包的结束符
		const uint SOCKET_DELIMITER_LEN = sizeof(SOCKET_DELIMITER);  ///< 数据包结束符的长度
		const uint SOCKET_HEAD_LEN = 20;  ///< 数据包头部的长度
		const uint SOCKET_OTHER_LEN = SOCKET_HEAD_LEN + SOCKET_DELIMITER_LEN;  ///< 除数据外的长度
		const size_t SOCKET_BUFFER_SIZE = 10240;  ///< 网络数据处理的缓存大小

		//对INFO字段里的一些定义
		const uint INFO_NUM = 0;				//存放序号的位置
		const uint INFO_TYPE = 1;				//存放处理类型的位置
		const byte BUS_TYPE_REGISTER = 1;		//注册信息
		const byte BUS_TYPE_REQUEST = 2;		//请求信息
		const byte BUS_TYPE_RESPONSE = 4;		//返回信息

		//系统级的OP（要求在0x00FF以内）
		const uint OP_DB_SAVE = 0x0031;

		/**
		* 网络相关的配置
		*/
		class NetConfig
		{
		private:
			static bool _isInited;  //是否已经初始化

		public:
			//是否在调试状态下显示Soacket数据里的详细信息
			static bool DebugSocketData;

			/**
			 * 对网络环境进行初始化，以消除在不同平台下的网络问题
			 */
			static void Init();

			/**
			 * 用于外部判断是否已经初始化
			 */
			static bool IsInited() { return NetConfig::_isInited; }
		};
	}
}


