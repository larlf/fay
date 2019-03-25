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
		std::vector<FayInst*> _insts;
		int64_t labelIndex = 1;  //用于生成label

	public:
		std::string package;
		MAP<std::string, std::string> usings;
		PTR(FayLib) lib;
		PTR(FayFile) file;
		PTR(FayInstClass) clazz;
		PTR(FayCompileFun) fun;

		//用于控制表达式中的模式，在赋值运算符左边和右边的处理不一样
		BuildExprMode exprMode = BuildExprMode::RightValue;

		//用于构造函数的参数
		std::vector<PTR(FayParamDef)> params;

		FayBuilder()
		{
		}

		~FayBuilder();

		//log记录
		void log(BuildLogLevel level, int line, int col, const std::string &msg);

		//引用相关的处理
		void addUsing(const std::string &alias, const std::string &packageName)
		{
			this->usings[alias] = packageName;
		}

		//文件的开始
		void bindFile(PTR(FayFile) file)
		{
			this->file = file;
			this->usings.clear();
		}

		//文件的结束
		void exitFile()
		{
			this->file.reset();
		}

		//Lib相关的处理
		void bindLib(PTR(FayLib) lib)
		{
			this->lib = lib;
		}

		//Class相关的处理
		//PTR(FayInstClass) addClass(const std::string &name);
		void bindClass(PTR(FayInstClass) clazz)
		{
			this->clazz = clazz;
		}

		//函数定义相关的处理
		void bindFun(PTR(FayCompileFun) fun)
		{
			this->fun = fun;
		}

		//以下是代码生成相关的内容

		//向函数添加指令
		void addInst(FayInst* inst);

		//当前指令数量
		size_t instsSize() { return this->_insts.size(); }

		//优化指令
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

