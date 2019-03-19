#include "fay_project.h"
#include <fay_project.h>
#include <fay_parser.h>
#include <fay_log.h>
#include <fay_system.h>
#include <mirror.h>
#include <string>

using namespace mirror;
using namespace fay;

void fay::FayProject::step1CheckFiles()
{
	if(this->_path.empty())
	{
		LogBus::Error("Project path is empty");
		return;
	}

	if(LogBus::IsDebug())
		LogBus::Debug(TOSTR("[Files]\nProject path : " << this->_path));

	//找到所有的代码文件
	fs::path srcPath(this->_path);
	srcPath.append("src");
	std::vector<fs::path> files;
	utils::FileUtils::FindFiles(files, this->_path, true, ".fay");
	for(const auto &it : files)
	{
		std::string filename = it.string();
		LogBus::Debug(filename);
		if(this->_tasks.find(filename) == this->_tasks.end())
			this->_tasks[filename] = MKPTR(BuildTask)(filename);
	}

	//读取文件
	this->executeThreads(FayProject::ReadFileThread);
}

void fay::FayProject::step2Lexical()
{
	this->executeThreads(BIND1_S(FayProject::LexicalWorkThread));
}

void fay::FayProject::step3AST()
{
	this->executeThreads(BIND1_S(FayProject::ASTWorkThread));
}

void fay::FayProject::step4ParseClass()
{
	FayBuilder builder;
	builder.bindLib(this->_lib);

	for(auto it : this->_tasks)
	{
		auto task = it.second;
		if(!task->ast)
			continue;

		try
		{
			LogBus::Debug("[Dig1]" + task->filename);
			task->ast->dig1(&builder);
		}
		catch(fay::CompileException e)
		{
			LogBus::Error(e.what(), e.part, e.trace);
		}
		catch(std::exception e)
		{
			LogBus::Error(TOSTR("Dig1 error." << "\n" << task->filename << "\n" << e.what()));
		}
	}

	if(LogBus::IsDebug())
	{
		std::string msg = this->_lib->toString();
		LogBus::Debug(msg);
	}
}

void fay::FayProject::step5ParseFieldAndMethod()
{
	FayBuilder builder;
	builder.bindLib(this->_lib);

	for(auto it : this->_tasks)
	{
		auto task = it.second;
		if(!task->ast)
			continue;

		try
		{
			LogBus::Debug("[Dig2]" + task->filename);
			task->ast->dig2(&builder);
		}
		catch(fay::CompileException e)
		{
			LogBus::Error(e.what(), e.part, e.trace);
		}
		catch(std::exception e)
		{
			LogBus::Error(TOSTR("Dig2 error." << "\n" << task->filename << "\n" << e.what()));
		}
	}

	if(LogBus::IsDebug())
	{
		std::string msg = this->_lib->toString();
		LogBus::Debug(msg);
	}
}

void fay::FayProject::step6FixedNodeType()
{
	this->executeThreads(BIND1_S(FayProject::Dig3WorkThread, this->_lib));
}

void fay::FayProject::step7GenerateIL()
{
	this->executeThreads(BIND1_S(FayProject::Dig4WorkThread, this->_lib));

	if(LogBus::IsDebug())
	{
		LogBus::Debug("[IL]");
		std::string msg = this->_lib->toString();
		LogBus::Debug(msg);
	}
}

void fay::FayProject::WorkThread(BuildTaskQueue<BuildTask>* queue, std::function<void(PTR(BuildTask))> fun, const std::string &errorMsg)
{
	auto task = queue->get();
	while(task != nullptr)
	{
		try
		{
			fun(task);
		}
		catch(fay::CompileException e)
		{
			LogBus::Error(e.what(), e.part, e.trace);
		}
		catch(std::exception e)
		{
			LogBus::Error(TOSTR(errorMsg << "\n" << task->filename << "\n" << e.what()));
		}

		LogBus::EndThread();
		queue->complete(task);
		task = queue->get();
	}
}

void fay::FayProject::ReadFileThread(BuildTaskQueue<BuildTask>* queue)
{
	WorkThread(queue, [&](PTR(BuildTask) task)
	{
		std::string text = utils::FileUtils::ReadTextFile(task->filename);
		task->file = MKPTR(FayFile)(task->filename, text);

		//生成Debug的信息
		if(LogBus::IsDebug())
			LogBus::Debug(TOSTR("Add source file " << task->filename << ", " << text.size() << " bytes."));
	}, "Read file error.");
}

