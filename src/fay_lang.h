#pragma once

#include <mirror_sys_const.h>
#include <fay_type.h>
#include <fay_inst.h>
#include <fay_const.h>
#include <fay_object.h>
#include <stack>

namespace fay
{
	class FayLib;
	class FayLibSet;
	class FayInstClass;
	class FayFun;
	class FayCode;
	class FayParamDef;
	class FayDomain;
	class FayClass;
	class FunTable;
	class FunTableItem;

	//工具类
	class FayLangUtils
	{
	public:
		//用于生成名称的方法
		static std::string ClassFullname(const std::string &package, const std::string &name);
		static std::string Fullname(const std::string &funName, const std::vector<PTR(FayClass)> &params);
		static std::string Fullname(const std::string &className, const std::string &funName, const std::vector<PTR(FayClass)> &params);

		//在两个类型中选出要转换的目标类型
		static ValueType WeightValueType(ValueType t1, ValueType t2);
		static PTR(FayClass) WeightValueType(PTR(FayLibSet) deps, PTR(FayClass) t1, PTR(FayClass) t2);
		//Class类型转换到ValueType类型
		static ValueType ClassToValueType(PTR(FayClass) clazz);

		//生成用于数据转换的代码
		static FayInst* ConvertInst(ValueType src, ValueType dest);
		//生成压入默认值的代码
		static FayInst* PushDefault(PTR(FayClass) type);
		//生成压入数字的代码
		static FayInst* PushNumber(ValueType type, int32_t value);
		//根据类型转换为对应的数字
		static bool IsNumberType(ValueType type);
		static bool IsIntegerNumberType(ValueType type);
		//根据类型生成操作代码
		static FayInst* OPInst(InstGroupType op, ValueType type);
	};

	////////////////////////////////////////////////////////////////

	//类型说明信息
	class TypeDesc : public BaseObject
	{
	public:
		bool isAny = true;  //是否可以为任意类型
		ValueType type = ValueType::Void;  //类型定义
		PTR(FayClass) classType;  //类的定义

		//分级处理的构造方法
		TypeDesc() {}
		TypeDesc(ValueType type) : isAny(false), type(type) {}
		TypeDesc(PTR(FayClass) clazz) : isAny(false), type(ValueType::Object), classType(clazz) {}

		virtual void buildString(mirror::utils::StringBuilder* sb) override;
	};

	//函数表的元素
	class FunTableItem
	{
	protected:
		pos_t _indexValue = -1;

	public:
		inline pos_t indexValue() { return this->_indexValue; }

		friend class FunTable;
	};

	//函数表
	class FunTable : public BaseObject
	{
	private:
		std::vector<PTR(FayFun)> _funs;  //所有的函数列表

	public:
		size_t size() { return this->_funs.size(); }

		//添加函数
		pos_t addFun(PTR(FayFun) fun);

		//取得函数
		PTR(FayFun) getFun(pos_t index);

		//根据父类的虚函数进行重建
		void rebuild(std::vector<PTR(FayFun)> &parentFuns);
		void rebuild(FunTable* parent);

		//匹配函数
		std::vector<PTR(FayFun)> matchFun(const std::string &funName, const std::vector<PTR(FayClass)> &paramsType);
		pos_t getFunIndex(const std::string &fullname);
		PTR(FayFun) findFun(const std::string &fullname);

		//根据名字查找
		std::vector<PTR(FayFun)> findFunByName(const std::string &name);
		void findFunByName(const std::string &name, std::vector<PTR(FayFun)> &list);

		virtual void buildString(mirror::utils::StringBuilder* sb) override;
	};

	//变量
	class FayVar : public BaseObject
	{
	public:
		std::string name;  //名称
		WPTR(FayClass) classType;  //类型

		FayVar(const std::string &name, PTR(FayClass) clazz)
			: name(name), classType(clazz) {}

		const std::string fullname();
		virtual void buildString(mirror::utils::StringBuilder* sb) override;
	};

	//变量定义
	class FayVarDef : public FayVar, public IndexMapItem<FayVarDef>
	{
		using FayVar::FayVar;
	public:
		virtual const std::string indexKey() override { return this->name; }
	};

	//静态变量定义
	class FayStaticVarDef : public FayVar, public IndexMapItem<FayStaticVarDef>
	{
	public:
		FayValue value;
		FayStaticVarDef(const std::string &name, PTR(FayClass) clazz);
		virtual const std::string indexKey() override { return this->name; }
	};

