﻿#pragma once
#include <mirror_sys_const.h>
#include <mirror_utils_string.h>
#include <fay_const.h>
#include <string>
#include <exception>

//这个文件主要用于定义一些不同对象之前公用或限定规则的类

namespace fay
{
	template<class T> class IndexMap;

	//所有对象的基类
	class BaseObject
	{
	public:
		BaseObject() {}
		virtual ~BaseObject() {}

		//生成字符串的方法
		std::string toString();
		virtual void buildString(mirror::utils::StringBuilder* sb) {}
	};

	//文件对象
	class FayFile : public BaseObject
	{
	private:
		std::string _filename;
		std::string _text;

	public:
		FayFile(const std::string &filename, const std::string &text)
			: _filename(filename), _text(text) {}

		const std::string &filename() { return this->_filename; }
		const std::string &text() { return this->_text; }

		//取出指定行的内容
		//处理的时候没有考虑性能，只在报错的时候再用
		const std::string line(int index);
	};

	//代码片段
	class FayPart
	{
	private:
		PTR(FayFile) _file;
		int _line;
		int _start;
		int _end;

	public:
		FayPart(PTR(FayFile) file, int line, int start, int end)
			: _file(file), _line(line), _start(start), _end(end) {}

		PTR(FayFile) file() { return this->_file; }
		int line() { return this->_line; }
		int start() { return this->_start; }
		int end() { return this->_end; }
	};

	//用于处理报错信息的异常
	class FayCompileException : public std::exception
	{
	protected:
		PTR(FayFile) _file;  //文件名称
		std::string _trace;  //堆栈
		int _line;  //行号
		int _col;  //列

	public:
		FayCompileException(const std::string &msg);
		FayCompileException(const std::string &msg, PTR(FayFile) file, int line, int col);

		const PTR(FayFile) file() { return this->_file; }
		const std::string &trace() { return this->_trace; }
		const std::string source();
		int line() { return this->_line; }
		int col() { return this->_col; }
	};

	//放向IndexMap的元素
	template<class T>
	class IndexMapItem
	{
	private:
		pos_t _index = -1;
	public:
		virtual pos_t indexValue() { return this->_index; }
		virtual const std::string &indexKey() = 0;
		friend class IndexMap<T>;
	};

	//有索引的Map结构
	template<class T>
	class IndexMap
	{
	private:
		std::vector<PTR(T)> _list;
		MAP<std::string, pos_t> _map;

	public:
		IndexMap() {}

		//数据长度
		pos_t size() { return this->_list.size(); }
		//返回Vector对象
		const std::vector<PTR(T)> &list() { return this->_list; }

		pos_t add(PTR(T) obj)
		{
			//如果没有此对象，添加进来
			auto r = this->_map.find(obj->indexKey());
			if(r == this->_map.end())
			{
				pos_t index = this->_list.size();
				this->_list.push_back(obj);
				this->_map[obj->indexKey()] = index;
				obj->_index = index;
				return index;
			}
			else
				return r->second;
		}

		inline pos_t findIndex(const std::string &key)
		{
			auto r = this->_map.find(key);
			if(r != this->_map.end())
				return r->second;

			return -1;
		}

		inline PTR(T) find(pos_t index)
		{
			if(index >= 0 && index < this->_list.size())
				return this->_list[index];

			return nullptr;
		}

		inline PTR(T) find(const std::string &key)
		{
			pos_t index = findIndex(key);
			if(index >= 0)
				return this->_list[index];

			return nullptr;
		}

		inline PTR(T) operator[](pos_t index)
		{
			return this->_list[index];
		}

		inline PTR(T) operator[](const std::string &key)
		{
			return this->_map[key];
		}
	};


}