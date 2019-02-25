﻿#include <fay_project.h>
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
			this->_files[filename] = MKPTR(FaySource)(file);

			if(LogBus::IsDebug())
				LogBus::Debug(TOSTR("Add source file " << filename << ", " << text.size() << " bytes."));
		}
	}
}

void fay::FayProject::step2Lexical()
{
	//创建诃法解析的任务
	BuildTaskQueue<FaySource> lexicalQueue;
	for(auto it : this->_files)
		lexicalQueue.add(it.second);

	//定一下要开几个线程来进行处理
	int threadCount = 1;
	while(threadCount < SystemEnv::CPUS && lexicalQueue.activeSize() > threadCount * 5)
		threadCount++;

	//启动进行诃法分析的线程
	for(auto i = 0; i < threadCount; ++i)
	{
		std::thread t(BIND_S(FayProject::lexicalWorkThread, &lexicalQueue));
		t.detach();
	}

	//等候词法分析的完成
	while(lexicalQueue.activeSize() > 0)
		std::this_thread::yield();
}

void fay::FayProject::step3AST()
{
	//创建语法解析的任务
	BuildTaskQueue<FaySource> astQueue;
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
		std::thread t(BIND_S(FayProject::astWorkThread, &astQueue));
		t.detach();
	}

	//等候语法分析的完成
	while(astQueue.activeSize() > 0)
		std::this_thread::yield();
}

void fay::FayProject::step4ParseClass()
{
	FayBuilder builder;
	builder.beginLib("default");

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
			catch(BuildException e)
			{
				std::string msg = e.what();
				msg += I18N::Get("location", src->filename(), std::to_string(e.ast->token()->line()), std::to_string(e.ast->token()->col()));
				LogBus::Error(msg, MKPTR(FilePart)(src->file, e.ast->token()->line(), e.ast->token()->col(), e.ast->token()->size()));
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
}

void fay::FayProject::step5ParseFieldAndMethod()
{
}

void fay::FayProject::lexicalWorkThread(BuildTaskQueue<FaySource>* queue)
{
	Lexer lexer;  //构建词法解析器

	auto task = queue->get();
	while(task != nullptr)
	{
		try
		{
			task->tokens = lexer.Execute(task->text());

			//生成Debug的信息
			if(LogBus::IsDebug())
			{
				std::string str = TOSTR("[Tokens]\n" << task->filename() << "\n" << task->tokensStr());
				LogBus::Debug(str);
			}
		}
		catch(LexerException e)
		{
			std::string msg = e.what();
			msg += I18N::Get("location", task->filename(), std::to_string(e.line), std::to_string(e.col));
			LogBus::Error(msg, MKPTR(FilePart)(task->file, e.line, e.col, 0));
		}
		catch(std::exception e)
		{
			std::string msg = I18N::Get("err.lexical");
			msg += task->filename();
			msg += "\n";
			msg += e.what();
			LogBus::Error(msg);
		}

		LogBus::EndThread();
		queue->complete(task);
		task = queue->get();
	}
}

void fay::FayProject::astWorkThread(BuildTaskQueue<FaySource>* queue)
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
		catch(ParseException e)
		{
			std::string msg = e.what();
			msg += I18N::Get("location", task->filename(), std::to_string(e.token->line()), std::to_string(e.token->col()));

			std::lock_guard<std::recursive_mutex> lg(LogBus::LogLock);
			LogBus::Error(msg, MKPTR(FilePart)(task->file, e.token->line(), e.token->col(), e.token->size()));
		}
		catch(std::exception e)
		{
			std::string msg = I18N::Get("err.ast");
			msg += task->filename();
			msg += "\n";
			msg += e.what();
			LogBus::Error(msg);
		}

		LogBus::EndThread();
		queue->complete(task);
		task = queue->get();
	}
}

fay::FayProject::FayProject(const std::string &projectPath)
	: _path(projectPath)
{
	_builder = MKPTR(FayBuilder)();
	_builder->beginLib("default");
}

fay::FayProject::FayProject(const std::string &name, int marjor, int minjor)
	: _name(name), _marjor(marjor), _minjor(minjor)
{
	//_domain = MKPTR(fay::FayDomain)()->init();

	_builder = MKPTR(FayBuilder)();
	_builder->beginLib(name);
}

void fay::FayProject::build()
{
	this->step1CheckFiles();

	{
		PTR(Lexer) lexer = MKPTR(Lexer)();

		for(auto it : this->_files)
		{
			try
			{
				//it.second->parse(lexer);
			}
			catch(FayCompileException &e)
			{
				LOG_ERROR(e.what());
				PRINT(e.source());
				PRINT(e.trace());
			}
		}
	}

	for(auto it : this->_files)
	{
		try
		{
			it.second->ast->dig1(this->_builder.get());
		}
		catch(FayCompileException &e)
		{
			LOG_ERROR(e.what());
			PRINT(e.source());
			PRINT(e.trace());
			return;
		}
	}

	for(auto it : this->_files)
	{
		try
		{
			it.second->ast->dig2(this->_builder.get());
		}
		catch(FayCompileException &e)
		{
			LOG_ERROR(e.what());
			PRINT(e.source());
			PRINT(e.trace());
			return;
		}
	}

	for(auto clazz : FayDomain::classes().list())
		clazz->rebuild();

	for(auto it : this->_files)
	{
		try
		{
			it.second->ast->dig3(this->_builder.get());
		}
		catch(FayCompileException &e)
		{
			LOG_ERROR(e.what());
			PRINT(e.source());
			PRINT(e.trace());
			return;
		}
	}

	for(auto it : this->_files)
	{
		try
		{
			it.second->ast->dig4(this->_builder.get());
		}
		catch(FayCompileException &e)
		{
			LOG_ERROR(e.what());
			PRINT(e.source());
			PRINT(e.trace());
			return;
		}
	}
}

void fay::FayProject::build2()
{
	this->step1CheckFiles();
	this->step2Lexical();
	this->step3AST();
	this->step4ParseClass();
	this->step5ParseFieldAndMethod();

	LogBus::Clear();
}

PTR(FaySource) fay::FayProject::findSource(const std::string &name)
{
	for(auto it : this->_files)
	{
		if(it.second->filename().find(name) != std::string::npos)
			return it.second;
	}

	return nullptr;
}

std::string fay::FaySource::tokensStr()
{
	utils::StringBuilder sb;
	int line = 0;

	for(auto it : *this->tokens)
	{
		if(it->line() != line)
		{
			line = it->line();
			std::string lineStr = std::to_string(line);

			if(sb.size() > 0)
				sb.add("\n");
			sb.add(' ', 5 - lineStr.size())->add(lineStr)->add(" | ");
		}

		std::string text = utils::StringUtils::EncodeSpecialChar(it->text());
		sb.add(text)->add("(")->add(TypeDict::ToName(it->type()))->add(") ");
	}

	return sb.toString();
}

std::string fay::FaySource::astStr()
{
	utils::StringBuilder sb;
	this->ast->buildString(&sb);
	return sb.toString();

}
