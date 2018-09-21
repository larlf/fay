#pragma once
#include <iostream>
#include <mirage/mirage_const.h>

using namespace std;

namespace fay
{
	namespace lang
	{
		//语法树中节点的类型
		//Abstract Syntax Tree
		enum ASTType
		{
			//ASTType_Start
			AST_Text = 1,
			AST_File = 2,
			AST_ID = 3,
			AST_IntValue = 4,
			AST_LongValue = 5,
			AST_FloatValue = 6,
			AST_DoubleValue = 7,
			AST_StringValue = 8,
			AST_Expr1Left = 9,
			AST_Expr1Right = 10,
			AST_Expr2 = 11,
			AST_Expr3 = 12,
			AST_Call = 13,
			AST_Params = 14,
			AST_VarStmt = 15,
			AST_CallStmt = 16,
			AST_AssignStmt = 17
			//ASTType_End
		};

		//程序结构的节点类型
		//Program Struct Tree
		enum PSTType
		{
			//PSTType_Start
			PST_Empty = 1,
			PST_ID = 2,
			PST_FixedValue = 4,
			PST_Expr = 8,
			PST_Expr1Left = 16,
			PST_Expr1Right = 32,
			PST_Expr2 = 64,
			PST_Expr3 = 128,
			PST_Call = 256,
			PST_Params = 512,
			PST_Assign = 1024
			//PSTType_End
		};

		//所有的指令
		enum InstructType
		{
			//InstructType_Start
			INS_PushByte = 1,
			INS_PushInt = 2,
			INS_PushLong = 3,
			INS_PushFloat = 4,
			INS_PushDouble = 5,
			INS_PushBool = 6,
			INS_PushString = 7,
			INS_Pop = 8,
			INS_PopTo = 9,
			INS_IntToByte = 10,
			INS_IntToInt = 11,
			INS_IntToLong = 12,
			INS_IntToFloat = 13,
			INS_IntToDouble = 14,
			INS_IntToBool = 15,
			INS_IntToString = 16,
			INS_AddByte = 17,
			INS_AddInt = 18,
			INS_AddLong = 19,
			INS_AddFloat = 20,
			INS_AddDouble = 21,
			INS_AddBool = 22,
			INS_AddString = 23,
			INS_SubByte = 24,
			INS_SubInt = 25,
			INS_SubLong = 26,
			INS_SubFloat = 27,
			INS_SubDouble = 28,
			INS_SubBool = 29,
			INS_SubString = 30
			//InstructType_End
		};

		//值类型
		enum ValueType
		{
			//ValueType_Start
			VAL_Void = 1,
			VAL_Byte = 2,
			VAL_Int = 3,
			VAL_Long = 4,
			VAL_Float = 5,
			VAL_Double = 6,
			VAL_Bool = 7,
			VAL_String = 8,
			VAL_Object = 9,
			VAL_Function = 10
			//ValueType_End
		};


		//值的联合体
		union VarValueUnion
		{
			unsigned long value;
			unsigned char byteValue;
			int intValue;
			long longValue;
			float floatValue;
			double doubleValue;
			bool boolValue;
			void* stringValue;
			void* functionValue;
			void* objectValue;
		};

		//值
		class VarValue
		{
		public:
			ValueType type;
			VarValueUnion value;

			VarValue() : type(VAL_Void)
			{
				this->value.value = 0;
			}

			VarValue(int v) : type(VAL_Int)
			{
				this->value.intValue = v;
			}

			VarValue(long v) : type(VAL_Long)
			{
				this->value.longValue = v;
			}

			string toString();
		};

		class FayConst
		{
		private:
			class FayConstInit
			{
			public:
				FayConstInit();
			};
			static FayConstInit __init;

		public:
			static MAP<ASTType, string> ASTTypeName;
			static MAP<InstructType, string> InstructTypeName;
			static MAP<ValueType, string> ValueTypeName;
			static MAP<string, ValueType> ValueTypeValue;
			static MAP<PSTType, string> PSTTypeName;
		};

		class FayUtils
		{
		public:
			static ValueType StringToValueType(const string &str);
			static string ValueTypeToString(ValueType type);
			static string ASTTypeToString(ASTType type);
		};
	}
}


