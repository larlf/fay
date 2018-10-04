#pragma once

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
		AstNode(const std::string &text)
			:_text(text) {}
		virtual ~AstNode() {}

		const std::string& text() { return this->_text; }
		size_t childNodesNum() { return this->_nodes.size(); }
		template<typename T> bool is() { return typeid(*this) == typeid(T); }

		//销毁引用关系
		void destory();
		//添加子节点
		void addChildNode(PTR(AstNode) node);
		
		virtual void toString(mirror::utils::StringBuilder &sb);
	};

	//文件节点
	class AstFile : public AstNode
	{
	public:
		using AstNode::AstNode;
	};
}