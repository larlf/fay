﻿#pragma once

#include <iostream>
#include <mirror_sys_const.h>

typedef int32_t pos_t;  //用于对长度进行定位

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
		Goto,
		Label,
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
		Bool,
		As,
		New,  //创建新对象
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
		Local  //局部变量
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

	enum class InstGroupType
	{
		//InstGroupTypeStart
		Nop,
		Jump,
		Return,
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
		JumpTrue = 4365,
		Return = 4,
		PushBool = 5,
		PushByte = 6,
		PushInt = 7,
		PushLong = 8,
		PushFloat = 9,
		PushDouble = 10,
		PushString = 11,
		Dup = 4366,
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
		CopyLocal = 22,
		CopyField = 4367,
		CopyStatic = 4368,
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
		MinusVoid = 4369,
		MinusBool = 4370,
		MinusByte = 4371,
		MinusInt = 4372,
		MinusLong = 4373,
		MinusFloat = 4374,
		MinusDouble = 4375,
		MinusString = 4376,
		MinusObject = 4377,
		MinusFunction = 4378,
		PlusVoid = 4379,
		PlusBool = 4380,
		PlusByte = 4381,
		PlusInt = 4382,
		PlusLong = 4383,
		PlusFloat = 4384,
		PlusDouble = 4385,
		PlusString = 4386,
		PlusObject = 4387,
		PlusFunction = 4388,
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
		ModVoid = 4389,
		ModBool = 4390,
		ModByte = 4391,
		ModInt = 4392,
		ModLong = 4393,
		ModFloat = 4394,
		ModDouble = 4395,
		ModString = 4396,
		ModObject = 4397,
		ModFunction = 4398,
		BoolNot = 4399,
		BoolAnd = 4400,
		BoolOr = 4401,
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
		BitNotVoid = 4402,
		BitNotBool = 4403,
		BitNotByte = 4404,
		BitNotInt = 4405,
		BitNotLong = 4406,
		BitNotFloat = 4407,
		BitNotDouble = 4408,
		BitNotString = 4409,
		BitNotObject = 4410,
		BitNotFunction = 4411,
		BitAndVoid = 4412,
		BitAndBool = 4413,
		BitAndByte = 4414,
		BitAndInt = 4415,
		BitAndLong = 4416,
		BitAndFloat = 4417,
		BitAndDouble = 4418,
		BitAndString = 4419,
		BitAndObject = 4420,
		BitAndFunction = 4421,
		BitOrVoid = 4422,
		BitOrBool = 4423,
		BitOrByte = 4424,
		BitOrInt = 4425,
		BitOrLong = 4426,
		BitOrFloat = 4427,
		BitOrDouble = 4428,
		BitOrString = 4429,
		BitOrObject = 4430,
		BitOrFunction = 4431,
		BitXorVoid = 4432,
		BitXorBool = 4433,
		BitXorByte = 4434,
		BitXorInt = 4435,
		BitXorLong = 4436,
		BitXorFloat = 4437,
		BitXorDouble = 4438,
		BitXorString = 4439,
		BitXorObject = 4440,
		BitXorFunction = 4441,
		LShiftVoid = 4442,
		LShiftBool = 4443,
		LShiftByte = 4444,
		LShiftInt = 4445,
		LShiftLong = 4446,
		LShiftFloat = 4447,
		LShiftDouble = 4448,
		LShiftString = 4449,
		LShiftObject = 4450,
		LShiftFunction = 4451,
		RShiftVoid = 4452,
		RShiftBool = 4453,
		RShiftByte = 4454,
		RShiftInt = 4455,
		RShiftLong = 4456,
		RShiftFloat = 4457,
		RShiftDouble = 4458,
		RShiftString = 4459,
		RShiftObject = 4460,
		RShiftFunction = 4461,
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
		//根据名称查找ValueType
		static const ValueType ToValueType(const std::string& name);
	};
}

