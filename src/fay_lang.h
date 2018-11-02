﻿#pragma once

#include <mirror_sys_const.h>
#include <fay_type.h>
#include <fay_inst.h>
#include <fay_const.h>
#include <fay_object.h>
#include <stack>

namespace fay
{
	class FayLib;
	class FayInstClass;
	class FayFun;
	class FayCode;
	class FayParamDef;
	class FayDomain;
	class FayClass;

	//工具类
	class FayLangUtils
	{
	public:
		//用于生成名称的方法
		static std::string Fullname(const std::string &funName, const std::vector<PTR(FayClass)> &params);
		static std::string Fullname(const std::string &className, const std::string &funName, const std::vector<PTR(FayClass)> &params);

		//在两个类型中选出要转换的目标类型
		static ValueType WeightValueType(ValueType t1, ValueType t2);
		//Class类型转换到ValueType类型
		static ValueType ClassToValueType(PTR(FayClass) clazz);

		//生成用于数据转换的代码
		static FayInst* ConvertInst(ValueType src, ValueType dest);
		//根据类型生成操作代码
		static FayInst* OPInst(InstGroupType op, ValueType type);
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

	//函数表
	class FunTable : public FayObject
	{
	private:
		std::vector<PTR(FayFun)> _funs;  //所有的函数列表

	public:
		//添加函数
		pos_t addFun(PTR(FayFun) fun);
		//取得函数
		PTR(FayFun) getFun(pos_t index);
		//根据父类的虚函数进行重建
		void rebuild(std::vector<PTR(FayFun)> &parentFuns);
		//匹配函数
		std::vector<pos_t> matchFun(const std::string &funName, const std::vector<PTR(FayClass)> &paramsType);
		pos_t findFunIndex(const std::string &fullname);
		PTR(FayFun) findFun(const std::string &fullname);

		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	//变量定义
	class FayVarDef : public FayLangObject
	{
	private:
		std::string _name;  //名称
		std::string _fullname;  //全名
		WPTR(FayClass) _class;  //类型

	public:
		FayVarDef(PTR(FayDomain) domain,  const std::string &name, PTR(FayClass) clazz)
			: FayLangObject(domain), _name(name), _class(clazz) {}

		inline const std::string &name() { return  this->_name; }
		PTR(FayClass) type() { return this->_class.lock(); }

		virtual const std::string &fullname() override;
		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	//数据类型
	class FayClass : public FayLangObject, public std::enable_shared_from_this<FayClass>
	{
	protected:
		ClassType _type;
		WPTR(FayLib) _lib;
		WPTR(FayClass) _parent;  //父类
		FunTable _sft;  //静态函数表
		FunTable _vft;  //虚函数表

	public:
		FayClass(PTR(FayDomain) domain, ClassType type) 
			: FayLangObject(domain), _type(type) {}

		//Get or set
		ClassType type() { return this->_type; }
		void lib(PTR(FayLib) v) { this->_lib = v; }
		PTR(FayLib) lib() { return this->_lib.lock(); }

		//处理父类
		inline void parent(PTR(FayClass) v) { this->_parent = v; }
		PTR(FayClass) parent() { return this->_parent.lock(); }

		//是否匹配
		//当传入的类型和自己一样或是此类型的子类的时候为True
		virtual bool match(PTR(FayClass) type);

		//添加函数
		pos_t addFun(PTR(FayFun) fun);
		//匹配符合要求的函数
		std::vector<pos_t> matchFun(const std::string &funName, const std::vector<PTR(FayClass)> &paramsType, bool isStatic);
		//根据Index取得函数
		PTR(FayFun) findFun(pos_t index, bool isStatic);
		PTR(FayFun) findFun(const std::string &fullname, bool isStatic);
		pos_t getFunIndex(const std::string &fullname, bool isStatic);
	};

	//简单类型
	class FaySimpleClass : public FayClass
	{
	private:
		static MAP<ValueType, PTR(FayClass)> _Types;

