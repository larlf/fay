#include "fay_project.h"
#include <fay_project.h>
#include <fay_parser.h>
#include <mirror_utils_file.h>

using namespace mirror;

fay::FayProject::FayProject(const std::string & name, int marjor, int minjor)
	: _name(name), _marjor(marjor), _minjor(minjor)
{
	_domain = MKPTR(fay::FayDomain)();
	_domain->initSysLib();

	_builder = MKPTR(FayBuilder)(_domain);
	_builder->beginLib(name);
}

void fay::FayProject::addFiles(std::vector<std::string> &files)
{
	for(auto filename : files)
	{
		if(this->_files.find(filename) == this->_files.end())
		{
			std::string text = utils::FileUtils::ReadTextFile(filename);
			PTR(FayFile) file = MKPTR(FayFile)(filename, text);
			this->_files[filename] = MKPTR(FaySource)(file);
		}
	}
}

void fay::FayProject::parse()
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
		}
	}
}

void fay::FayProject::build()
{
	for (auto it : this->_files)
	{
		try
		{
			it.second->ast()->dig1(this->_builder.get());
		}
		catch (FayCompileException &e)
		{
			LOG_ERROR(e.what());
		}
	}

	for (auto it : this->_files)
	{
		try
		{
			it.second->ast()->dig2(this->_builder.get());
		}
		catch (FayCompileException &e)
		{
			LOG_ERROR(e.what());
		}
	}

	for (auto it : this->_files)
	{
		try
		{
			it.second->ast()->dig3(this->_builder.get());
		}
		catch (FayCompileException &e)
		{
			LOG_ERROR(e.what());
		}
	}

	for (auto it : this->_files)
	{
		try
		{
			it.second->ast()->dig4(this->_builder.get());
		}
		catch (FayCompileException &e)
		{
			LOG_ERROR(e.what());
		}
	}
}

void fay::FaySource::parse(PTR(Lexer) lexer)
{
	this->_tokens = lexer->Execute(this->_file);
	this->_ast = fay::Parser::Parse(this->_tokens, this->_file->filename());
}
