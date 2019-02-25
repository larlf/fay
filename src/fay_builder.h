#pragma once

#include <mirror_sys_const.h>
#include <fay_lang.h>

namespace fay
{
	//处理表达式所在的模式
	enum class BuildExprMode
	{
		RightValue,  //右值，用于计算
		LeftValue  //左值，用于寻址赋值
	};

	//编译时的Log的级别
	enum class BuildLogLevel
	{
		Debug,  //调试信息
		Warning,  //警告
		Error,  //错误
		Final  //致命错误
	};

	//用于生成整体的代码结构
	class FayBuilder
	{
	private:
		std::string _package;
		PTR(FayFile) _file;
		PTR(FayLib) _lib;
		PTR(FayInstClass) _class;
		PTR(FayInstFun) _fun;
		std::vector<std::string> _usings;
		std::vector<FayInst*> _insts;
		int64_t labelIndex = 1;  //用于生成label

	public:
		//用于控制表达式中的模式，在赋值运算符左边和右边的处理不一样
		BuildExprMode exprMode = BuildExprMode::RightValue;
		//用于构造函数的参数
		std::vector<PTR(FayParamDef)> params;

		FayBuilder() {}
		~FayBuilder();

		//log记录
		void log(BuildLogLevel level, int line, int col, const std::string &msg);

		PTR(FayFile) file() { return this->_file; }
		PTR(FayLib) lib() { return this->_lib; }
		PTR(FayInstClass) clazz() { return this->_class; }
		PTR(FayInstFun) fun() { return this->_fun; }

		//文件的开始和结束
		void beginFile(PTR(FayFile) file);
		void endFile();

		//Lib相关的处理
		void beginLib(const std::string &name);

		//引用相关的处理
		void addUsing(const std::string &packageName);
		std::vector<std::string> &usings() { return this->_usings; }

		//包名相关的处理
		void package(const std::string &name) { this->_package = name; }
		const std::string &package() { return this->_package; }

		//Class相关的处理
		pos_t addClass(PTR(FayInstClass) clazz);
		void bindClass(PTR(FayInstClass) clazz) { this->_class = clazz; }
		//std::vector<PTR(FayClass)> findClass(const std::string &name);

		//函数定义相关的处理
		pos_t addFun(PTR(FayInstFun) fun);
		void bindFun(PTR(FayInstFun) fun) { this->_fun = fun; }

		//以下是代码生成相关的内容

		//向函数添加指令
		void addInst(FayInst* inst);
		//当前指令数量
		size_t instsSize() { return this->_insts.size(); }
		void optimizeInsts();

		//生成一个新的Label，并放到Fun的LabelTable里
		std::string makeLabel();
		//把taget和label绑定起来
		void useLabel(const std::string &label, int32_t* target);
		//确定指定的label地址
		void fixedLabel(const std::string &label);

		//查找变量
		PTR(FayVarDef) findVar(const std::string &name);
		pos_t findVarIndex(const std::string &name);
	};
}

