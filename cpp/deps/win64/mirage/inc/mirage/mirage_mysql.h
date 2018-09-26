#ifndef DEW_MYSQL_H
#define DEW_MYSQL_H

#include "mirage_utils.h"
#include <vector>
#include <queue>
#include <mysql.h>

const char STATE_NONE=0;		//没有分配
const char STATE_FREE=1;		//空闲
const char STATE_BUSY=2;		//忙碌

class SQLTask;
class MySQLConfig;

/**
 * 到MySQL连接的一个Session
 */
class MySQLSesssion
{
private:
	bool isCommited;  //是否已经提交了
	MySQLConfig *pool;
	MYSQL conn;

	void close();
	//输出错误信息
	void showError(const string &sql);
	void showError();

public:
	//bool isError;		///< Session是在一个错误的状态
	bool isConnected;  //是否已经连接

	MySQLSesssion(MySQLConfig *pool);
	~MySQLSesssion();
	bool connect();
	void reconnect();
	void commit();
	void rollback();
	bool execute(const string sql);
	string query(const string sql);
	//执行更新语句并返回更新的数量
	long update(const string sql);
	bool ping();
};


/**
 * SQL连接池
 */
class MySQLConfig
{
public:
	string host;
	uint port;
	string username;
	string password;
	string schema;

	MySQLConfig() : port(0) {}
};

#endif