	//数据类型
	class FayClass : public BaseObject, public std::enable_shared_from_this<FayClass>, public IndexMapItem<FayClass>
	{
	protected:
		FunTable _sft;  //静态函数表
		FunTable _vft;  //虚函数表
		IndexMap<FayStaticVarDef> _staticVarDefs;  //静态变量表
		IndexMap<FayVarDef> _varDefs;  //变量表

		//是否已经构建过
		//这一步在解析完成后执行，主要用于创建OOP体系
		bool _isRebuild = false;
		//是否已经初始化过
		//这一步在第一次使用这个类的时候执行，主要用于静态内容进行初始化
		bool _isInited = false;

	public:
		std::string package;
		std::string name;
		WPTR(FayClass) superClass;  //父类
		WPTR(FayLib) lib;

		FayClass(const std::string &package, const std::string &name)
			: package(package), name(name) {}

		//在设置完成后调用
		virtual void onAddToLib(PTR(FayLib) lib) { this->lib = lib; }

		virtual const std::string fullname();

		inline const bool isInited() { return this->_isInited; }

		//第一次使用的时候要对类进行一下初始化
		void initClass();
		void rebuild();

		//是否匹配
		//当传入的类型和自己一样或是此类型的子类的时候为True
		virtual bool match(PTR(FayClass) type);

		//静态变量
		pos_t addStaticVar(const std::string &name, PTR(FayClass) classType);
		PTR(FayStaticVarDef) findStaticVar(const std::string &name);
		PTR(FayStaticVarDef) findStaticVar(pos_t index);
		FayValue &staticVar(pos_t index);

		//字段变量
		pos_t addVar(const std::string &name, PTR(FayClass) type);
		PTR(FayVarDef) findVar(const std::string &name);
		PTR(FayVarDef) findVar(pos_t index);
		size_t varCount() { return this->_varDefs.size(); }

		//添加函数
		pos_t addFun(PTR(FayFun) fun);
		//匹配符合要求的函数
		std::vector<PTR(FayFun)> findFun(const std::string &funName, const std::vector<PTR(FayClass)> &paramsType, bool isStatic);
		PTR(FayFun) findFun(pos_t index, bool isStatic);
		PTR(FayFun) findFun(const std::string &fullname, bool isStatic);
		std::vector<PTR(FayFun)> findFunByName(const std::string &name, bool isStatic);

		//是否可以转换为指定的类型
		bool canCovertTo(PTR(FayClass) type);

		virtual ValueType valueType() { return ValueType::Void; }
		virtual void buildString(mirror::utils::StringBuilder* sb) override;

		// Inherited via IndexMapItem
		virtual const std::string indexKey() override { return this->fullname(); }

	};

	//Class类型
	class FayInstClass : public FayClass
	{
		using FayClass::FayClass;
	public:
		virtual ValueType valueType() override;

	};

	//////////////////////////////////////////////////////////////////

	//函数的参数定义
	class FayParamDef : public BaseObject, public std::enable_shared_from_this<FayParamDef>
	{
	public:
		std::string name;  //参数名
		WPTR(FayClass) type;  //参数类型

		FayParamDef(const std::string &name, PTR(FayClass) type)
			: name(name), type(type) {}
		const std::string fullname();
		virtual void buildString(mirror::utils::StringBuilder* sb) override;
	};

	//标签
	class FayLabel : public BaseObject
	{
	private:
		std::string _name;
		int32_t _pos = -1;
		std::vector<int32_t*> _targets;

	public:
		FayLabel(const std::string &name) : _name(name) {}
		void addTarget(int32_t* target);
		void setPos(int32_t pos);
		virtual void buildString(mirror::utils::StringBuilder* sb) override;
	};

	//标签表
	class FayLabelTable : public BaseObject
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
	class FayFun : public BaseObject, public std::enable_shared_from_this<FayFun>, public FunTableItem
	{
	public:
		std::string name;  //函数名称
		FunType type = FunType::Code; //函数的类型
		bool isStatic = false; //是否是静态函数，非静态函数需要有this参数
		FunAccessType accessType = FunAccessType::Public;  //访问权限
		std::vector<PTR(FayParamDef)> params;  //参数定义
		WPTR(FayClass) returnValue;  //返回值的类型
		WPTR(FayClass) clazz;  //所属的Class

		FayFun(const std::string &name, FunType type, bool isStatic, FunAccessType accessType, std::vector<PTR(FayParamDef)> &params, PTR(FayClass) returnValue);
		virtual ~FayFun() {}