		ValueType _valueType;

		//这种类型，不能从外部创建
		FaySimpleClass(ValueType classType)
			: FayClass(nullptr, ClassType::Simple), _valueType(classType) {}

	public:
		static PTR(FayClass) Get(ValueType classType);

		inline ValueType valueType() { return this->_valueType; }

		// 通过 FayType 继承
		virtual const std::string &fullname() override;
	};

	//Class类型
	class FayInstClass : public FayClass
	{
	private:

		std::string _fullname;  //全名

	public:
		std::string package;  //所在的名
		std::string name;  //类的名称

		FayInstClass(PTR(FayDomain) domain, const std::string &package, const std::string &name)
			: FayClass(domain, ClassType::Inst), package(package), name(name)
		{
			this->_fullname = (package.size() > 0 ? package + "." : "") + name;
		}

		virtual const std::string &fullname() override;
		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	//////////////////////////////////////////////////////////////////

	//函数的参数定义
	class FayParamDef : public FayLangObject, public std::enable_shared_from_this<FayParamDef>
	{
	private:
		std::string _fullname;
		std::string _name;  //参数名
		WPTR(FayClass) _class;  //参数类型

	public:
		FayParamDef(PTR(FayDomain) domain, const std::string &name, PTR(FayClass) type)
			: FayLangObject(domain), _name(name), _class(type) {}

		inline const std::string &name() { return this->_name; }
		inline const PTR(FayClass) type() { return this->_class.lock(); }

		virtual const std::string &fullname() override;
		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	//标签
	class FayLabel
	{
	private:
		std::string _name;
		int32_t _pos = -1;
		std::vector<int32_t*> _targets;

	public:
		FayLabel(const std::string &name) : _name(name) {}
		void addTarget(int32_t* target);
		void setPos(int32_t pos);
	};

	//标签表
	class FayLabelTable : public FayObject
	{
	private:
		MAP<std::string, PTR(FayLabel)> _labels;
		PTR(FayLabel) findLabel(const std::string &name);

	public:
		void addLabel(const std::string &name);
		void addTarget(const std::string &name, int32_t* target);
		void setPos(const std::string &name, int32_t pos);
	};

	//函数
	//包括函数、内部函数、匿名函数等
	class FayFun : public FayLangObject, public std::enable_shared_from_this<FayFun>
	{
	protected:
		FunType _type;  //函数的类型
		bool _isStatic;  //是否是静态函数
		WPTR(FayClass) _class;  //所属的Class
		std::string _name;  //函数名称
		std::string _fullname;  //全名
		std::vector<PTR(FayParamDef)> _params;  //参数定义
		std::vector<WPTR(FayClass)> _returns;  //返回值的类型，支持多返回值
		PTR(FayLabelTable) _labels = MKPTR(FayLabelTable)();  //标签表，以后可以判断下，运行期不用创建此对象

	public:

		FayFun(PTR(FayDomain) domain, const std::string &name, FunType type, bool isStatic)
			: FayLangObject(domain), _name(name), _type(type), _isStatic(isStatic) {}
		virtual ~FayFun() {}

		//Get & Set
		inline const std::string &name() { return this->_name; }
		inline const FunType type() { return this->_type; }
		inline const bool isStatic() { return this->_isStatic; }
		inline void clazz(PTR(FayClass) v) { this->_class = v; }
		inline const PTR(FayClass) clazz() { return this->_class.lock(); }
		inline const size_t returnsCount() { return this->_returns.size(); }
		inline const PTR(FayLabelTable) labels() { return this->_labels; }

		//添加参数描述
		void addParam(PTR(FayParamDef) def);
		//添加返回值类型
		void addReturn(PTR(FayClass) type);
		//检查参数是否匹配
		bool match(const std::vector<PTR(FayClass)> &paramsType);

