﻿#pragma once

#include <iostream>
#include <mirror_sys_const.h>

typedef int32_t pos_t;  //用于对长度进行定位

namespace fay
{
	const size_t STACK_INIT_SIZE = 8;  //堆栈初始化的长度

	//一些特殊的函数名
	const std::string FUN_STATIC_INIT = ".init";
	const std::string FUN_CREATE = ".create";
	const std::string FUN_DESTORY = ".destory";
	const std::string FUN_MAIN = "main";

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
		Colon,  //:
		Semicolon,  //;
		Comma,  //,
		LeftBrace,  //{
		RightBrace,  //}
		OP,  //操作符
		DoubleSlash,  ////
		Comment,  //注释
		Using,  //using
		Package,  //package
		Var,  //var
		Fun,  //fun
		Class,  //class
		Interface,  //interface
		DescSymbol,  //public,private,protected,static
		SystemName,
		Goto,  //goto
		Label,  //label
		If,  //if
		Else,  //else
		ElseIf,  //elseif
		For,  //for
		Foreach,  //foreach
		While,  //while
		Do,  //do
		Return,  //return
		ID,
		BasicType,
		Number,  //数字
		String,  //字符串
		Char,  //字符
		Bool,  //布尔值
		As,  //as
		New,  //new,创建新对象
		Try,  //try
		Catch,  //catch
		Finally,  //finally
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

	//变量类型
	enum class VarType
	{
		Static,  //静态变量
		Field,  //字段变量
		Local,  //局部变量
		Error  //异常变量
	};

	//类的类型
	enum class ClassType
	{
		Simple,  //简单类型
		Inst,  //代码类型
	};

	//方法的类型
	enum class FunType
	{
		Internal,  //内部方法
		Code,  //代码中的方法
	};

	//方法的访问权限类型
	enum class FunAccessType
	{
		Public,
		Private,
		Protected
	};

	//Try语句块的类型
	enum class TryHandlerType
	{
		Catch,
		Finally
	};

	enum class InstGroupType
	{
		//InstGroupTypeStart
		Nop,
		Jump,
		Return,
		Leave,
		Push,
		Dup,
		Pop,
		PopTo,
		Call,
		Set,
		Load,
		Copy,
		VoidTo,
		BoolTo,
		ByteTo,
		IntTo,
		LongTo,
		FloatTo,
		DoubleTo,
		StringTo,
		ObjectTo,
		FunctionTo,
		Minus,
		Plus,
		Add,
		Sub,
		Mul,
		Div,
		Mod,
		Bool,
		Equal,
		NotEqual,
		Greater,
		GreaterEqual,
		Less,
		LessEqual,
		BitNot,
		BitAnd,
		BitOr,
		BitXor,
		LShift,
		RShift,
		//InstGroupTypeEnd
	};

