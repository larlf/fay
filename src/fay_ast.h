﻿#pragma once

#include <string>
#include <vector>
#include <mirror_sys_const.h>
#include <mirror_utils_log.h>
#include <mirror_utils_string.h>
#include <fay_const.h>
#include <fay_object.h>
#include <fay_type.h>
#include <fay_builder.h>
#include <fay_token.h>
#include <fay_i18n.h>

namespace fay
{
	class AstNode;

	//语法解析中的异常
	class BuildException : public FayCompileException
	{
	public:
		//stack : 当前正在处理的TokenStack
		//key : 错误信息的国际化信息
		template<typename... Params>
		BuildException(PTR(fay::AstNode) ast, const std::string &key, Params... args)
			: FayCompileException((I18N::Get(key, args...)))
		{
			if (ast)
			{
				PTR(Token) token = ast->token();
				if (token)
				{
					this->_file = token->file();
					this->_line = token->line();
					this->_col = token->col();
				}
			}
		}
	};

	class AstNode : public FayObject, public std::enable_shared_from_this<AstNode>
	{
	protected:
		std::string _text;
		WPTR(AstNode) _parent;
		std::vector<PTR(AstNode)> _nodes;
		PTR(fay::Token) _token;

	public:
		AstNode(const PTR(Token) &token)
			: _token(token), _text(token ? token->text() : "") {}

		virtual ~AstNode() {}

		const std::string &text() { return this->_text; }
		const PTR(fay::Token) &token() { return this->_token; }
		size_t childNodesNum() { return this->_nodes.size(); }

		//取子节点
		PTR(AstNode) childNode(size_t index) { return (index >= 0 && index < this->_nodes.size()) ? this->_nodes[index] : nullptr; }
		//取子节点并自带类型转换
		template<typename T> PTR(T) childNode(size_t index) { return (typeid(*this->_nodes[index]) == typeid(T)) ? TOPTR(T, this->_nodes[index]) : nullptr; }

		//取当前Class的名字，需要启用RTTI
		virtual std::string className();

		//用于判断类型
		template<typename T> bool is() 
		{ 
			if (typeid(*this) == typeid(T))
				return true;

			T* t = dynamic_cast<T*>(this);
			if (t != nullptr)
				return true;

			return false;
		}

		//添加子节点
		void addChildNode(PTR(AstNode) node);
		//插入并替换现有节点
		void insertChldNode(size_t index, PTR(AstNode) node);

		//值类型
		virtual ValueType valueType() { return ValueType::Void; }
		//对象类型
		virtual PTR(FayClass) classType(FayBuilder* builder);
		//转换成字符串
		virtual void toString(mirror::utils::StringBuilder* sb) override;

		//因为生成数据的时候对结构会有依赖，只能先生成结构，再生成代码
		//就像在田里挖土豆，挖一遍再挖一遍，需要经过多次dig()才能生成最终的结构
		//第一次，只产生Class这一级的信息
		virtual void dig1(FayBuilder* builder);
		//第二次，生成函数和函数参数列表
		virtual void dig2(FayBuilder* builder);
		//第三次，做一些生成代码之前的准备和优化的工作
		//因为一些特别的原因，这个方法编写的时候应该尽量考虑到多次执行的情况
		virtual void dig3(FayBuilder* builder);
		//第四次，生成中间代码
		virtual void dig4(FayBuilder* builder);
	};

	/******************************************************
	* 文件结构类的AST节点
	*******************************************************/

	//文件节点
	class AstFile : public AstNode
	{
	public:
		AstFile(const std::string &filename) : AstNode(nullptr)
		{
			this->_text = filename;
		}

		virtual void dig1(FayBuilder* builder) override;
		virtual void dig2(FayBuilder* builder) override;
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;

	};

	class AstUsing : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig4(FayBuilder* builder) override;
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
		AstClass(const PTR(fay::Token) &token, std::vector<std::string> &descWords)
			: AstNode(token), _descWords(descWords) {}

		virtual void dig1(FayBuilder* builder) override;
		virtual void dig2(FayBuilder* builder) override;
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstField : public AstNode
	{
		using AstNode::AstNode;
	};

	//函数
	//下面会有三个节点：<参数表><返回值><函数体>
	class AstFun : public AstNode
	{
	private:
		pos_t _index = -1;
		bool isStatic = false;
		std::vector<std::string> _descWords;

