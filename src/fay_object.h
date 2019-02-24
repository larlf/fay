#pragma once
#include <mirror.h>
#include <fay_const.h>
#include <string>
#include <exception>
#include <thread>
#include <queue>
#include <mutex>

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
	public:
		std::string filename;  //文件的名称
		std::string text;  //文件的内容

		FayFile(const std::string &filename, const std::string &text)
			: filename(filename), text(text) {}
	};

	//代码片段
	class FilePart
	{
	public:
		PTR(FayFile) file;
		int line;
		int start;
		int len;

		FilePart(PTR(FayFile) file, int line, int start, int len)
			: file(file), line(line), start(start), len(len) {}
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

	//编译时的任务队列
	//因为要给多个工作线程使用，这是里的方法都是线程安全的
	template<class T>
	class BuildTaskQueue
	{
	private:
		std::mutex _lock;  //锁
		size_t _taskCount = 0;  //任务数量，这个是没有完成的任务数量
		std::queue<PTR(T)> _waitQueue;  //待处理任务队列
		std::vector<PTR(T)> _doingTasks;  //待完成任务队列
		std::vector<PTR(T)> _completedTasks;  //待完成和已完成任务队列

	public:
		//等待处理的任务数量
		size_t waitSize() { return this->_waitQueue.size(); }
		//没有处理完成的任务数量
		size_t activeSize() { return this->_taskCount; }
		//完成的任务列表
		std::vector<PTR(T)> completedTasks() { return this->_completedTasks; }

		//添加任务
		void add(PTR(T) task)
		{
			std::lock_guard<std::mutex> lg(this->_lock);

			if(task != nullptr)
			{
				this->_waitQueue.push(task);
				this->_taskCount++;
			}
		}

		//取得任务
		PTR(T) get()
		{
			std::lock_guard<std::mutex> lg(this->_lock);

			if(this->_waitQueue.size() > 0)
			{
				PTR(T) task = this->_waitQueue.front();
				this->_waitQueue.pop();
				this->_doingTasks.push_back(task);
				return task;
			}

			return nullptr;
		}

		//完成并交还任务
		void complete(PTR(T) task)
		{
			std::lock_guard<std::mutex> lg(this->_lock);

			for(auto i = 0; i < this->_doingTasks.size(); ++i)
			{
				if(this->_doingTasks[i] != nullptr && this->_doingTasks[i] == task)
				{
					this->_completedTasks.push_back(task);
					this->_doingTasks[i] = nullptr;
					this->_taskCount--;
				}
			}
		}
	};

}