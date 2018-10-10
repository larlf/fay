#pragma once

#include <string>
#include <vector>
#include <mirror_sys_const.h>
#include <mirror_utils_log.h>
#include <mirror_utils_string.h>
#include <fay_const.h>
#include <fay_builder.h>

namespace fay
{
	class AstNode : public std::enable_shared_from_this<AstNode>
	{
	protected:
		std::string _text;
		WPTR(AstNode) _parent;
		std::vector<PTR(AstNode)> _nodes;

		//取当前Class的名字，需要启用RTTI
		std::string className();

	public:
		AstNode() {}

		AstNode(const std::string &text)
			:_text(text) {}

		virtual ~AstNode() {}

		const std::string &text() { return this->_text; }
		size_t childNodesNum() { return this->_nodes.size(); }
		template<typename T> bool is() { return typeid(*this) == typeid(T); }

		//添加子节点
		void addChildNode(PTR(AstNode) node);

		//节点的数值类型
		virtual ValueType valueType() { return ValueType::Void; }
		//转换成字符串
		virtual void toString(mirror::utils::StringBuilder &sb);
		//生成代码的结构
		virtual void makeOutline(OutlineBuilder* builder);
		//生成语句代码
		virtual void makeInst(InstBuilder* builder);
	};

	/******************************************************
	* 文件结构类的AST节点
	*******************************************************/

	//文件节点
	class AstFile : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void makeOutline(OutlineBuilder* builder) override;

	};

	class AstUsing : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstPackage : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstArray : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstBlock : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstClass : public AstNode
	{
	private:
		std::vector<std::string> _descWords;
	public:
		AstClass(const std::string &name, std::vector<std::string> &descWords)
			: AstNode(name), _descWords(descWords) {}

		virtual void makeOutline(OutlineBuilder* builder) override;

	};

	class AstCondition : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstField : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstFun : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void makeOutline(OutlineBuilder* builder) override;

	};

	class AstParamDefine : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstParamDefineList : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstParams : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstType : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstArrayType : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstID : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstBoolOP : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstBracket : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstLeftRightOP : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstPostOP : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstPreOP : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstAssign : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstCall : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstIf : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstReturn : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstVar : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstByte : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstNumber : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstString : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void makeInst(InstBuilder* builder) override;

	};

	//______________________________________________

	class FayAstCode : public FayCode
	{
	private:
		PTR(AstNode) _ast;

	public:
		FayAstCode(PTR(AstNode) ast) : _ast(ast) {}

		// 通过 FayCode 继承
		virtual std::vector<PTR(FayInst)> insts() override;
	};

}