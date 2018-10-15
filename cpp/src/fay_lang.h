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
	class FayParamDef;

	//当前总体管理类
	class FayDomain : public FayObject, public std::enable_shared_from_this<FayDomain>
	{
	private:
		std::vector<PTR(FayLib)> _libs;
		IndexMap<PTR(FayType)> _types;

	public:
		FayDomain();
		//添加Lib
		void addLib(PTR(FayLib) lib);
		//添加新的类型
		//返回类型在Domain里的序号
		pos_t addType(PTR(FayType) t);
		//根据类型的全称查找类型定义
		PTR(FayType) findType(const std::string &typeFullname);
		PTR(FayType) findType(pos_t index);
		//根据引用和类型名，查找类型的定义
		std::vector<PTR(FayType)> findType(std::vector<std::string> &imports, const std::string &typeName);
		//从函数表中查找指定的函数
		PTR(FayFun) findFun(const std::string &className, const std::string &funName, std::vector<PTR(FayType)> paramsType);

		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	//函数信息
	class OutsideFun
	{
	public:
		pos_t index;
		std::string funFullname;
		WPTR(FayFun) fun;
	};

	//一个的信息
	class FayLib : public FayObject, public std::enable_shared_from_this<FayLib>
	{
	private:
		IndexMap<PTR(OutsideFun)> _outsideFuns;  //外部函数的列表

	public:
		std::string name;
		WPTR(FayDomain) domain;
		std::vector<PTR(FayClass)> classes;

		FayLib(const std::string &name)
			:  name(name) {}
		~FayLib() {}

		pos_t addClass(PTR(FayClass) clazz);
		PTR(FayFun) findFun(const std::string &className, const std::string &funName, std::vector<PTR(FayType)> paramsType);

		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	class FayClass : public FayType, public std::enable_shared_from_this<FayClass>
	{
	private:
		std::string _fullname;  //全名
		IndexMap<WPTR(FayFun)> _funsMap;  //函数的映射

	public:
		WPTR(FayDomain) domain; //所在的Domain
		WPTR(FayLib) lib;  //所属的Lib
		std::string package;  //所在的名
		std::string name;  //类的名称
		std::vector<PTR(FayFun)> funs;  //类里的函数列表

		FayClass(const std::string &package, const std::string &name)
			: package(package), name(name)
		{
			this->_fullname = (package.size() > 0 ? package + "." : "") + name;
		}

		void addFun(PTR(FayFun) fun);

		PTR(FayFun) findFun(const std::string &funName, std::vector<PTR(FayType)> paramsType);

		virtual const std::string &fullname() override;
		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	//函数
	class FayFun : public FayObject, public std::enable_shared_from_this<FayFun>
	{
	private:
		std::string _name;
		std::string _fullname;
		std::vector<PTR(FayParamDef)> _params;

	public:
		WPTR(FayType) clazz;  //所属的类

		FayFun(PTR(FayType) clazz, const std::string &name)
			: clazz(clazz), _name(name) {}
		~FayFun();

		const std::string &name() { return this->_name; }
		const std::string &fullname();

		void addParam(PTR(FayParamDef) p1);

		bool matchParams(std::vector<PTR(FayType)> paramsType);

		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	//参数定义
	class FayParamDef : public FayObject, public std::enable_shared_from_this<FayParamDef>
	{
	private:
		std::string _name;
		std::string _fullname;
		std::string _typeName;

	public:
		FayParamDef(const std::string &name, std::string typeName)
			: _name(name), _typeName(typeName) {}

		const std::string &name() { return this->_name; }

		const std::string &fullname();
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