	enum class InstType
	{
		//InstTypeStart
		Nop = 1,
		Jump = 2,
		JumpFalse = 3,
		JumpTrue = 10016,
		Return = 4,
		LeaveTry = 10017,
		LeaveCatch = 10018,
		LeaveFinally = 10019,
		PushBool = 5,
		PushByte = 6,
		PushInt = 7,
		PushLong = 8,
		PushFloat = 9,
		PushDouble = 10,
		PushString = 11,
		Dup = 10020,
		Pop = 12,
		PopTo = 13,
		CallStatic = 14,
		CallVirtual = 15,
		SetLocal = 16,
		SetField = 17,
		SetStatic = 18,
		LoadLocal = 19,
		LoadField = 20,
		LoadStatic = 21,
		LoadError = 10021,
		CopyLocal = 22,
		CopyField = 10022,
		CopyStatic = 10023,
		VoidToVoid = 24,
		VoidToBool = 25,
		VoidToByte = 26,
		VoidToInt = 27,
		VoidToLong = 28,
		VoidToFloat = 29,
		VoidToDouble = 30,
		VoidToString = 31,
		VoidToObject = 32,
		VoidToFunction = 33,
		BoolToVoid = 34,
		BoolToBool = 35,
		BoolToByte = 36,
		BoolToInt = 37,
		BoolToLong = 38,
		BoolToFloat = 39,
		BoolToDouble = 40,
		BoolToString = 41,
		BoolToObject = 42,
		BoolToFunction = 43,
		ByteToVoid = 44,
		ByteToBool = 45,
		ByteToByte = 46,
		ByteToInt = 47,
		ByteToLong = 48,
		ByteToFloat = 49,
		ByteToDouble = 50,
		ByteToString = 51,
		ByteToObject = 52,
		ByteToFunction = 53,
		IntToVoid = 54,
		IntToBool = 55,
		IntToByte = 56,
		IntToInt = 57,
		IntToLong = 58,
		IntToFloat = 59,
		IntToDouble = 60,
		IntToString = 61,
		IntToObject = 62,
		IntToFunction = 63,
		LongToVoid = 64,
		LongToBool = 65,
		LongToByte = 66,
		LongToInt = 67,
		LongToLong = 68,
		LongToFloat = 69,
		LongToDouble = 70,
		LongToString = 71,
		LongToObject = 72,
		LongToFunction = 73,
		FloatToVoid = 74,
		FloatToBool = 75,
		FloatToByte = 76,
		FloatToInt = 77,
		FloatToLong = 78,
		FloatToFloat = 79,
		FloatToDouble = 80,
		FloatToString = 81,
		FloatToObject = 82,
		FloatToFunction = 83,
		DoubleToVoid = 84,
		DoubleToBool = 85,
		DoubleToByte = 86,
		DoubleToInt = 87,
		DoubleToLong = 88,
		DoubleToFloat = 89,
		DoubleToDouble = 90,
		DoubleToString = 91,
		DoubleToObject = 92,
		DoubleToFunction = 93,
		StringToVoid = 94,
		StringToBool = 95,
		StringToByte = 96,
		StringToInt = 97,
		StringToLong = 98,
		StringToFloat = 99,
		StringToDouble = 100,
		StringToString = 101,
		StringToObject = 102,
		StringToFunction = 103,
		ObjectToVoid = 104,
		ObjectToBool = 105,
		ObjectToByte = 106,
		ObjectToInt = 107,
		ObjectToLong = 108,
		ObjectToFloat = 109,
		ObjectToDouble = 110,
		ObjectToString = 111,
		ObjectToObject = 112,
		ObjectToFunction = 113,
		FunctionToVoid = 114,
		FunctionToBool = 115,
		FunctionToByte = 116,
		FunctionToInt = 117,
		FunctionToLong = 118,
		FunctionToFloat = 119,
		FunctionToDouble = 120,
		FunctionToString = 121,
		FunctionToObject = 122,
		FunctionToFunction = 123,
		MinusVoid = 10024,
		MinusBool = 10025,
		MinusByte = 10026,
		MinusInt = 10027,
		MinusLong = 10028,
		MinusFloat = 10029,
		MinusDouble = 10030,
		MinusString = 10031,
		MinusObject = 10032,
		MinusFunction = 10033,
		PlusVoid = 10034,
		PlusBool = 10035,
		PlusByte = 10036,
		PlusInt = 10037,
		PlusLong = 10038,
		PlusFloat = 10039,
		PlusDouble = 10040,
		PlusString = 10041,
		PlusObject = 10042,
		PlusFunction = 10043,
		AddVoid = 124,
		AddBool = 125,
		AddByte = 126,
		AddInt = 127,
		AddLong = 128,
		AddFloat = 129,
		AddDouble = 130,
		AddString = 131,
		AddObject = 132,
		AddFunction = 133,
		SubVoid = 134,
		SubBool = 135,
		SubByte = 136,
		SubInt = 137,
		SubLong = 138,
		SubFloat = 139,
		SubDouble = 140,
		SubString = 141,
		SubObject = 142,
		SubFunction = 143,
		MulVoid = 144,
		MulBool = 145,
		MulByte = 146,
		MulInt = 147,
		MulLong = 148,
		MulFloat = 149,
		MulDouble = 150,
		MulString = 151,
		MulObject = 152,
		MulFunction = 153,
		DivVoid = 154,
		DivBool = 155,
		DivByte = 156,
		DivInt = 157,
		DivLong = 158,
		DivFloat = 159,
		DivDouble = 160,
		DivString = 161,
		DivObject = 162,
		DivFunction = 163,
		ModVoid = 10044,
		ModBool = 10045,
		ModByte = 10046,
		ModInt = 10047,
		ModLong = 10048,
		ModFloat = 10049,
		ModDouble = 10050,
		ModString = 10051,
		ModObject = 10052,
		ModFunction = 10053,
		BoolNot = 10054,
		BoolAnd = 10055,
		BoolOr = 10056,
		EqualVoid = 164,
		EqualBool = 165,
		EqualByte = 166,
		EqualInt = 167,
		EqualLong = 168,
		EqualFloat = 169,
		EqualDouble = 170,
		EqualString = 171,
		EqualObject = 172,
		EqualFunction = 173,
		NotEqualVoid = 174,
		NotEqualBool = 175,
		NotEqualByte = 176,
		NotEqualInt = 177,
		NotEqualLong = 178,
		NotEqualFloat = 179,
		NotEqualDouble = 180,
		NotEqualString = 181,
		NotEqualObject = 182,
		NotEqualFunction = 183,
		GreaterVoid = 184,
		GreaterBool = 185,
		GreaterByte = 186,
		GreaterInt = 187,
		GreaterLong = 188,
		GreaterFloat = 189,
		GreaterDouble = 190,
		GreaterString = 191,
		GreaterObject = 192,
		GreaterFunction = 193,
		GreaterEqualVoid = 194,
		GreaterEqualBool = 195,
		GreaterEqualByte = 196,
		GreaterEqualInt = 197,
		GreaterEqualLong = 198,
		GreaterEqualFloat = 199,
		GreaterEqualDouble = 200,
		GreaterEqualString = 201,
		GreaterEqualObject = 202,
		GreaterEqualFunction = 203,
		LessVoid = 204,
		LessBool = 205,
		LessByte = 206,
		LessInt = 207,
		LessLong = 208,
		LessFloat = 209,
		LessDouble = 210,
		LessString = 211,
		LessObject = 212,
		LessFunction = 213,
		LessEqualVoid = 214,
		LessEqualBool = 215,
		LessEqualByte = 216,
		LessEqualInt = 217,
		LessEqualLong = 218,
		LessEqualFloat = 219,
		LessEqualDouble = 220,
		LessEqualString = 221,
		LessEqualObject = 222,
		LessEqualFunction = 223,
		BitNotVoid = 10057,
		BitNotBool = 10058,
		BitNotByte = 10059,
		BitNotInt = 10060,
		BitNotLong = 10061,
		BitNotFloat = 10062,
		BitNotDouble = 10063,
		BitNotString = 10064,
		BitNotObject = 10065,
		BitNotFunction = 10066,
		BitAndVoid = 10067,
		BitAndBool = 10068,
		BitAndByte = 10069,
		BitAndInt = 10070,
		BitAndLong = 10071,
		BitAndFloat = 10072,
		BitAndDouble = 10073,
		BitAndString = 10074,
		BitAndObject = 10075,
		BitAndFunction = 10076,
		BitOrVoid = 10077,
		BitOrBool = 10078,
		BitOrByte = 10079,
		BitOrInt = 10080,
		BitOrLong = 10081,
		BitOrFloat = 10082,
		BitOrDouble = 10083,
		BitOrString = 10084,
		BitOrObject = 10085,
		BitOrFunction = 10086,
		BitXorVoid = 10087,
		BitXorBool = 10088,
		BitXorByte = 10089,
		BitXorInt = 10090,
		BitXorLong = 10091,
		BitXorFloat = 10092,
		BitXorDouble = 10093,
		BitXorString = 10094,
		BitXorObject = 10095,
		BitXorFunction = 10096,
		LShiftVoid = 10097,
		LShiftBool = 10098,
		LShiftByte = 10099,
		LShiftInt = 10100,
		LShiftLong = 10101,
		LShiftFloat = 10102,
		LShiftDouble = 10103,
		LShiftString = 10104,
		LShiftObject = 10105,
		LShiftFunction = 10106,
		RShiftVoid = 10107,
		RShiftBool = 10108,
		RShiftByte = 10109,
		RShiftInt = 10110,
		RShiftLong = 10111,
		RShiftFloat = 10112,
		RShiftDouble = 10113,
		RShiftString = 10114,
		RShiftObject = 10115,
		RShiftFunction = 10116,
		NewObject = 10117,
		NullObject = 10118,
		//InstTypeEnd
	};

	class TypeDict
	{
	private:
		static MAP<TokenType, std::string> TokenTypeName;
		static MAP<ValueType, std::string> ValueTypeName;
		static MAP<InstType, std::string> InstTypeName;
		static MAP<std::string, ValueType> ValueTypeMap;

	public:
		static void Init();
		static const std::string &ToName(TokenType value);
		static const std::string &ToName(ValueType value);
		static const std::string &ToName(InstType value);
		static const std::string ToName(VarType value);
		//根据名称查找ValueType
		static const ValueType ToValueType(const std::string &name);
	};
}

