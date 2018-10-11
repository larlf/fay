#pragma once

#include <mirror_sys_const.h>
#include <fay_type.h>
#include <fay_inst.h>
#include <fay_const.h>
#include <fay_object.h>

namespace fay
{
	class FayLib;
	class FayClass;
	class FayFun;
	class FayCode;

	//当前总体管理类
	class FayDomain : public FayObject, public std::enable_shared_from_this<FayDomain>
	{
	private:
		std::vector<PTR(FayLib)> _libs;

	public:
		void addLib(PTR(FayLib) lib);
		virtual void toString(mirror::utils::StringBuilder* sb) override;
		PTR(FayFun) findFun(const std::string &className, const std::string &funName, std::vector<PTR(FayType)> paramsType);
	};

	//函数信息
	class OutsideFun
	{
	public:
		pos_t index;
		std::string funFullname;
		pos_t classPos;
		pos_t funPos;
	};

	//一个的信息
	class FayLib : public FayObject, public std::enable_shared_from_this<FayLib>
	{
	public:
		std::string name;
		std::vector<PTR(FayClass)> classes;
		WPTR(FayDomain) domain;

		FayLib(const std::string &name);
		~FayLib();

		void addClass(PTR(FayClass) clazz);
		PTR(FayFun) findFun(const std::string &className, const std::string &funName, std::vector<PTR(FayType)> paramsType);

		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	class FayClass : public FayType, public std::enable_shared_from_this<FayClass>
	{
	public:
		std::string name;  //类的名称
		std::vector<PTR(FayFun)> funs;  //类里的函数列表

		FayClass(const std::string &name) : name(name) {}

		void addFun(PTR(FayFun) fun);

		PTR(FayFun) findFun(const std::string &funName, std::vector<PTR(FayType)> paramsType);

		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	class FayParamDef : public FayObject, public std::enable_shared_from_this<FayParamDef>
	{
	private:
		std::string _name;
		PTR(FayType) _type;

	public:
		FayParamDef(const std::string &name, PTR(FayType) type)
			: _name(name), _type(type) {}

		const std::string &name() { return this->_name; }
		PTR(FayType) type() { return this->_type; }

		virtual void toString(mirror::utils::StringBuilder* sb) override;

	};

	class FayFun : public FayObject, public std::enable_shared_from_this<FayFun>
	{
	private:
		std::string _name;
		PTR(FayCode) _code;
		std::vector<PTR(FayParamDef)> _params;

	public:
		FayFun(const std::string &name, PTR(FayCode) code)
			: _name(name), _code(code) {}
		~FayFun();

		const std::string &name() { return this->_name; }
		const std::string &fullname();

		void addParam(PTR(FayParamDef) p1); 

		bool matchParams(std::vector<PTR(FayType)> paramsType);

		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	//代码生成器的父类
	class FayCode : public FayObject, public std::enable_shared_from_this<FayCode>
	{
	public:
		virtual std::vector<PTR(FayInst)> insts() = 0;
	};

	class FayScope : public FayObject, public std::enable_shared_from_this<FayScope>
	{

	};
}

