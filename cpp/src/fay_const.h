#pragma once

#include <iostream>
#include <mirror_sys_const.h>

typedef int32_t pos_t;

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
		Void = 1,  //没有值类型
		Byte = 2,  //字节值
		Int = 3,  //整形
		Long = 4,  //长整型
		Float = 5,  //浮点数
		Double = 6,  //双精度浮点数
		Bool = 7,  //布尔
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
		PushByte = 17,
		PushInt = 18,
		PushLong = 19,
		PushFloat = 20,
		PushDouble = 21,
		PushBool = 22,
		PushString = 23,
		Pop = 32,
		PopTo = 48,
		IntToByte = 65,
		IntToInt = 66,
		IntToLong = 67,
		IntToFloat = 68,
		IntToDouble = 69,
		IntToBool = 70,
		IntToString = 71,
		AddByte = 81,
		AddInt = 82,
		AddLong = 83,
		AddFloat = 84,
		AddDouble = 85,
		AddBool = 86,
		AddString = 87,
		SubByte = 97,
		SubInt = 98,
		SubLong = 99,
		SubFloat = 100,
		SubDouble = 101,
		SubBool = 102,
		SubString = 103,
		CallStatic = 113,
		CallSpecial = 114,
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

