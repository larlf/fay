#pragma once

#include <mirror_sys_const.h>
#include <fay_inst.h>

namespace fay
{
	class FayLib;
	class FayClass;
	class FayFun;
	class FayCode;

	class FayDomain : std::enable_shared_from_this<FayDomain>
	{
	private:
		std::vector<PTR(FayLib)> _libs;

	public:

	};

	class FayLib : std::enable_shared_from_this<FayLib>
	{
	private:
		std::string _name;
		std::vector<PTR(FayClass)> _classes;

	public:
		FayLib(const std::string &name);
		~FayLib();

		const std::string &name() { return this->_name; }

		void addClass(PTR(FayClass) clazz);
	};

	class FayClass : std::enable_shared_from_this<FayClass>
	{
	private:
		std::string _name;
		std::vector<PTR(FayFun)> _funs;

	public:
		FayClass(const std::string &name) : _name(name) {}
		const std::string &name() { return this->_name; }

		void addFun(PTR(FayFun) fun);
	};

	class FayFun : std::enable_shared_from_this<FayFun>
	{
	private:
		std::string _name;
		PTR(FayCode) _code;

	public:
		FayFun(const std::string &name, PTR(FayCode) code)
			: _name(name), _code(code) {}
		~FayFun();

		const std::string &name() { return this->_name; }
	};

	//代码生成器的父类
	class FayCode : std::enable_shared_from_this<FayCode>
	{
	public:
		virtual std::vector<PTR(FayInst)> insts() = 0;
	};

	class FayScope : std::enable_shared_from_this<FayScope>
	{
		
	};
}

