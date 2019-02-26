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
	if(this->_path.size() < 1)
	{
		LogBus::Error("Project path is empty");
		return;
	}

	if(LogBus::IsDebug())
		LogBus::Debug(TOSTR("[Files]\nProject path : " << this->_path));

	//找到所有的代码文件并读取进来
	fs::path srcPath(this->_path);
	srcPath.append("src");
	std::vector<fs::path> files;
	utils::FileUtils::FindFiles(files, this->_path, true, ".fay");
	for(auto it : files)
	{
		std::string filename = it.string();
		LogBus::Debug(filename);
		if(this->_files.find(filename) == this->_files.end())
		{
			std::string text = utils::FileUtils::ReadTextFile(filename);
			PTR(FayFile) file = MKPTR(FayFile)(filename, text);
			this->_files[filename] = MKPTR(BuildTask)(file);

			if(LogBus::IsDebug())
				LogBus::Debug(TOSTR("Add source file " << filename << ", " << text.size() << " bytes."));
		}
	}
}

void fay::FayProject::step2Lexical()
{
	this->executeThreads(BIND1_S(FayProject::LexicalWorkThread));
}

void fay::FayProject::step3AST()
{
	//创建语法解析的任务
	BuildTaskQueue<BuildTask> astQueue;
	for(auto it : this->_files)
	{
		if(it.second->tokens != nullptr)
			astQueue.add(it.second);
	}

	//定一下要开几个线程来进行处理
	int threadCount = 1;
	while(threadCount < SystemEnv::CPUS && astQueue.activeSize() > threadCount * 5)
		threadCount++;

	//启动进行语法分析的线程
	for(auto i = 0; i < threadCount; ++i)
	{
		std::thread t(BIND_S(FayProject::ASTWorkThread, &astQueue));
		t.detach();
	}

	//等候语法分析的完成
	while(astQueue.activeSize() > 0)
		std::this_thread::yield();
}

