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

	//数据类型
	class FayType : public FayObject
	{
	protected:
		IndexMap<PTR(FayFun)> _funs;  //函数的映射

	public:
		//是否匹配
		//当传入的类型和自己一样或是此类型的子类的时候为True
		virtual bool match(PTR(FayType) type);
		//查找符合要求的
		std::vector<PTR(FayFun)> findFun(const std::string &funName, const std::vector<PTR(FayType)> &paramsType);
		PTR(FayFun) findFun(pos_t index);
	};

	//简单类型
	class SimpleType : public FayType
	{
	private:
		static MAP<ValueType, PTR(FayType)> _Types;

		ValueType _type;

		//这种类型，不能从外部创建
		SimpleType(ValueType valueType)
			: _type(valueType) {}

	public:
		static PTR(FayType) Get(ValueType valueType);

		// 通过 FayType 继承
		virtual const std::string &fullname() override;
	};

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
		std::vector<PTR(FayFun)> findFun(const std::string &className, const std::string &funName, const std::vector<PTR(FayType)> &paramsType);

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
		//外部函数的列表
		//这个表的主要用处，是把本Lib中的函数调用转换成索引值
		IndexMap<PTR(OutsideFun)> _outsideFuns;

	public:
		std::string name;
		WPTR(FayDomain) domain;
		std::vector<PTR(FayClass)> classes;

		FayLib(const std::string &name)
			:  name(name) {}
		~FayLib() {}

		pos_t addClass(PTR(FayClass) clazz);
		//返回调用方法在外部函数表中的索引
		pos_t findOutsideFun(const std::string &className, const std::string &funName, const std::vector<PTR(FayType)> &paramsType);

		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	class FayClass : public FayType, public std::enable_shared_from_this<FayClass>
	{
	private:
		std::string _fullname;  //全名
		

	public:
		WPTR(FayDomain) domain; //所在的Domain
		WPTR(FayLib) lib;  //所属的Lib

		std::string package;  //所在的名
		std::string name;  //类的名称

		FayClass(const std::string &package, const std::string &name)
			: package(package), name(name)
		{
			this->_fullname = (package.size() > 0 ? package + "." : "") + name;
		}

		pos_t addFun(PTR(FayFun) fun);


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
		WPTR(FayDomain) domain;
		WPTR(FayType) clazz;  //所属的类
		std::vector<FayInst*> insts;  //代码，注意这里考虑到性能，没用智能指针，因此所有地方也不存对指令的引用

		FayFun(const std::string &name)
			: _name(name) {}
		virtual ~FayFun();

		const std::string &name() { return this->_name; }

		void addParam(PTR(FayParamDef) def);
		bool matchParams(const std::vector<PTR(FayType)> &paramsType);

		virtual const std::string &fullname() override;
		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	//参数定义
	class FayParamDef : public FayObject, public std::enable_shared_from_this<FayParamDef>
	{
	private:
		std::string _fullname;

	public:
		std::string name;
		WPTR(FayType) type;

		FayParamDef(const std::string &name, PTR(FayType) type)
			: name(name), type(type) {}

		virtual const std::string &fullname() override;
		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	/////////////////////////////////////////////////

	class FaySystemClass : public FayClass
	{
	public:
		FaySystemClass();
		void init();
	};

	class FaySystemLib : public FayLib
	{
	public:
		FaySystemLib();
		void init();
	};

}

