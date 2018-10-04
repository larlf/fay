﻿#pragma once

#include <mirror_sys_const.h>
#include <mirror_utils_string.h>
#include <mirror_utils_log.h>
#include <string>

namespace fay
{
	class AstNode : public std::enable_shared_from_this<AstNode>
	{
	protected:
		std::string _text;
		PTR(AstNode) _parent;
		std::vector<PTR(AstNode)> _nodes;

		//取当前Class的名字，需要启用RTTI
		std::string className();

	public:
		AstNode() {}

		AstNode(const std::string &text)
			:_text(text) {}

		AstNode(std::vector<PTR(AstNode)> nodes)
		{
			for each(auto it in nodes)
			{
				this->addChildNode(it);
			}
		}

		AstNode(const std::string &text, std::vector<PTR(AstNode)> nodes)
			:_text(text)
		{
			new (this)AstNode(nodes);
		}

		virtual ~AstNode() {}

		const std::string &text() { return this->_text; }
		size_t childNodesNum() { return this->_nodes.size(); }
		template<typename T> bool is() { return typeid(*this) == typeid(T); }

		//销毁引用关系
		void destory();
		//添加子节点
		void addChildNode(PTR(AstNode) node);

		virtual void toString(mirror::utils::StringBuilder &sb);
	};

	/******************************************************
	* 文件结构类的AST节点
	*******************************************************/

	//文件节点
	class AstFile : public AstNode
	{
		using AstNode::AstNode;
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
		using AstNode::AstNode;
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
	};

}