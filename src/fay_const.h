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
		Bool,
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

	enum class InstGroupType
	{
		//InstGroupTypeStart
		Push = 1,
		Pop = 2,
		PopTo = 3,
		Call = 7,
		Set = 8,
		Load = 9,
		VoidTo = 21,
		BoolTo = 22,
		ByteTo = 23,
		IntTo = 24,
		LongTo = 25,
		FloatTo = 26,
		DoubleTo = 27,
		StringTo = 28,
		ObjectTo = 29,
		FunctionTo = 30,
		Add = 41,
		Sub = 42,
		Mul = 43,
		Div = 44,
		//InstGroupTypeEnd
	};

	enum class InstType
	{
		//InstTypeStart
		Nop = 0,
		PushBool = 18,
		PushByte = 19,
		PushInt = 20,
		PushLong = 21,
		PushFloat = 22,
		PushDouble = 23,
		PushString = 24,
		Pop = 32,
		PopTo = 48,
		CallStatic = 113,
		CallVirtual = 115,
		SetLocal = 129,
		SetField = 130,
		SetStatic = 131,
		LoadLocal = 145,
		LoadField = 146,
		LoadStatic = 147,
		VoidToVoid = 337,
		VoidToBool = 338,
		VoidToByte = 339,
		VoidToInt = 340,
		VoidToLong = 341,
		VoidToFloat = 342,
		VoidToDouble = 343,
		VoidToString = 344,
		VoidToObject = 345,
		VoidToFunction = 346,
		BoolToVoid = 353,
		BoolToBool = 354,
		BoolToByte = 355,
		BoolToInt = 356,
		BoolToLong = 357,
		BoolToFloat = 358,
		BoolToDouble = 359,
		BoolToString = 360,
		BoolToObject = 361,
		BoolToFunction = 362,
		ByteToVoid = 369,
		ByteToBool = 370,
		ByteToByte = 371,
		ByteToInt = 372,
		ByteToLong = 373,
		ByteToFloat = 374,
		ByteToDouble = 375,
		ByteToString = 376,
		ByteToObject = 377,
		ByteToFunction = 378,
		IntToVoid = 385,
		IntToBool = 386,
		IntToByte = 387,
		IntToInt = 388,
		IntToLong = 389,
		IntToFloat = 390,
		IntToDouble = 391,
		IntToString = 392,
		IntToObject = 393,
		IntToFunction = 394,
		LongToVoid = 401,
		LongToBool = 402,
		LongToByte = 403,
		LongToInt = 404,
		LongToLong = 405,
		LongToFloat = 406,
		LongToDouble = 407,
		LongToString = 408,
		LongToObject = 409,
		LongToFunction = 410,
		FloatToVoid = 417,
		FloatToBool = 418,
		FloatToByte = 419,
		FloatToInt = 420,
		FloatToLong = 421,
		FloatToFloat = 422,
		FloatToDouble = 423,
		FloatToString = 424,
		FloatToObject = 425,
		FloatToFunction = 426,
		DoubleToVoid = 433,
		DoubleToBool = 434,
		DoubleToByte = 435,
		DoubleToInt = 436,
		DoubleToLong = 437,
		DoubleToFloat = 438,
		DoubleToDouble = 439,
		DoubleToString = 440,
		DoubleToObject = 441,
		DoubleToFunction = 442,
		StringToVoid = 449,
		StringToBool = 450,
		StringToByte = 451,
		StringToInt = 452,
		StringToLong = 453,
		StringToFloat = 454,
		StringToDouble = 455,
		StringToString = 456,
		StringToObject = 457,
		StringToFunction = 458,
		ObjectToVoid = 465,
		ObjectToBool = 466,
		ObjectToByte = 467,
		ObjectToInt = 468,
		ObjectToLong = 469,
		ObjectToFloat = 470,
		ObjectToDouble = 471,
		ObjectToString = 472,
		ObjectToObject = 473,
		ObjectToFunction = 474,
		FunctionToVoid = 481,
		FunctionToBool = 482,
		FunctionToByte = 483,
		FunctionToInt = 484,
		FunctionToLong = 485,
		FunctionToFloat = 486,
		FunctionToDouble = 487,
		FunctionToString = 488,
		FunctionToObject = 489,
		FunctionToFunction = 490,
		AddVoid = 657,
		AddBool = 658,
		AddByte = 659,
		AddInt = 660,
		AddLong = 661,
		AddFloat = 662,
		AddDouble = 663,
		AddString = 664,
		AddObject = 665,
		AddFunction = 666,
		SubVoid = 673,
		SubBool = 674,
		SubByte = 675,
		SubInt = 676,
		SubLong = 677,
		SubFloat = 678,
		SubDouble = 679,
		SubString = 680,
		SubObject = 681,
		SubFunction = 682,
		MulVoid = 689,
		MulBool = 690,
		MulByte = 691,
		MulInt = 692,
		MulLong = 693,
		MulFloat = 694,
		MulDouble = 695,
		MulString = 696,
		MulObject = 697,
		MulFunction = 698,
		DivVoid = 705,
		DivBool = 706,
		DivByte = 707,
		DivInt = 708,
		DivLong = 709,
		DivFloat = 710,
		DivDouble = 711,
		DivString = 712,
		DivObject = 713,
		DivFunction = 714,
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

