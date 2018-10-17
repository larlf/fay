#pragma once

#include <string>
#include <vector>
#include <mirror_sys_const.h>
#include <mirror_utils_log.h>
#include <mirror_utils_string.h>
#include <fay_const.h>
#include <fay_object.h>
#include <fay_type.h>
#include <fay_builder.h>

namespace fay
{
	class AstNode : public FayObject, public std::enable_shared_from_this<AstNode>
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
		virtual void toString(mirror::utils::StringBuilder* sb) override;

		//因为生成数据的时候对结构会有依赖，只能先生成结构，再生成代码
		//就像在田里挖土豆，挖一遍再挖一遍，需要经过多次dig()才能生成最终的结构
		//第一次，只产生Class这一级的信息
		virtual void dig1(FayBuilder* builder);
		//第二次，生成函数和函数参数列表
		virtual void dig2(FayBuilder* builder);
		//第三次，生成中间代码
		virtual void dig3(FayBuilder* builder);
		//会不会有第四次，拭目以待……
	};

	/******************************************************
	* 文件结构类的AST节点
	*******************************************************/

	//文件节点
	class AstFile : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig1(FayBuilder* builder) override;

	};

	class AstUsing : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstPackage : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig1(FayBuilder* builder) override;

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
		pos_t typeIndex = -1;  //用于记录在domain中的位置
	public:
		AstClass(const std::string &name, std::vector<std::string> &descWords)
			: AstNode(name), _descWords(descWords) {}

		virtual void dig1(FayBuilder* builder) override;
		virtual void dig2(FayBuilder* builder) override;
		virtual void dig3(FayBuilder* builder) override;
	};

	class AstCondition : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstField : public AstNode
	{
		using AstNode::AstNode;
	};

	//函数
	//下面会有三个节点：<参数表><返回值><函数体>
	class AstFun : public AstNode
	{
		using AstNode::AstNode;
	private:
		pos_t _index = -1;
	public:
		virtual void dig2(FayBuilder* builder) override;
		virtual void dig3(FayBuilder* builder) override;

	};

	class AstParamDefine : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig2(FayBuilder* builder) override;

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
	public:
		virtual ValueType valueType() override;

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
		virtual ValueType valueType() override;

		virtual void dig3(FayBuilder* builder) override;

	};

}