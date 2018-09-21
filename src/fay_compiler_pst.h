#pragma once
#include "mirage/mirage_utils.h"
#include "fay_lang_const.h"
#include "fay_lang_utils.h"
#include "fay_compiler.h"

using namespace std;
using namespace fay::lang;

namespace fay
{
	namespace compiler
	{
		/**
		 * 程序节点
		 * PST : Program Structure Tree
		 * 此结构从AST生成，然后处理进一步的优化和最终的代码生成
		 */
		class PSTNode
		{
		protected:
			uint64 _sign;  //用此值来判断类型
			vector<PSTNode *> _childNodes;

		public:
			PSTNode() : _sign(0) {}
			virtual ~PSTNode() {}

			uint64 sign() { return this->_sign; }
			void sign(uint64 v) { this->_sign |= v; }
			//返回类型名称
			string typeName();

			virtual void dump(TextBuilder *tb);
			virtual string info();

		};

		/**
		 * 表达式节点
		 */
		class ExprNode : public PSTNode
		{
		public:
			ExprNode()
			{
				this->sign(PST_Expr);
			}

			virtual ValueType valueType() = 0;
		};

		namespace pst
		{
			/**
			 * 空节点
			 */
			class Empty :public PSTNode
			{
			public:
				Empty()
				{
					this->sign(PST_Empty);
				}

				void addNode(PSTNode *node)
				{
					this->_childNodes.push_back(node);
				}
			};

			class ID : public PSTNode
			{
			private:
				string name;

			public:
				ID(const string &str) : name(str) 
				{
					this->sign(PST_ID);
				}
			};

			/**
			* 固定值
			*/
			class FixedValue : public ExprNode
			{
			public:
				VarValue value;

				FixedValue(VarValue value) : value(value) 
				{
					this->sign(PST_FixedValue);
				}

				virtual ValueType valueType() override;
			};

			/**
			* ID表达式
			*/
			class IDValue : public ExprNode
			{
			public:
				Scope *scope;
				string name;

				IDValue(Scope *scope, const string &name)
					: scope(scope), name(name) {}
				virtual ValueType valueType() override;
			};

			/**
			 * 双项表达式
			 */
			class Expr2 : public ExprNode
			{
			private:
				string op;
				ExprNode *item1;
				ExprNode *item2;

			public:
				Expr2(const string &op, ExprNode *item1, ExprNode *item2)
					: op(op), item1(item1), item2(item2) 
				{
					this->sign(PST_Expr2);
				}

				virtual ValueType valueType() override;
			};

			/**
			 * 赋值
			 */
			class Assign : public PSTNode
			{
			private:
				string id;
				ExprNode *value;

			public:
				Assign(const string &id, ExprNode *value)
					: id(id), value(value) 
				{
					this->sign(PST_Assign);
				}
			};

			/**
			 * 调用
			 */
			class Call : public PSTNode
			{
			private:
				string id;
				vector<ExprNode *> params;

			public:
				Call()
				{
					this->sign(PST_Call);
				}

				void addParam(ExprNode *param);
				void addParam(const string &id);
			};

			/**
			* 参数列表
			*/
			class Params : public PSTNode
			{
			private:
				vector<string> items;

			public:
				Params()
				{
					this->sign(PST_Params);
				}

				void addVar(const string &name);
			};
		}
	}
}



