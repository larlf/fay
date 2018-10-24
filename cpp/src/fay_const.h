#pragma once

#include <iostream>
#include <mirror_sys_const.h>

typedef int64_t pos_t;

namespace fay
{
	const size_t STACK_INIT_SIZE = 1024;  //堆栈初始化的长度

	/**
	* 词法分析器的模式
	*/
	enum class LexMode
	{
		Program,
		Comment
	};

	//Token的类型
	enum class TokenType
	{
		//TokenTypeStart
		None,
		Assign,  //=
		Colon,  //:
		Semicolon,  //;
		Comma,  //,
		LeftBrace,  //{
		RightBrace,  //}
		OP,  //操作符
		DoubleSlash,  ////
		Comment,  //注释
		Using,
		Package,
		Var,
		Fun,
		Class,
		Interface,
		DescSymbol,
		SystemName,
		If,
		Else,
		ElseIf,
		For,
		Foreach,
		While,
		Do,
		Return,
		ID,
		BasicType,
		Number,
		String,
		Char,
		//TokenTypeEnd
	};

	//值类型
	enum class ValueType
	{
		//ValueTypeStart
		Void,  //没有值类型
		Byte,  //字节值
		Int,  //整形
		Long,  //长整型
		Float,  //浮点数
		Double,  //双精度浮点数
		Bool,  //布尔
		String,  //字符串
		Object,  //对象
		Function,  //函数
		//ValueTypeEnd
	};

	//方法的类型
	enum class FunType
	{
		Internal,  //内部方法
		Code  //代码中的方法
	};

	enum class InstType
	{
		PushString,
		Call
	};

	class TypeDict
	{
	private:
		static MAP<TokenType, std::string> TokenTypeName;
		static MAP<ValueType, std::string> ValueTypeName;

	public:
		static MAP<std::string, ValueType> ValueTypeMap;

		static void Init();
		static const std::string &ToName(TokenType value);
		static const std::string &ToName(ValueType value);
	};
}