		//添加到Class时调用
		virtual void onAddToClass(PTR(FayClass) clazz) { this->clazz = clazz; }

		//检查参数是否匹配
		bool match(const std::vector<PTR(FayClass)> &paramsType);

		virtual const std::string fullname();
		virtual void buildString(mirror::utils::StringBuilder* sb) override;

	};

	//异常处理
	class TryHandler : public BaseObject
	{
	public:
		TryHandlerType type = TryHandlerType::Catch;
		int start = -1;
		int end = -1;
		int target = -1;

		virtual void buildString(mirror::utils::StringBuilder* sb) override;
	};

	//指令函数
	class FayInstFun : public FayFun
	{
	private:
		//代码，注意这里考虑到性能，没用智能指针
		//除此所有地方不存对指令的引用，以防止出非法引用
		std::vector<FayInst*> _insts;
		//内部变量表
		IndexMap<FayVarDef> _vars;
		//Catch表
		std::vector<TryHandler*> _handlers;
		//是否已经进行过预处理
		bool isPrepared = false;
		//对代码运行前做一些预处理
		void prepareInsts(PTR(FayLibSet) deps);

	public:
		FayInstFun(const std::string &name, bool isStatic, FunAccessType accessType, std::vector<PTR(FayParamDef)> &params, PTR(FayClass) returnValue)
			: FayFun(name, FunType::Code, isStatic, accessType, params, returnValue) {}
		FayInstFun(const std::string &name, bool isStatic, FunAccessType accessType, PTR(FayClass) returnValue)
			: FayFun(name, FunType::Code, isStatic, accessType, std::vector<PTR(FayParamDef)> {}, returnValue) {}
		virtual ~FayInstFun();

		//添加指令集
		inline std::vector<FayInst*> insts() { return this->_insts; }
		inline void insts(std::vector<FayInst*> v) { this->_insts = v; }
		//取得预处理后的指令集
		std::vector<FayInst*>* getPreparedInsts();

		//添加变量
		pos_t addVar(const std::string &name, PTR(FayClass) type);
		PTR(FayVarDef) findVar(const std::string &name);
		pos_t getVarIndex(const std::string &name);
		size_t varsSize() { return this->_vars.size(); }

		//TryHandler
		void addHandler(TryHandler* handler) { this->_handlers.push_back(handler); }
		//查找代码段是否在Handler中
		TryHandler* findHandler(int ip);

		virtual void buildString(mirror::utils::StringBuilder* sb) override;
	};

	//内部函数
	class FayInternalFun : public FayFun
	{
	private:
		std::function<void(PTR(std::stack<FayValue>))> _fun;

	public:
		FayInternalFun(const std::string &name, bool isStatic, std::vector<PTR(FayParamDef)> &params, PTR(FayClass) returnValue, std::function<void(PTR(std::stack<FayValue>))> fun)
			: FayFun(name, FunType::Internal, isStatic, FunAccessType::Public, params, returnValue), _fun(fun) {}
		FayInternalFun(const std::string &name, bool isStatic, PTR(FayClass) returnValue, std::function<void(PTR(std::stack<FayValue>))> fun)
			: FayFun(name, FunType::Internal, isStatic, FunAccessType::Public, std::vector<PTR(FayParamDef)> {}, returnValue), _fun(fun) {}

		//执行内部函数
		inline void Invoke(PTR(std::stack<FayValue>) stack) { this->_fun(stack); }
	};

	//用于进行编译的函数
	//里面有一些编译期才会用到的属性
	class FayCompileFun : public FayInstFun
	{
		using FayInstFun::FayInstFun;

	protected:
		PTR(FayLabelTable) _labels = MKPTR(FayLabelTable)();  //标签表，以后可以判断下，运行期不用创建此对象

	public:
		inline const PTR(FayLabelTable) labels() { return this->_labels; }

	};

	//////////////////////////////////////////////////////////////////////

	//对静态变量的引用
	class StaticVarRef : public BaseObject, public IndexMapItem<StaticVarRef>
	{
	private:
		std::string _className;
		std::string _varName;
		pos_t _classIndex;
		pos_t _varIndex;
		std::string _fullname;

	public:
		StaticVarRef(const std::string &fullname, PTR(FayClass) clazz, PTR(FayStaticVarDef) var);
		pos_t classIndex() { return this->_classIndex; }
		pos_t varIndex() { return this->_varIndex; }
		virtual const std::string indexKey() override { return this->_fullname; }
	};

