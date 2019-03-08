#pragma once

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
		Any = 11,  //任意值，可能是上面的任一种
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
		JumpTrue = 4,
		Return = 5,
		LeaveTry = 6,
		LeaveCatch = 7,
		LeaveFinally = 8,
		PushBool = 9,
		PushByte = 10,
		PushInt = 11,
		PushLong = 12,
		PushFloat = 13,
		PushDouble = 14,
		PushString = 15,
		Dup = 16,
		Pop = 17,
		PopTo = 18,
		CallStatic = 19,
		CallVirtual = 20,
		SetLocal = 21,
		SetField = 22,
		SetStatic = 23,
		LoadLocal = 24,
		LoadField = 25,
		LoadStatic = 26,
		LoadError = 27,
		CopyLocal = 28,
		CopyField = 29,
		CopyStatic = 30,
		VoidToVoid = 31,
		VoidToBool = 32,
		VoidToByte = 33,
		VoidToInt = 34,
		VoidToLong = 35,
		VoidToFloat = 36,
		VoidToDouble = 37,
		VoidToString = 38,
		VoidToObject = 39,
		VoidToFunction = 40,
		BoolToVoid = 41,
		BoolToBool = 42,
		BoolToByte = 43,
		BoolToInt = 44,
		BoolToLong = 45,
		BoolToFloat = 46,
		BoolToDouble = 47,
		BoolToString = 48,
		BoolToObject = 49,
		BoolToFunction = 50,
		ByteToVoid = 51,
		ByteToBool = 52,
		ByteToByte = 53,
		ByteToInt = 54,
		ByteToLong = 55,
		ByteToFloat = 56,
		ByteToDouble = 57,
		ByteToString = 58,
		ByteToObject = 59,
		ByteToFunction = 60,
		IntToVoid = 61,
		IntToBool = 62,
		IntToByte = 63,
		IntToInt = 64,
		IntToLong = 65,
		IntToFloat = 66,
		IntToDouble = 67,
		IntToString = 68,
		IntToObject = 69,
		IntToFunction = 70,
		LongToVoid = 71,
		LongToBool = 72,
		LongToByte = 73,
		LongToInt = 74,
		LongToLong = 75,
		LongToFloat = 76,
		LongToDouble = 77,
		LongToString = 78,
		LongToObject = 79,
		LongToFunction = 80,
		FloatToVoid = 81,
		FloatToBool = 82,
		FloatToByte = 83,
		FloatToInt = 84,
		FloatToLong = 85,
		FloatToFloat = 86,
		FloatToDouble = 87,
		FloatToString = 88,
		FloatToObject = 89,
		FloatToFunction = 90,
		DoubleToVoid = 91,
		DoubleToBool = 92,
		DoubleToByte = 93,
		DoubleToInt = 94,
		DoubleToLong = 95,
		DoubleToFloat = 96,
		DoubleToDouble = 97,
		DoubleToString = 98,
		DoubleToObject = 99,
		DoubleToFunction = 100,
		StringToVoid = 101,
		StringToBool = 102,
		StringToByte = 103,
		StringToInt = 104,
		StringToLong = 105,
		StringToFloat = 106,
		StringToDouble = 107,
		StringToString = 108,
		StringToObject = 109,
		StringToFunction = 110,
		ObjectToVoid = 111,
		ObjectToBool = 112,
		ObjectToByte = 113,
		ObjectToInt = 114,
		ObjectToLong = 115,
		ObjectToFloat = 116,
		ObjectToDouble = 117,
		ObjectToString = 118,
		ObjectToObject = 119,
		ObjectToFunction = 120,
		FunctionToVoid = 121,
		FunctionToBool = 122,
		FunctionToByte = 123,
		FunctionToInt = 124,
		FunctionToLong = 125,
		FunctionToFloat = 126,
		FunctionToDouble = 127,
		FunctionToString = 128,
		FunctionToObject = 129,
		FunctionToFunction = 130,
		MinusVoid = 131,
		MinusBool = 132,
		MinusByte = 133,
		MinusInt = 134,
		MinusLong = 135,
		MinusFloat = 136,
		MinusDouble = 137,
		MinusString = 138,
		MinusObject = 139,
		MinusFunction = 140,
		PlusVoid = 141,
		PlusBool = 142,
		PlusByte = 143,
		PlusInt = 144,
		PlusLong = 145,
		PlusFloat = 146,
		PlusDouble = 147,
		PlusString = 148,
		PlusObject = 149,
		PlusFunction = 150,
		AddVoid = 151,
		AddBool = 152,
		AddByte = 153,
		AddInt = 154,
		AddLong = 155,
		AddFloat = 156,
		AddDouble = 157,
		AddString = 158,
		AddObject = 159,
		AddFunction = 160,
		SubVoid = 161,
		SubBool = 162,
		SubByte = 163,
		SubInt = 164,
		SubLong = 165,
		SubFloat = 166,
		SubDouble = 167,
		SubString = 168,
		SubObject = 169,
		SubFunction = 170,
		MulVoid = 171,
		MulBool = 172,
		MulByte = 173,
		MulInt = 174,
		MulLong = 175,
		MulFloat = 176,
		MulDouble = 177,
		MulString = 178,
		MulObject = 179,
		MulFunction = 180,
		DivVoid = 181,
		DivBool = 182,
		DivByte = 183,
		DivInt = 184,
		DivLong = 185,
		DivFloat = 186,
		DivDouble = 187,
		DivString = 188,
		DivObject = 189,
		DivFunction = 190,
		ModVoid = 191,
		ModBool = 192,
		ModByte = 193,
		ModInt = 194,
		ModLong = 195,
		ModFloat = 196,
		ModDouble = 197,
		ModString = 198,
		ModObject = 199,
		ModFunction = 200,
		BoolNot = 201,
		BoolAnd = 202,
		BoolOr = 203,
		EqualVoid = 204,
		EqualBool = 205,
		EqualByte = 206,
		EqualInt = 207,
		EqualLong = 208,
		EqualFloat = 209,
		EqualDouble = 210,
		EqualString = 211,
		EqualObject = 212,
		EqualFunction = 213,
		NotEqualVoid = 214,
		NotEqualBool = 215,
		NotEqualByte = 216,
		NotEqualInt = 217,
		NotEqualLong = 218,
		NotEqualFloat = 219,
		NotEqualDouble = 220,
		NotEqualString = 221,
		NotEqualObject = 222,
		NotEqualFunction = 223,
		GreaterVoid = 224,
		GreaterBool = 225,
		GreaterByte = 226,
		GreaterInt = 227,
		GreaterLong = 228,
		GreaterFloat = 229,
		GreaterDouble = 230,
		GreaterString = 231,
		GreaterObject = 232,
		GreaterFunction = 233,
		GreaterEqualVoid = 234,
		GreaterEqualBool = 235,
		GreaterEqualByte = 236,
		GreaterEqualInt = 237,
		GreaterEqualLong = 238,
		GreaterEqualFloat = 239,
		GreaterEqualDouble = 240,
		GreaterEqualString = 241,
		GreaterEqualObject = 242,
		GreaterEqualFunction = 243,
		LessVoid = 244,
		LessBool = 245,
		LessByte = 246,
		LessInt = 247,
		LessLong = 248,
		LessFloat = 249,
		LessDouble = 250,
		LessString = 251,
		LessObject = 252,
		LessFunction = 253,
		LessEqualVoid = 254,
		LessEqualBool = 255,
		LessEqualByte = 256,
		LessEqualInt = 257,
		LessEqualLong = 258,
		LessEqualFloat = 259,
		LessEqualDouble = 260,
		LessEqualString = 261,
		LessEqualObject = 262,
		LessEqualFunction = 263,
		BitNotVoid = 264,
		BitNotBool = 265,
		BitNotByte = 266,
		BitNotInt = 267,
		BitNotLong = 268,
		BitNotFloat = 269,
		BitNotDouble = 270,
		BitNotString = 271,
		BitNotObject = 272,
		BitNotFunction = 273,
		BitAndVoid = 274,
		BitAndBool = 275,
		BitAndByte = 276,
		BitAndInt = 277,
		BitAndLong = 278,
		BitAndFloat = 279,
		BitAndDouble = 280,
		BitAndString = 281,
		BitAndObject = 282,
		BitAndFunction = 283,
		BitOrVoid = 284,
		BitOrBool = 285,
		BitOrByte = 286,
		BitOrInt = 287,
		BitOrLong = 288,
		BitOrFloat = 289,
		BitOrDouble = 290,
		BitOrString = 291,
		BitOrObject = 292,
		BitOrFunction = 293,
		BitXorVoid = 294,
		BitXorBool = 295,
		BitXorByte = 296,
		BitXorInt = 297,
		BitXorLong = 298,
		BitXorFloat = 299,
		BitXorDouble = 300,
		BitXorString = 301,
		BitXorObject = 302,
		BitXorFunction = 303,
		LShiftVoid = 304,
		LShiftBool = 305,
		LShiftByte = 306,
		LShiftInt = 307,
		LShiftLong = 308,
		LShiftFloat = 309,
		LShiftDouble = 310,
		LShiftString = 311,
		LShiftObject = 312,
		LShiftFunction = 313,
		RShiftVoid = 314,
		RShiftBool = 315,
		RShiftByte = 316,
		RShiftInt = 317,
		RShiftLong = 318,
		RShiftFloat = 319,
		RShiftDouble = 320,
		RShiftString = 321,
		RShiftObject = 322,
		RShiftFunction = 323,
		NewObject = 324,
		NullObject = 325,
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

