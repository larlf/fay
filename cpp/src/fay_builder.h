#pragma once

#include <mirror_sys_const.h>
#include <fay_lang.h>

namespace fay
{
	//用于生成整体的代码结构
	class FayBuilder
	{
	private:
		std::string _package;
		std::string _filename;
		PTR(FayDomain) _domain;
		PTR(FayLib) _lib;
		PTR(FayClass) _class;
		PTR(FayFun) _fun;

	public:
		FayBuilder(PTR(FayDomain) domain)
			: _domain(domain) {}
		~FayBuilder() {}

		PTR(FayLib) lib() { return this->_lib; }
		PTR(FayClass) clazz() { return this->_class; }
		PTR(FayFun) fun() { return this->_fun; }

		void package(const std::string &name);

		void beginFile(const std::string &filename);
		void endFile();

		void beginLib(const std::string &name);

		pos_t beginClass(const std::string &name);
		void bindClass(pos_t index);

		pos_t beginFun(const std::string &name);
		void bindFun(pos_t index);

		void addParamDefine(const std::string &name, const std::string &type);
	};
}

