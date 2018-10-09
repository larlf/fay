#pragma once

#include <mirror_sys_const.h>

namespace fay
{
	class FayLib;
	class FayClass;
	class FayFun;

	class FayDomain : std::enable_shared_from_this<FayDomain>
	{
	private:
		std::vector<PTR(FayLib)> _libs;

	public:

	};

	class FayLib : std::enable_shared_from_this<FayLib>
	{
	private:
		std::vector<PTR(FayClass)> _classes;

	public:
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
	};

	class FayFun : std::enable_shared_from_this<FayFun>
	{
	public:
	};
}

