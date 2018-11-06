﻿#pragma once
#include <mirror_utils_string.h>
#include <mirror_sys_const.h>
#include <fay_const.h>
#include <string>
#include <exception>

//这个文件主要用于定义一些不同对象之前公用或限定规则的类

namespace fay
{
	//所有对象的基类
	class FayObject
	{
	public:
		FayObject() {}
		virtual ~FayObject() {}

		//生成字符串的方法
		virtual void toString(mirror::utils::StringBuilder* sb) {}
	};

	//用于处理报错信息的异常
	class FayException : public std::exception
	{
	private:
		std::string _filename;  //文件名称
		std::string _trace;  //堆栈
		int _line;  //行号
		int _col;  //列

	public:
		FayException(const std::string &msg);
		FayException(const std::string &msg, const std::string &filename, int line, int col);

		const std::string &filename() { return this->_filename; }
		const std::string &trace() { return this->_trace; }
		int line() { return this->_line; }
		int col() { return this->_col; }
	};

	//文件对象
	class FayFile : public FayObject
	{
	private:
		std::string _filename;

	public:
		FayFile(const std::string &filename)
			: _filename(filename) {}

		const std::string &filename() { return this->_filename; }
	};

	//有索引的Mpa结构
	template<class T>
	class IndexMap
	{
	private:
		T _null;
		std::vector<T> _list;
		MAP<std::string, pos_t> _map;

	public:
		IndexMap() : _null(nullptr) {}

		//数据长度
		pos_t size() { return this->_list.size(); }
		//返回Vector对象
		const std::vector<T> &list() { return this->_list; }

		pos_t add(const std::string &key, T obj)
		{
			//如果没有此对象，添加进来
			auto r = this->_map.find(key);
			if (r == this->_map.end())
			{
				pos_t index = this->_list.size();
				this->_list.push_back(obj);
				this->_map[key] = index;
				return index;
			}
			else
				return r->second;
		}

		inline pos_t findIndex(const std::string &key)
		{
			auto r = this->_map.find(key);
			if (r != this->_map.end())
				return r->second;

			return -1;
		}

		inline T &find(pos_t index)
		{
			if (index >= 0 && index < this->_list.size())
				return this->_list[index];

			return this->_null;
		}

		inline T &find(const std::string &key)
		{
			pos_t index = findIndex(key);
			if (index >= 0)
				return this->_list[index];

			return this->_null;
		}

		inline T &operator[](pos_t index)
		{
			return this->find(index);
		}
	};
}