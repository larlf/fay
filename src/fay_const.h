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
		JumpTrue = 3492,
		Return = 4,
		PushBool = 5,
		PushByte = 6,
		PushInt = 7,
		PushLong = 8,
		PushFloat = 9,
		PushDouble = 10,
		PushString = 11,
		Dup = 3493,
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
		CopyField = 3494,
		CopyStatic = 3495,
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
		MinusVoid = 3496,
		MinusBool = 3497,
		MinusByte = 3498,
		MinusInt = 3499,
		MinusLong = 3500,
		MinusFloat = 3501,
		MinusDouble = 3502,
		MinusString = 3503,
		MinusObject = 3504,
		MinusFunction = 3505,
		PlusVoid = 3506,
		PlusBool = 3507,
		PlusByte = 3508,
		PlusInt = 3509,
		PlusLong = 3510,
		PlusFloat = 3511,
		PlusDouble = 3512,
		PlusString = 3513,
		PlusObject = 3514,
		PlusFunction = 3515,
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
		ModVoid = 3516,
		ModBool = 3517,
		ModByte = 3518,
		ModInt = 3519,
		ModLong = 3520,
		ModFloat = 3521,
		ModDouble = 3522,
		ModString = 3523,
		ModObject = 3524,
		ModFunction = 3525,
		BoolNot = 3526,
		BoolAnd = 3527,
		BoolOr = 3528,
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
		BitNotVoid = 3529,
		BitNotBool = 3530,
		BitNotByte = 3531,
		BitNotInt = 3532,
		BitNotLong = 3533,
		BitNotFloat = 3534,
		BitNotDouble = 3535,
		BitNotString = 3536,
		BitNotObject = 3537,
		BitNotFunction = 3538,
		BitAndVoid = 3539,
		BitAndBool = 3540,
		BitAndByte = 3541,
		BitAndInt = 3542,
		BitAndLong = 3543,
		BitAndFloat = 3544,
		BitAndDouble = 3545,
		BitAndString = 3546,
		BitAndObject = 3547,
		BitAndFunction = 3548,
		BitOrVoid = 3549,
		BitOrBool = 3550,
		BitOrByte = 3551,
		BitOrInt = 3552,
		BitOrLong = 3553,
		BitOrFloat = 3554,
		BitOrDouble = 3555,
		BitOrString = 3556,
		BitOrObject = 3557,
		BitOrFunction = 3558,
		BitXorVoid = 3559,
		BitXorBool = 3560,
		BitXorByte = 3561,
		BitXorInt = 3562,
		BitXorLong = 3563,
		BitXorFloat = 3564,
		BitXorDouble = 3565,
		BitXorString = 3566,
		BitXorObject = 3567,
		BitXorFunction = 3568,
		LShiftVoid = 3569,
		LShiftBool = 3570,
		LShiftByte = 3571,
		LShiftInt = 3572,
		LShiftLong = 3573,
		LShiftFloat = 3574,
		LShiftDouble = 3575,
		LShiftString = 3576,
		LShiftObject = 3577,
		LShiftFunction = 3578,
		RShiftVoid = 3579,
		RShiftBool = 3580,
		RShiftByte = 3581,
		RShiftInt = 3582,
		RShiftLong = 3583,
		RShiftFloat = 3584,
		RShiftDouble = 3585,
		RShiftString = 3586,
		RShiftObject = 3587,
		RShiftFunction = 3588,
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