void fay::FayProject::LexicalWorkThread(BuildTaskQueue<BuildTask>* queue)
{
	Lexer lexer;  //构建词法解析器

	WorkThread(queue, [&](PTR(BuildTask) task)
	{
		if(!task->file)
			return;

		task->tokens = lexer.Execute(task->file);

		//生成Debug的信息
		if(LogBus::IsDebug())
		{
			std::string str = TOSTR("[Tokens]\n" << task->filename << "\n" << task->tokensStr());
			LogBus::Debug(str);
		}
	}, "Lexical error.");
}

void fay::FayProject::ASTWorkThread(BuildTaskQueue<BuildTask>* queue)
{
	Lexer lexer;  //构建词法解析器

	WorkThread(queue, [&](PTR(BuildTask) task)
	{
		if(!task->file || !task->tokens)
			return;

		LogBus::Debug(TOSTR("Start parse ast : " << task->filename));
		auto ast = fay::Parser::Parse(task->file, task->tokens);
		task->ast = ast;

		//生成Debug的信息
		if(LogBus::IsDebug())
		{
			std::string str = TOSTR("[AST]\n" << task->filename << "\n" << task->astStr());
			LogBus::Debug(str);
		}
	}, "AST error.");
}

void fay::FayProject::Dig3WorkThread(BuildTaskQueue<BuildTask>* queue, PTR(FayLib) lib)
{
	FayBuilder builder;
	builder.bindLib(lib);

	WorkThread(queue, [&](PTR(BuildTask) task)
	{
		if(!task->ast)
			return;

		LogBus::Debug(TOSTR("[Dig3]" << task->filename));
		task->ast->dig3(&builder);

		if(LogBus::IsDebug())
			LogBus::Debug(task->ast->toString());

	}, "Dig3 error.");
}

void fay::FayProject::Dig4WorkThread(BuildTaskQueue<BuildTask>* queue, PTR(FayLib) lib)
{
	FayBuilder builder;
	builder.bindLib(lib);

	WorkThread(queue, [&](PTR(BuildTask) task)
	{
		if(!task->ast)
			return;

		LogBus::Debug(TOSTR("[Dig4]" << task->filename));
		task->ast->dig4(&builder);

	}, "Dig4 error.");
}

void fay::FayProject::executeThreads(std::function<void(BuildTaskQueue<BuildTask>*)> fun)
{
	//创建语法解析的任务
	BuildTaskQueue<BuildTask> taskQueue;
	for(auto it : this->_tasks)
		taskQueue.add(it.second);

	//定一下要开几个线程来进行处理
	size_t threadCount = 1;
	if(!LogBus::IsDebug())
	{
		threadCount = SystemEnv::CPUS;
		if(taskQueue.waitSize() < threadCount)
			threadCount = taskQueue.waitSize();
	}

	//启动进行语法分析的线程
	for(auto i = 0; i < threadCount; ++i)
	{
		std::thread t(BIND_S(fun, &taskQueue));
		t.detach();
	}

	//等候语法分析的完成
	while(taskQueue.activeSize() > 0)
		std::this_thread::yield();
}

fay::FayProject::FayProject(const std::string &projectPath)
	: _path(projectPath)
{
}

fay::FayProject::FayProject(const std::string &name, int marjor, int minjor)
	: _name(name), _marjor(marjor), _minjor(minjor)
{
}

void fay::FayProject::build()
{
	this->_lib = MKPTR(FayLib)("default", FayDomain::AllLibs(), 0, 0);

	this->step1CheckFiles();
	this->step2Lexical();
	this->step3AST();
	this->step4ParseClass();
	this->step5ParseFieldAndMethod();
	this->step6FixedNodeType();

	if(!LogBus::Count(LogType::Error))
		this->step7GenerateIL();

	LogBus::EndAllThread();
}

PTR(BuildTask) fay::FayProject::findSource(const std::string &name)
{
	for(auto it : this->_tasks)
	{
		if(it.second->filename.find(name) != std::string::npos)
			return it.second;
	}

	return nullptr;
}

std::string fay::BuildTask::tokensStr()
{
	utils::StringBuilder sb;
	int line = 0;

	for(const auto &it : *this->tokens)
	{
		if(it->line != line)
		{
			line = it->line;
			std::string lineStr = std::to_string(line);

			if(sb.size() > 0)
				sb.add("\n");
			sb.add(' ', 5 - lineStr.size())->add(lineStr)->add(" | ");
		}

		std::string text = utils::StringUtils::EncodeSpecialChar(it->text);
		sb.add(text)->add("(")->add(TypeDict::ToName(it->type))->add(") ");
	}

	return sb.toString();
}

std::string fay::BuildTask::astStr()
{
	utils::StringBuilder sb;
	this->ast->buildString(&sb);
	return sb.toString();

}