	public:
		AstFun(const PTR(Token) &token, std::vector<std::string> descWords)
			: AstNode(token), _descWords(descWords) {}

		virtual void dig2(FayBuilder* builder) override;
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;

	};

	class AstParamDefine : public AstNode
	{
		using AstNode::AstNode;
	public:
		PTR(FayClass) getType(FayBuilder* builder);
		virtual void dig2(FayBuilder* builder) override;
	};

	class AstParamDefineList : public AstNode
	{
		using AstNode::AstNode;
	public:
		std::vector<PTR(FayClass)> getTypeList(fay::FayBuilder* builder);
	};

	class AstParams : public AstNode
	{
		using AstNode::AstNode;
	public:
		size_t size() { return this->childNodesNum(); }
		std::vector<PTR(FayClass)> paramsType(FayBuilder* builder);
	};

	class AstType : public AstNode
	{
		using AstNode::AstNode;
	private:
		ValueType _valueType = ValueType::Void;

	public:
		PTR(FayClass) toFayType(FayBuilder* builder);

		virtual ValueType valueType() override;

	};

	class AstArrayType : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstID : public AstNode
	{
		using AstNode::AstNode;
	private:
		ValueType _valueType = ValueType::Void;

	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
		virtual PTR(FayClass) classType(FayBuilder* builder) override;
		virtual ValueType valueType() override;
	};

	class AstBoolOP : public AstNode
	{
		using AstNode::AstNode;
	private:
		ValueType _itemType = ValueType::Void;

	public:
		virtual ValueType valueType() override;
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstBracket : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstLeftRightOP : public AstNode
	{
		using AstNode::AstNode;
	private:
		ValueType _valueType = ValueType::Void;

	public:
		AstLeftRightOP(const PTR(Token) &token, const std::string &text)
			:AstNode::AstNode(token) { this->_text = text; }

		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;


		virtual ValueType valueType() override;

	};

	class AstPostOP : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstPreOP : public AstNode
	{
		using AstNode::AstNode;
	private:
		std::string idName;
		ValueType _valueType = ValueType::Void;

	public:
		virtual ValueType valueType() override;
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	//空语句
	class AstEmptyStmt : public AstNode
	{
		using AstNode::AstNode;
	};

	//只有表达式的语句
	class AstExprStmt : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstAssign : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstCall : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig4(FayBuilder* builder) override;

	};

	class AstIf : public AstNode
	{
		using AstNode::AstNode;
	private:
		std::string _endLabel;

	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstCondition : public AstNode
	{
		using AstNode::AstNode;
	private:
		std::string _label;
	public:
		const std::string &label() { return this->_label; }
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstFor : public AstNode
	{
		using AstNode::AstNode;

	private:
		std::string expr2Label;
		std::string endLabel;

	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstReturn : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstVar : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstByte : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstNumber : public AstNode
	{
	private:
		ValueType _class;
		FayValue _val;

	public:
		AstNumber(const PTR(Token) &token) : AstNumber::AstNumber(token, token->text()) {}
		AstNumber(const PTR(Token) &token, const std::string &text);
		virtual void dig4(FayBuilder* builder) override;
		virtual ValueType valueType() override;
	};

	//指定类型的数值
	class AstFixedNumber : public AstNode
	{
	private:
		ValueType _type;
		int32_t _value;

	public:
		AstFixedNumber(const PTR(Token) &token, ValueType type, int32_t value)
			: AstNode(token), _type(type), _value(value) {}

		virtual ValueType valueType() override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstString : public AstNode
	{
	private:
		std::string _value;
	public:
		AstString(const PTR(Token) &token);
		virtual ValueType valueType() override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstBool : public AstNode
	{
	private:
		bool _value;
	public:
		AstBool(const PTR(Token) &token);
		virtual ValueType valueType() override;
		virtual void dig4(FayBuilder* builder) override;
	};

	//类型转换
	class AstTypeConvert : public AstNode
	{
	private:
		ValueType _srcType;
		ValueType _destType;

	public:
		AstTypeConvert(const PTR(Token) &token, ValueType srcType, ValueType destType)
			: AstNode(token), _srcType(srcType), _destType(destType) {}
		virtual void dig4(FayBuilder* builder) override;

		virtual ValueType valueType() override;

	};

	class AstLabel : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstGoto : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig4(FayBuilder* builder) override;
	};

}