		virtual const std::string &fullname() override;
		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	//指令函数
	class FayInstFun : public FayFun
	{
	private:
		//代码，注意这里考虑到性能，没用智能指针
		//除此所有地方不存对指令的引用，以防止出非法引用
		std::vector<FayInst*> _insts;
		//内部变量表
		IndexMap<PTR(FayVarDef)> _vars;
		//是否已经进行过预处理
		bool isPrepared = false;
		//对代码运行前做一些预处理
		void prepareInsts();

	public:
		FayInstFun(PTR(FayDomain) domain, const std::string &name, bool isStatic)
			: FayFun(domain, name, FunType::Code, isStatic) {}
		virtual ~FayInstFun();

		//添加指令集
		inline void insts(std::vector<FayInst*> v) { this->_insts = v; }
		//取得预处理后的指令集
		std::vector<FayInst*>* getPreparedInsts();

		//添加变量
		pos_t addVar(const std::string &name, PTR(FayClass) type);
		PTR(FayVarDef) findVar(const std::string &name);
		pos_t getVarIndex(const std::string &name);
		size_t varsSize() { return this->_vars.size(); }

		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	//内部函数
	class FayInternalFun : public FayFun
	{
	private:
		std::function<void(std::stack<PTR(FayValue)>&)> _fun;

	public:
		FayInternalFun(PTR(FayDomain) domain, const std::string &name, std::function<void(std::stack<PTR(FayValue)>&)> fun, std::vector<std::string> params);

		//执行内部函数
		inline void Invoke(std::stack<PTR(FayValue)> &stack) { this->_fun(stack); }
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

		virtual void toString(mirror::utils::StringBuilder* sb) override;
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
		std::vector<PTR(FayInstClass)> classes;

		FayLib(PTR(FayDomain) domain, const std::string &name)
			: FayLangObject(domain), name(name) {}
		~FayLib() {}

		pos_t addClass(PTR(FayInstClass) clazz);
		//返回调用方法在外部函数表中的索引
		pos_t findOutsideFun(const std::string &className, const std::string &funName, const std::vector<PTR(FayClass)> &paramsType);
		PTR(OutsideFun) findOutsideFun(pos_t index) { return this->_outsideFuns.find(index); }

		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	//当前总体管理类
	class FayDomain : public FayObject, public std::enable_shared_from_this<FayDomain>
	{
	private:
		std::vector<PTR(FayLib)> _libs;
		IndexMap<PTR(FayClass)> _types;

	public:
		FayDomain();
		//初始化系统库
		void initSysLib();
		//添加Lib
		void addLib(PTR(FayLib) lib);
		//添加新的类型
		//返回类型在Domain里的序号
		pos_t addType(PTR(FayClass) t);
		//查找类型在Domain中的位置
		pos_t getTypeIndex(PTR(FayClass) t);
		//根据类型的全称查找类型定义
		PTR(FayClass) findType(const std::string &typeFullname);
		PTR(FayClass) findType(pos_t index);
		PTR(FayClass) findType(ValueType type);
		//根据引用和类型名，查找类型的定义
		std::vector<PTR(FayClass)> findType(std::vector<std::string> &imports, const std::string &typeName);
		//下面是用各种方式来查找函数
		//std::vector<PTR(FayFun)> findFun(const std::string &className, const std::string &funName, const std::vector<PTR(FayType)> &paramsType);
		PTR(FayFun) findFun(const std::string &typeFullname, const std::string &funFullname, bool isStatic);
		PTR(FayFun) findFun(pos_t typeIndex, pos_t funIndex, bool isStatic);
		bool findFunIndex(const std::string &typeFullname, const std::string &funFullname, bool isStatic, pos_t &typeIndex, pos_t &funIndex);
		bool findFunIndex(PTR(FayFun) fun, pos_t &typeIndex, pos_t &funIndex);

		virtual void toString(mirror::utils::StringBuilder* sb) override;
	};

	/////////////////////////////////////////////////

	//类型的实例
	class FayInstance : public FayObject, public std::enable_shared_from_this<FayInstance>
	{
	public:
		PTR(FayClass) type;
	};

}