void fay::FayProject::step4ParseClass()
{
	FayBuilder builder;
	builder.bindLib(this->_lib);

	for(auto it : this->_files)
	{
		auto src = it.second;
		if(src->ast != nullptr)
		{
			try
			{
				LogBus::Debug("[Dig1]" + src->filename());
				src->ast->dig1(&builder);
			}
			catch(fay::CompileException e)
			{
				LogBus::Error(e.what(), e.part);
			}
			catch(std::exception e)
			{
				std::string msg = I18N::Get("err.builder");
				msg += src->filename();
				msg += "\n";
				msg += e.what();
				LogBus::Error(msg);
			}
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

	for(auto it : this->_files)
	{
		auto src = it.second;
		if(src->ast != nullptr)
		{
			try
			{
				LogBus::Debug("[Dig2]" + src->filename());
				src->ast->dig2(&builder);
			}
			catch(fay::CompileException e)
			{
				LogBus::Error(e.what(), e.part);
			}
			catch(std::exception e)
			{
				std::string msg = I18N::Get("err.builder");
				msg += src->filename();
				msg += "\n";
				msg += e.what();
				LogBus::Error(msg);
			}
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
	//创建语法解析的任务
	BuildTaskQueue<BuildTask> taskQueue;
	for(auto it : this->_files)
	{
		if(it.second->ast != nullptr)
			taskQueue.add(it.second);
	}

	//定一下要开几个线程来进行处理
	int threadCount = 1;
	while(threadCount < SystemEnv::CPUS && taskQueue.activeSize() > threadCount * 5)
		threadCount++;

	//启动进行语法分析的线程
	for(auto i = 0; i < threadCount; ++i)
	{
		std::thread t(BIND_S(FayProject::Dig3WorkThread, &taskQueue, this->_lib));
		t.detach();
	}

	//等候语法分析的完成
	while(taskQueue.activeSize() > 0)
		std::this_thread::yield();
}

void fay::FayProject::step7GenerateIL()
{
	//创建语法解析的任务
	BuildTaskQueue<BuildTask> taskQueue;
	for(auto it : this->_files)
	{
		if(it.second->ast != nullptr)
			taskQueue.add(it.second);
	}

	//定一下要开几个线程来进行处理
	int threadCount = 1;
	while(threadCount < SystemEnv::CPUS && taskQueue.activeSize() > threadCount * 5)
		threadCount++;

	//启动进行语法分析的线程
	for(auto i = 0; i < threadCount; ++i)
	{
		std::thread t(BIND_S(FayProject::Dig4WorkThread, &taskQueue, this->_lib));
		t.detach();
	}

	//等候语法分析的完成
	while(taskQueue.activeSize() > 0)
		std::this_thread::yield();
}

void fay::FayProject::LexicalWorkThread(BuildTaskQueue<BuildTask>* queue)
{
	Lexer lexer;  //构建词法解析器

	auto task = queue->get();
	while(task != nullptr)
	{
		try
		{
			task->tokens = lexer.Execute(task->file);

			//生成Debug的信息
			if(LogBus::IsDebug())
			{
				std::string str = TOSTR("[Tokens]\n" << task->filename() << "\n" << task->tokensStr());
				LogBus::Debug(str);
			}
		}
		catch(fay::CompileException e)
		{
			LogBus::Error(e.what(), e.part);
		}
		catch(std::exception e)
		{
			LogBus::Error(e.what());
		}

		LogBus::EndThread();
		queue->complete(task);
		task = queue->get();
	}
}

void fay::FayProject::ASTWorkThread(BuildTaskQueue<BuildTask>* queue)
{
	auto task = queue->get();
	while(task != nullptr)
	{
		try
		{
			LogBus::Debug(TOSTR("Start parse ast : " << task->filename()));
			auto ast = fay::Parser::Parse(task->file, task->tokens);
			task->ast = ast;

			//生成Debug的信息
			if(LogBus::IsDebug())
			{
				std::string str = TOSTR("[AST]\n" << task->filename() << "\n" << task->astStr());
				LogBus::Debug(str);
			}
		}
		catch(fay::CompileException e)
		{
			LogBus::Error(e.what(), e.part);
		}
		catch(std::exception e)
		{
			LogBus::Error(e.what());
		}

		LogBus::EndThread();
		queue->complete(task);
		task = queue->get();
	}
}

void fay::FayProject::Dig3WorkThread(BuildTaskQueue<BuildTask>* queue, PTR(FayLib) lib)
{
	FayBuilder builder;
	builder.bindLib(lib);

	auto task = queue->get();
	while(task != nullptr)
	{
		if(task->ast != nullptr)
		{
			try
			{
				LogBus::Debug(TOSTR("[Dig3]" << task->filename()));
				task->ast->dig3(&builder);
			}
			catch(fay::CompileException e)
			{
				LogBus::Error(e.what(), e.part);
			}
			catch(std::exception e)
			{
				LogBus::Error(e.what());
			}

			LogBus::EndThread();
			queue->complete(task);
			task = queue->get();
		}
	}
}

void fay::FayProject::Dig4WorkThread(BuildTaskQueue<BuildTask>* queue, PTR(FayLib) lib)
{
	FayBuilder builder;
	builder.bindLib(lib);

	auto task = queue->get();
	while(task != nullptr)
	{
		if(task->ast != nullptr)
		{
			try
			{
				LogBus::Debug(TOSTR("[Dig4]" << task->filename()));
				task->ast->dig4(&builder);
			}
			catch(fay::CompileException e)
			{
				LogBus::Error(e.what(), e.part);
			}
			catch(std::exception e)
			{
				LogBus::Error(e.what());
			}

			LogBus::EndThread();
			queue->complete(task);
			task = queue->get();
		}
	}
}

void fay::FayProject::executeThreads(std::function<void(BuildTaskQueue<BuildTask>*)> fun)
{
	//创建语法解析的任务
	BuildTaskQueue<BuildTask> taskQueue;
	for(auto it : this->_files)
		taskQueue.add(it.second);

	//定一下要开几个线程来进行处理
	int threadCount = 1;
	while(threadCount < SystemEnv::CPUS && taskQueue.activeSize() > threadCount * 5)
		threadCount++;

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
	this->_lib = MKPTR(FayLib)("default", 0, 0);

	this->step1CheckFiles();
	this->step2Lexical();
	this->step3AST();
	this->step4ParseClass();
	this->step5ParseFieldAndMethod();
	this->step6FixedNodeType();
	this->step7GenerateIL();

	LogBus::Clear();
}

PTR(BuildTask) fay::FayProject::findSource(const std::string &name)
{
	for(auto it : this->_files)
	{
		if(it.second->filename().find(name) != std::string::npos)
			return it.second;
	}

	return nullptr;
}

std::string fay::BuildTask::tokensStr()
{
	utils::StringBuilder sb;
	int line = 0;

	for(auto it : *this->tokens)
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
