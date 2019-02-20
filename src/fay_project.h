#pragma once
#include <mutex>
#include <fay_lang.h>
#include <fay_ast.h>
#include <fay_lexer.h>
#include <queue>

namespace fay
{
	//源代码
	class FaySource : BaseObject
	{
	private:
		PTR(FayFile) _file;
		PTR(std::vector<PTR(Token)>) _tokens;
		PTR(AstNode) _ast;

	public:
		FaySource(PTR(FayFile) file) : _file(file) {}

		const std::string filename() { return this->_file->filename(); }
		PTR(std::vector<PTR(Token)>) tokens() { return this->_tokens; }
		PTR(AstNode) ast() { return this->_ast; }

		//对内容进行解析
		void parse(PTR(Lexer) lexer);

		//取得Token的内容用于显示
		std::string tokensStr();
		//取得用于显示AST的内容
		std::string astStr();
	};

	//项目
	class FayProject : BaseObject
	{
	private:
		std::string _path;  //项目所在的目录
		std::string _name;
		int _marjor = 0;
		int _minjor = 0;

		MAP<std::string, PTR(FaySource)> _files;
		PTR(FayBuilder) _builder;

		//检查所有需要处理的文件
		void checkAllFiles();

	public:
		FayProject(const std::string &projectPath);
		FayProject(const std::string &name, int marjor, int minjor);

		//void addFiles(std::vector<std::string> &files);
		//void parse();
		void build();

		PTR(FaySource) findSource(const std::string &name);
	};

	class BuildTask
	{
	public:
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

			if (task != nullptr)
			{
				this->_waitQueue.push(task);
				this->_taskCount++;
			}
		}

		//取得任务
		PTR(T) get()
		{
			std::lock_guard<std::mutex> lg(this->_lock);

			if (this->_waitQueue.size() > 0)
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

			for (auto i = 0; i < this->_doingTasks.size(); ++i)
			{
				if (this->_doingTasks[i] != nullptr && this->_doingTasks[i] == task)
				{
					this->_completedTasks.push_back(task);
					this->_doingTasks[i] = nullptr;
					this->_taskCount--;
				}
			}
		}
	};
}