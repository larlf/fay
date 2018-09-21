#pragma once
#include "fay_lang_utils.h"
#include "fay_compiler.h"
#include "fay_compiler_pst.h"

using namespace fay::lang;

namespace fay
{
	namespace compiler
	{
		/**
		* 语法树的节点
		*/
		class ASTNode
		{
		private:
			int _linenum;  //行号
			ASTType _type;  //类型
			std::string _text;  //文本内容
			std::vector<ASTNode *> _childNodes; //所有的子节点
			ASTNode *_parentNode;  //有一些推断需要用至父节点
			ValueType _valueType;  //用于对类型值进行缓存

			ASTNode();

		public:
			ASTNode(const char *v);
			ASTNode(ASTType type, const char *v);
			ASTNode(ASTType type, ASTNode *n1);
			ASTNode(ASTType type, ASTNode *n1, ASTNode *n2);
			ASTNode(ASTType type, ASTNode *n1, ASTNode *n2, ASTNode *n3);

			int childNodesCount() { return this->_childNodes.size(); }
			string text() const { return this->_text; }

			//取得节点的值类型
			ValueType valueType();
			//取子节点
			ASTNode *childNode(int index);
			//添加字节点
			ASTNode *addChildNode(ASTNode *node);
			//显示AST树的内容
			void dump(TextBuilder *tb);
			//生成格式化的代码
			void format(std::string &text);
			//编译
			void compile(Compiler *compiler);
			//编译代码
			void build(AppDomain *domain, Compiler *compiler);
			//显示节点信息
			string toString();
		};

		class ASTBus
		{
		public:
			static int LineNum;

			//当前解析的AST树的根节点
			static ASTNode *ASTRootNode;
		};
	}
}



