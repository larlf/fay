#include "fay_project.h"
#include <fay_parser.h>
#include <mirror.h>

using namespace mirror;
using namespace fay;

void fay::FayProject::checkAllFiles()
{
	if(this->_path.size() < 1)
	{
		LOG_ERROR("Unknow project path");
		return;
	}

	//找到所有的代码文件并读取进来
	fs::path srcPath(this->_path);
	srcPath.append("src");
	std::vector<fs::path> files;
	utils::FileUtils::FindFiles(files, this->_path, true, ".fay");
	for(auto it : files)
	{
		std::string filename = it.string();
		if(this->_files.find(filename) == this->_files.end())
		{
			std::string text = utils::FileUtils::ReadTextFile(filename);
			PTR(FayFile) file = MKPTR(FayFile)(filename, text);
			this->_files[filename] = MKPTR(FaySource)(file);
		}
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
	this->checkAllFiles();

	{
		PTR(Lexer) lexer = MKPTR(Lexer)();

		for(auto it : this->_files)
		{
			try
			{
				it.second->parse(lexer);
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
			it.second->ast()->dig1(this->_builder.get());
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
			it.second->ast()->dig2(this->_builder.get());
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
			it.second->ast()->dig3(this->_builder.get());
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
			it.second->ast()->dig4(this->_builder.get());
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

PTR(FaySource) fay::FayProject::findSource(const std::string &name)
{
	for(auto it : this->_files)
	{
		if(it.second->filename().find(name) != std::string::npos)
			return it.second;
	}

	return nullptr;
}

void fay::FaySource::parse(PTR(Lexer) lexer)
{
	this->_tokens = lexer->Execute(this->_file);
	this->_ast = fay::Parser::Parse(this->_tokens, this->_file->filename());
}

std::string fay::FaySource::tokensStr()
{
	utils::StringBuilder sb;

	for(auto it : *this->_tokens)
	{
		if(sb.size() > 0)
			sb.add("\n");

		sb.add(it->toString());
	}

	return sb.toString();
}

std::string fay::FaySource::astStr()
{
	utils::StringBuilder sb;
	this->_ast->buildString(&sb);
	return sb.toString();

}
