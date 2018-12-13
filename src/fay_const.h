#pragma once

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
		JumpTrue = 6422,
		Return = 4,
		PushBool = 5,
		PushByte = 6,
		PushInt = 7,
		PushLong = 8,
		PushFloat = 9,
		PushDouble = 10,
		PushString = 11,
		Dup = 6423,
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
		CopyField = 6424,
		CopyStatic = 6425,
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
		MinusVoid = 6426,
		MinusBool = 6427,
		MinusByte = 6428,
		MinusInt = 6429,
		MinusLong = 6430,
		MinusFloat = 6431,
		MinusDouble = 6432,
		MinusString = 6433,
		MinusObject = 6434,
		MinusFunction = 6435,
		PlusVoid = 6436,
		PlusBool = 6437,
		PlusByte = 6438,
		PlusInt = 6439,
		PlusLong = 6440,
		PlusFloat = 6441,
		PlusDouble = 6442,
		PlusString = 6443,
		PlusObject = 6444,
		PlusFunction = 6445,
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
		ModVoid = 6446,
		ModBool = 6447,
		ModByte = 6448,
		ModInt = 6449,
		ModLong = 6450,
		ModFloat = 6451,
		ModDouble = 6452,
		ModString = 6453,
		ModObject = 6454,
		ModFunction = 6455,
		BoolNot = 6456,
		BoolAnd = 6457,
		BoolOr = 6458,
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
		BitNotVoid = 6459,
		BitNotBool = 6460,
		BitNotByte = 6461,
		BitNotInt = 6462,
		BitNotLong = 6463,
		BitNotFloat = 6464,
		BitNotDouble = 6465,
		BitNotString = 6466,
		BitNotObject = 6467,
		BitNotFunction = 6468,
		BitAndVoid = 6469,
		BitAndBool = 6470,
		BitAndByte = 6471,
		BitAndInt = 6472,
		BitAndLong = 6473,
		BitAndFloat = 6474,
		BitAndDouble = 6475,
		BitAndString = 6476,
		BitAndObject = 6477,
		BitAndFunction = 6478,
		BitOrVoid = 6479,
		BitOrBool = 6480,
		BitOrByte = 6481,
		BitOrInt = 6482,
		BitOrLong = 6483,
		BitOrFloat = 6484,
		BitOrDouble = 6485,
		BitOrString = 6486,
		BitOrObject = 6487,
		BitOrFunction = 6488,
		BitXorVoid = 6489,
		BitXorBool = 6490,
		BitXorByte = 6491,
		BitXorInt = 6492,
		BitXorLong = 6493,
		BitXorFloat = 6494,
		BitXorDouble = 6495,
		BitXorString = 6496,
		BitXorObject = 6497,
		BitXorFunction = 6498,
		LShiftVoid = 6499,
		LShiftBool = 6500,
		LShiftByte = 6501,
		LShiftInt = 6502,
		LShiftLong = 6503,
		LShiftFloat = 6504,
		LShiftDouble = 6505,
		LShiftString = 6506,
		LShiftObject = 6507,
		LShiftFunction = 6508,
		RShiftVoid = 6509,
		RShiftBool = 6510,
		RShiftByte = 6511,
		RShiftInt = 6512,
		RShiftLong = 6513,
		RShiftFloat = 6514,
		RShiftDouble = 6515,
		RShiftString = 6516,
		RShiftObject = 6517,
		RShiftFunction = 6518,
		New = 6519,
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

