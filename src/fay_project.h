#pragma once
#include <mutex>
#include <fay_lang.h>
#include <fay_ast.h>
#include <fay_lexer.h>

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

	template<class T>
	class BuildTaskQueue
	{
	private:
		std::mutex _lock;  //锁
		int taskCount = 0;  //任务数量
		int taskPos = -1;  //当前指向的任务
		std::vector<PTR(T)> _queue;  //任务队列

	public:
		size_t add(PTR(T) task)
		{
			std::lock_guard<std::mutex> lg(this->_lock);
			this->_queue.push_back(task);
		}

		PTR(T) get()
		{
			std::lock_guard<std::mutex> lg(this->_lock);

			return nullptr;
		}
	};
}