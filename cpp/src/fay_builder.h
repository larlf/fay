#pragma once

#include <mirror_sys_const.h>
#include <fay_lang.h>

namespace fay
{
	//用于生成整体的代码结构
	class OutlineBuilder
	{
	private:
		std::string _package;
		std::string _filename;
		PTR(FayLib) _lib;
		PTR(FayClass) _class;
		PTR(FayFun) _fun;

	public:
		OutlineBuilder(PTR(FayLib) lib);
		~OutlineBuilder();

		PTR(FayLib) lib() { return this->_lib; }
		PTR(FayClass) clazz() { return this->_class; }
		PTR(FayFun) fun() { return this->_fun; }

		void package(const std::string &name);

		void beginFile(const std::string &filename);
		void endFile();

		void beginClass(const std::string &name);
		void endClass();

		void beginFun(const std::string &name, PTR(FayCode) code);
		void endFun();
	};

	//用于生成中间代码
	class InstBuilder
	{
	private:
		std::vector<PTR(FayInst)> _insts;

	public:
		void addInst(PTR(FayInst) inst);
		std::vector<PTR(FayInst)> insts();
	};
}

