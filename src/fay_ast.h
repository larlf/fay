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
			if(ast)
			{
				PTR(Token) token = ast->token();
				if(token)
				{
					this->_file = token->file();
					this->_line = token->line();
					this->_col = token->col();
				}
			}
		}
	};

	class AstNode : public BaseObject, public std::enable_shared_from_this<AstNode>
	{
	protected:
		std::string _text;
		WPTR(AstNode) _parent;
		std::vector<PTR(AstNode)> _nodes;
		PTR(fay::Token) _token;
		WPTR(FayClass) _classType;

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
			if(typeid(*this) == typeid(T))
				return true;

			T* t = dynamic_cast<T*>(this);
			if(t != nullptr)
				return true;

			return false;
		}

		//添加子节点
		void addChildNode(PTR(AstNode) node);
		//插入并替换现有节点
		void insertChldNode(size_t index, PTR(AstNode) node);

		//值类型
		ValueType valueType();
		//对象类型
		PTR(FayClass) classType() { return this->_classType.lock(); }
		//转换成字符串
		virtual void buildString(mirror::utils::StringBuilder* sb) override;

		//因为生成数据的时候对结构会有依赖，只能先生成结构，再生成代码
		//就像在田里挖土豆，挖一遍再挖一遍，需要经过多次dig()才能生成最终的结构
		//第一次，只产生Class这一级的信息
		virtual void dig1(FayBuilder* builder);
		//第二次，生成函数和函数参数列表
		virtual void dig2(FayBuilder* builder);
		//第三次，确定每个节点的类型，并做一些生成代码之前的准备和优化的工作
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
		virtual void dig1(FayBuilder* builder) override;
		virtual void dig2(FayBuilder* builder) override;
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstPackage : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig1(FayBuilder* builder) override;
		virtual void dig2(FayBuilder* builder) override;
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;

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
		//pos_t classIndex = -1;  //用于记录在domain中的位置
		PTR(FayInstClass) _clazz;

		PTR(FayInstFun) _initFun;
		PTR(FayInstFun) _createFun;
		PTR(FayInstFun) _destoryFun;

	public:
		std::string superClassText;
		std::vector<std::string> interfaceText;

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
	private:
		pos_t varIndex = -1;
	public:
		MAP<std::string, bool> descWords;
		bool isStatic() { return this->descWords["static"]; }
		virtual void dig2(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	//函数
	//下面会有三个节点：<参数表><返回值><函数体>
	class AstFun : public AstNode
	{
	private:
		PTR(FayInstFun) _fun;
		//bool isStatic = false;
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
	public:
		virtual void dig2(FayBuilder* builder) override;
	};

	class AstArrayType : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstID : public AstNode
	{
		using AstNode::AstNode;
	private:
		VarType _type = VarType::Local;  //变量类型
		pos_t _classIndex = -1;
		pos_t _varIndex = -1;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstEqualityOP : public AstNode
	{
		using AstNode::AstNode;
	private:
		WPTR(FayClass) _itemType;

	public:
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
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstBitOP : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstBoolOP : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstMinusOP : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstPostOP : public AstNode
	{
		using AstNode::AstNode;
	private:
		std::string _id;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstPreOP : public AstNode
	{
		using AstNode::AstNode;
	private:
		std::string _id;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstBoolNot : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstBitNot : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstCast : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	//条件表达式
	class AstCondExpr : public AstNode
	{
		using AstNode::AstNode;
	private:
		std::string _v2Label;  //指向第二个值的标签
		std::string _endLabel;  //指向语句结束的标签
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	//空语句
	class AstEmptyStmt : public AstNode
	{
		using AstNode::AstNode;
	};

	//表达式的语句
	class AstExprStmt : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
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
	private:
		pos_t varIndex = -1;
		WPTR(FayFun) _fun;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstNew : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig3(FayBuilder* builder) override;
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

	class AstForEach : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstWhile : public AstNode
	{
		using AstNode::AstNode;
	private:
		std::string startLabel;
		std::string endLabel;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstDoWhile : public AstNode
	{
		using AstNode::AstNode;
	private:
		std::string startLabel;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstSwitch : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstReturn : public AstNode
	{
		using AstNode::AstNode;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstVar : public AstNode
	{
		using AstNode::AstNode;
	};

	class AstVarItem : public AstNode
	{
		using AstNode::AstNode;
	private:
		pos_t _index = -1;
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
		//ValueType _class;
		FayValue _val;

	public:
		AstNumber(const PTR(Token) &token) : AstNumber::AstNumber(token, token->text()) {}
		AstNumber(const PTR(Token) &token, const std::string &text);
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
		//virtual PTR(FayClass) classType(FayBuilder* builder) override;
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

		//virtual PTR(FayClass) classType(FayBuilder* builder) override;
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstString : public AstNode
	{
		using AstNode::AstNode;
	private:
		std::string _value;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	class AstBool : public AstNode
	{
		using AstNode::AstNode;
	private:
		bool _value;
	public:
		virtual void dig3(FayBuilder* builder) override;
		virtual void dig4(FayBuilder* builder) override;
	};

	//类型转换
	//注意这个节点经常是在dig3()中产生的，所以它的dig3()可能会不执行
	//有什么判断尽量都写到dig4()里
	class AstTypeConvert : public AstNode
	{
	private:
		WPTR(FayClass) _srcType;
		WPTR(FayClass) _dstType;

	public:
		AstTypeConvert(const PTR(Token) &token, PTR(FayClass) srcType, PTR(FayClass) destType);
		virtual void dig4(FayBuilder* builder) override;
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