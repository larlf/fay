#pragma once

#include <mirror_sys_const.h>
#include <fay_type.h>
#include <fay_inst.h>
#include <fay_const.h>
#include <fay_object.h>
#include <fay_stack.h>

namespace fay
{
	class FayLib;
	class FayClass;
	class FayFun;
	class FayCode;
	class FayParamDef;
	class FayDomain;
	class FayType;

	//工具类
	class FayLangUtils
	{
	public:
		//用于生成名称的方法
		static std::string Fullname(const std::string &funName, const std::vector<PTR(FayType)> &params);
		static std::string Fullname(const std::string &className, const std::string &funName, const std::vector<PTR(FayType)> &params);
	};

	//////////////////////////////////////////////////////////////

	//Fay语言中所有运行时对象的父类
	//此类对象有两个特点：
	//一、对Domain有引用，以方便进行各种操作
	//二、有fullname()方法，可以取出全名进行各处理以string为key的处理
	class FayLangObject : public FayObject
	{
	private:
		WPTR(FayDomain) _domain;
	public:
		FayLangObject(PTR(FayDomain) domain)
			: _domain(domain) {}

		PTR(FayDomain) domain() { return this->_domain.lock(); }

		//全名
		virtual const std::string &fullname() { return mirror::utils::StringUtils::Blank; }
	};

	////////////////////////////////////////////////////////////////

	//数据类型
	class FayType : public FayLangObject
	{
		using FayLangObject::FayLangObject;

	protected:
		IndexMap<PTR(FayFun)> _funs;  //函数的映射

	public:
		//是否匹配
		//当传入的类型和自己一样或是此类型的子类的时候为True
		virtual bool match(PTR(FayType) type);
		//查找符合要求的
		std::vector<PTR(FayFun)> findFun(const std::string &funName, const std::vector<PTR(FayType)> &paramsType);
		PTR(FayFun) findFun(pos_t index);
		pos_t getFunIndex(const std::string &funname);
		pos_t getFunIndex(const PTR(FayFun) &fun);
	};

	//简单类型
	class SimpleType : public FayType
	{
	private:
		static MAP<ValueType, PTR(FayType)> _Types;

		ValueType _type;

		//这种类型，不能从外部创建
		SimpleType(ValueType valueType)
			: FayType(nullptr), _type(valueType) {}

	public:
		static PTR(FayType) Get(ValueType valueType);

		// 通过 FayType 继承
		virtual const std::string &fullname() override;
	};

	//Class类型
	class FayClass : public FayType, public std::enable_shared_from_this<FayClass>
	{
	private:
		WPTR(FayLib) _lib;
		std::string _fullname;  //全名

	public:
		std::string package;  //所在的名
		std::string name;  //类的名称

		FayClass(PTR(FayDomain) domain, const std::string &package, const std::string &name)
			: FayType(domain), package(package), name(name)
		{
			this->_fullname = (package.size() > 0 ? package + "." : "") + name;
		}

		void lib(PTR(FayLib) v) { this->_lib = v; }
		PTR(FayLib) lib() { return this->_lib.lock(); }

		pos_t addFun(PTR(FayFun) fun);

		virtual const std::string &fullname() override;
		virtual void toString(mirror::utils::StringBuilder *sb) override;
	};

	//////////////////////////////////////////////////////////////////

	//函数的参数定义
	class FayParamDef : public FayLangObject, public std::enable_shared_from_this<FayParamDef>
	{
	private:
		std::string _fullname;

	public:
		std::string name;
		WPTR(FayType) type;

		FayParamDef(PTR(FayDomain) domain, const std::string &name, PTR(FayType) type)
			: FayLangObject(domain), name(name), type(type) {}

		virtual const std::string &fullname() override;
		virtual void toString(mirror::utils::StringBuilder *sb) override;
	};

	//函数
	//包括函数、内部函数、匿名函数等
	class FayFun : public FayLangObject, public std::enable_shared_from_this<FayFun>
	{
	protected:
		WPTR(FayClass) _class;  //所属的Class
		std::string _name;
		std::string _fullname;
		std::vector<PTR(FayParamDef)> _params;
		FunType _type;

	public:
		FayFun(PTR(FayDomain) domain, const std::string &name, FunType type)
			: FayLangObject(domain), _name(name), _type(type) {}
		virtual ~FayFun() {}

		//Get & Set
		const std::string &name() { return this->_name; }
		void clazz(PTR(FayClass) v) { this->_class = v; }
		PTR(FayClass) clazz() { return this->_class.lock(); }

