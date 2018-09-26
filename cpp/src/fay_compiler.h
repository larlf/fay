#pragma once
#include "fay_lang_const.h"
#include "fay_lang_ins.h"
#include <vector>

using namespace fay::lang;

namespace fay
{
	namespace compiler
	{
		class Scope;
		class Block;

		/**
		 * 用于对全局进行管理
		 */
		class AppDomain
		{
		private:
			Scope* _scope;

		public:
			AppDomain();
			~AppDomain();
			Scope* currentScope();
			void run();
		};

		/**
		 * 变量定义
		 */
		class VarDefine
		{
		public:
			std::string name;
			ValueType type;
			int index;
		};

		/**
		 * 作用域
		 */
		class Scope
		{
		private:
			std::vector<VarDefine*> vars;

		public:
			Scope * parentScope;

			Scope() :parentScope(nullptr) {}
			Scope(Scope* parent) :parentScope(parent) {}

			//在当前域中添加新的变量
			int addNewVar(const std::string &name, ValueType type);
			//取得指定变量的位置
			VarDefine* getVarDefine(const std::string &name);
		};

		class Compiler
		{
		private:
			Scope* currentScope;
			std::vector<Instruction*> program;

		public:
			Compiler();
			void startScope();
			void endScope();
			void addVar(const std::string &name, const std::string &type);
			void addInstruction(Instruction* inst);
		};
	}
}