	//外部函数信息
	//在lib的内部，会创建所有的调用方法的快速索引
	//这个索引在call的时候，用于对方法进行快速的定位
	class StaticFunRef : public BaseObject, public IndexMapItem<StaticFunRef>
	{
	private:
		std::string _fullname;

	public:
		pos_t classIndex = -1;
		pos_t funIndex = -1;

		StaticFunRef(const std::string &fullname, PTR(FayFun) fun);

		virtual void buildString(mirror::utils::StringBuilder* sb) override;

		// Inherited via IndexMapItem
		virtual const std::string indexKey() override { return this->_fullname; }
	};

	//库的版本信息
	class FayLibVersion
	{
	public:
		int marjor = 0;  //大版本
		int minjor = 0;  //小版本
		std::string desc;  //版本说明

		FayLibVersion() {}
		FayLibVersion(int marjor, int minjor)
			: marjor(marjor), minjor(minjor) {}
		FayLibVersion(int marjor, int minjor, const std::string &desc)
			: marjor(marjor), minjor(minjor), desc(desc) {}
	};

	//库
	class FayLib : public BaseObject, public std::enable_shared_from_this<FayLib>, public IndexMapItem<FayLib>
	{
	private:
		std::string _indexKey;  //库的Key

	public:
		std::string name;  //库的名字
		FayLibVersion ver; //版本
		PTR(FayLibSet) deps;  //依赖的所有库
		IndexMap<FayClass> classes;  //所有的Class

		FayLib(const std::string &name, PTR(FayLibSet) deps, int marjor, int minjor)
			: name(name), deps(deps), ver(marjor, minjor)
		{
			this->_indexKey = TOSTR(name << ":" << ver.marjor);
		}
		virtual ~FayLib() {}

		//进行初始化
		virtual void onAddToDomain() {}

		void addClass(PTR(FayClass) clazz);

		PTR(FayClass) findClass(const std::string &fullname);
		PTR(FayClass) findClass(ValueType type);
		LIST(PTR(FayClass)) findClassWithName(const std::string &name);

		//在库里查找Main函数的入口点
		PTR(std::vector<PTR(FayFun)>) findMainFun();

		virtual void buildString(mirror::utils::StringBuilder* sb) override;

		// Inherited via IndexMapItem
		virtual const std::string indexKey() override;
	};

	//一组Lib的集合
	class FayLibSet : public BaseObject
	{
	public:
		LIST(PTR(FayLib)) libs;

		FayLibSet() {}

		void addLib(PTR(FayLib) lib);

		virtual void buildString(mirror::utils::StringBuilder* sb) override;
		PTR(FayClass) findClass(const std::string &fullname);
		PTR(FayClass) findClass(ValueType type);
		LIST(PTR(FayClass)) findClassWithName(const std::string &name);
	};

	//当前总体管理类
	class FayDomain
	{
	public:
		static IndexMap<FayLib> Libs;

		//添加库
		static void AddLib(PTR(FayLib) lib);

		//查找LIb
		static PTR(FayLib) FindLib(const std::string &name);
		static PTR(FayLib) FindLib(const std::string &name, int marjor);
		static PTR(FayLib) FindLib(const std::string &name, int marjor, int minjor);

		//生成当前高版本的依赖库
		static PTR(FayLibSet) AllLibs();

		//定位Class
		static PTR(FayClass) FindClass(pos_t libIndex, pos_t classIndex);

		//使用Class，在VM里用这个方法，会检查是否已经初始化
		static PTR(FayClass) UseClass(pos_t libIndex, pos_t classIndex);

		//输出Domain信息
		static void buildString(mirror::utils::StringBuilder* sb);
		static std::string ToString();
	};

	/////////////////////////////////////////////////

	//类型的实例
	class FayObject : public BaseObject, public std::enable_shared_from_this<FayObject>
	{
	private:
		PTR(FayClass) _class;  //所属的类型
		std::vector<FayValue> _vars;  //字段

	public:
		FayObject(PTR(FayClass) clazz) : _class(clazz), _vars(clazz->varCount()) {}

		PTR(FayClass) clazz() { return this->_class; }
		void init();

		std::vector<FayValue> &vars() { return this->_vars; }
		void setVar(const std::string &name, FayValue &value);
	};

	/////////////////////////////////////////////////

	//语言抛出的异常
	class FayLangException : std::exception
	{
	public:
		FayLangException(const std::string &msg) : std::exception(msg.c_str()) {}
	};
}

