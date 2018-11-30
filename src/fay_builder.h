#pragma once

#include <mirror_sys_const.h>
#include <fay_lang.h>

namespace fay
{
	enum class BuildExprMode
	{
		Count,  //计算
		Assign  //赋值
	};

	//用于生成整体的代码结构
	class FayBuilder
	{
	private:
		std::string _package;
		std::string _filename;
		PTR(FayDomain) _domain;
		PTR(FayLib) _lib;
		PTR(FayInstClass) _class;
		PTR(FayInstFun) _fun;
		std::vector<std::string> _usings;
		std::vector<FayInst*> _insts;
		int64_t labelIndex = 1;  //用于生成label

	public:
		//用于控制表达式中的模式，在赋值运算符左边和右边的处理不一样
		BuildExprMode exprMode = BuildExprMode::Count;

		FayBuilder(PTR(FayDomain) domain)
			: _domain(domain) {}
		~FayBuilder();

		PTR(FayDomain) domain() { return this->_domain; }
		PTR(FayLib) lib() { return this->_lib; }
		PTR(FayInstClass) clazz() { return this->_class; }
		PTR(FayInstFun) fun() { return this->_fun; }
		std::vector<std::string> &usings() { return this->_usings; }

		//生成一个新的Label，并放到Fun的LabelTable里
		std::string makeLabel();
		//把taget和label绑定起来
		void useLabel(const std::string& label, int32_t* target);
		//确定指定的label地址
		void fixedLabel(const std::string& label);

		void package(const std::string &name);

		void beginFile(const std::string &filename);
		void endFile();
		void addUsing(const std::string &packageName);

		void beginLib(const std::string &name);

		pos_t addClass(const std::string &name);
		void bindClass(pos_t index);

		pos_t addFun(PTR(FayInstFun) fun);
		void bindFun(pos_t index, bool isStatic);

		void addParamDefine(const std::string &name, const std::string &type);
		void addInst(FayInst* inst);
		void optimizeInsts();
		//当前指令数量
		size_t instsSize() { return this->_insts.size(); }

		PTR(FayVarDef) findVar(const std::string &name);
		pos_t findVarIndex(const std::string &name);
	};
}

