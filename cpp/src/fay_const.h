#pragma once

#include <iostream>
#include <mirror_sys_const.h>

typedef int32_t pos_t;

namespace fay
{
	const size_t STACK_INIT_SIZE = 8;  //堆栈初始化的长度

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
		Void = 1,  //没有值类型
		Bool = 2,  //布尔
		Byte = 3,  //字节值
		Int = 4,  //整形
		Long = 5,  //长整型
		Float = 6,  //浮点数
		Double = 7,  //双精度浮点数
		String = 8,  //字符串
		Object = 9,  //对象
		Function = 10,  //函数
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
		//InstTypeStart
		PushBool = 18,
		PushByte = 19,
		PushInt = 20,
		PushLong = 21,
		PushFloat = 22,
		PushDouble = 23,
		PushString = 24,
		Pop = 32,
		PopTo = 48,
		IntToByte = 67,
		IntToInt = 68,
		IntToLong = 69,
		IntToFloat = 70,
		IntToDouble = 71,
		IntToBool = 66,
		IntToString = 72,
		AddByte = 83,
		AddInt = 84,
		AddLong = 85,
		AddFloat = 86,
		AddDouble = 87,
		AddBool = 82,
		AddString = 88,
		SubByte = 99,
		SubInt = 100,
		SubLong = 101,
		SubFloat = 102,
		SubDouble = 103,
		SubBool = 98,
		SubString = 104,
		CallStatic = 113,
		CallFun = 114,
		CallVirtual = 115,
		CallInterface = 116,
		//InstTypeEnd
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