		//添加参数描述
		void addParam(PTR(FayParamDef) def);
		//检查参数是否匹配
		bool matchParams(const std::vector<PTR(FayType)> &paramsType);

		virtual const std::string &fullname() override;
		virtual void toString(mirror::utils::StringBuilder *sb) override;
	};

	//指令函数
	class FayInstFun : public FayFun
	{
	private:
		//代码，注意这里考虑到性能，没用智能指针
		//除此所有地方不存对指令的引用，以防止出非法引用
		std::vector<FayInst *> _insts;
		//是否已经准备过
		bool isPrepared = false;
		//对代码运行前做一些预处理
		void prepareInsts();

	public:
		FayInstFun(PTR(FayDomain) domain, const std::string &name)
			: FayFun(domain, name, FunType::Code) {}
		virtual ~FayInstFun();

		void insts(std::vector<FayInst*> v) { this->_insts = v; }

		std::vector<FayInst *> &getPreparedInsts();

		virtual void toString(mirror::utils::StringBuilder *sb) override;
	};

	//内部函数
	class FayInternalFun : public FayFun
	{
	private:
		std::function<void(VMStack *)> _fun;

	public:
		FayInternalFun(PTR(FayDomain) domain, const std::string &name, std::function<void(VMStack *)> fun, std::vector<std::string> params);

		//执行内部函数
		inline void Invoke(VMStack *stack) { this->_fun(stack); }
	};

	//////////////////////////////////////////////////////////////////////

	//外部函数信息
	//在lib的内部，会创建所有的调用方法的快速索引
	//这个索引在call的时候，用于对方法进行快速的定位
	class OutsideFun : public FayObject
	{
	private:
		std::string _fullname;
		std::string _typeFullname;
		int32_t _typeIndex;
		std::string _funFullname;
		int32_t _funIndex;
		bool _resolved = false;  //是否已经确定是函数的位置

	public:
		OutsideFun(const std::string &typeFullname, int32_t typeIndex, const std::string &funName, int32_t funIndex)
			: _resolved(true), _typeFullname(typeFullname), _typeIndex(typeIndex), _funFullname(funName), _funIndex(funIndex) {}

		int32_t typeIndex() { return this->_typeIndex; }
		int32_t funIndex() { return this->_funIndex; }

		virtual void toString(mirror::utils::StringBuilder *sb) override;
	};

	//库
	class FayLib : public FayLangObject, public std::enable_shared_from_this<FayLib>
	{
	private:
		//外部函数的列表
		//这个表的主要用处，是把本Lib中的函数调用转换成索引值
		IndexMap<PTR(OutsideFun)> _outsideFuns;

	public:
		std::string name;
		std::vector<PTR(FayClass)> classes;

		FayLib(PTR(FayDomain) domain, const std::string &name)
			: FayLangObject(domain), name(name) {}
		~FayLib() {}

		pos_t addClass(PTR(FayClass) clazz);
		//返回调用方法在外部函数表中的索引
		pos_t findOutsideFun(const std::string &className, const std::string &funName, const std::vector<PTR(FayType)> &paramsType);
		PTR(OutsideFun) findOutsideFun(pos_t index) { return this->_outsideFuns.find(index); }

		virtual void toString(mirror::utils::StringBuilder *sb) override;
	};

	//当前总体管理类
	class FayDomain : public FayObject, public std::enable_shared_from_this<FayDomain>
	{
	private:
		std::vector<PTR(FayLib)> _libs;
		IndexMap<PTR(FayType)> _types;

	public:
		FayDomain();
		//初始化系统库
		void initSysLib();
		//添加Lib
		void addLib(PTR(FayLib) lib);
		//添加新的类型
		//返回类型在Domain里的序号
		pos_t addType(PTR(FayType) t);
		//查找类型在Domain中的位置
		pos_t getTypeIndex(PTR(FayType) t);
		//根据类型的全称查找类型定义
		PTR(FayType) findType(const std::string &typeFullname);
		PTR(FayType) findType(pos_t index);
		//根据引用和类型名，查找类型的定义
		std::vector<PTR(FayType)> findType(std::vector<std::string> &imports, const std::string &typeName);
		//从函数表中查找指定的函数
		std::vector<PTR(FayFun)> findFun(const std::string &className, const std::string &funName, const std::vector<PTR(FayType)> &paramsType);

		virtual void toString(mirror::utils::StringBuilder *sb) override;
	};

	/////////////////////////////////////////////////

	//类型的实例
	class FayInstance : public FayObject, public std::enable_shared_from_this<FayInstance>
	{
	public:
		PTR(FayType) type;
	};

}

