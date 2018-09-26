#pragma once
#include <mirage/mirage_utils.h>
#include "fay_lang_const.h"

namespace fay
{
	namespace lang
	{
		/**
		* 指令
		*/
		class Instruction
		{
		public:
			InstructType type;
			byte *data;
			int size;

			Instruction() {}

			Instruction(InstructType type, byte *data, int size)
				: type(type)
			{
				this->data = new byte[size];
				memcpy(this->data, data, size);
				this->size = size;
			}
		};

		namespace ins
		{
			class _PushInt : public Instruction
			{
			public:
				_PushInt(int p1) {}
				~_PushInt() {}

			};

			/* InstructionDefine_Start */
			
			class PushByte : public Instruction 
			{ 
			private:
				byte p1;
				
			public:
				PushByte(byte p1)
					: p1(p1) {}
				~PushByte() {}
			};
			
			class PushInt : public Instruction 
			{ 
			private:
				int p1;
				
			public:
				PushInt(int p1)
					: p1(p1) {}
				~PushInt() {}
			};
			
			class PushLong : public Instruction 
			{ 
			private:
				int64 p1;
				
			public:
				PushLong(int64 p1)
					: p1(p1) {}
				~PushLong() {}
			};
			
			class PushFloat : public Instruction 
			{ 
			private:
				float p1;
				
			public:
				PushFloat(float p1)
					: p1(p1) {}
				~PushFloat() {}
			};
			
			class PushDouble : public Instruction 
			{ 
			private:
				double p1;
				
			public:
				PushDouble(double p1)
					: p1(p1) {}
				~PushDouble() {}
			};
			
			class PushBool : public Instruction 
			{ 
			private:
				char p1;
				
			public:
				PushBool(char p1)
					: p1(p1) {}
				~PushBool() {}
			};
			
			class PushString : public Instruction 
			{ 
			private:
				int p1;
				
			public:
				PushString(int p1)
					: p1(p1) {}
				~PushString() {}
			};
			
			class Pop : public Instruction 
			{ 
			private:
				
			public:
				Pop()
					 {}
				~Pop() {}
			};
			
			class PopTo : public Instruction 
			{ 
			private:
				int p1;
				
			public:
				PopTo(int p1)
					: p1(p1) {}
				~PopTo() {}
			};
			
			class IntToByte : public Instruction 
			{ 
			private:
				
			public:
				IntToByte()
					 {}
				~IntToByte() {}
			};
			
			class IntToInt : public Instruction 
			{ 
			private:
				
			public:
				IntToInt()
					 {}
				~IntToInt() {}
			};
			
			class IntToLong : public Instruction 
			{ 
			private:
				
			public:
				IntToLong()
					 {}
				~IntToLong() {}
			};
			
			class IntToFloat : public Instruction 
			{ 
			private:
				
			public:
				IntToFloat()
					 {}
				~IntToFloat() {}
			};
			
			class IntToDouble : public Instruction 
			{ 
			private:
				
			public:
				IntToDouble()
					 {}
				~IntToDouble() {}
			};
			
			class IntToBool : public Instruction 
			{ 
			private:
				
			public:
				IntToBool()
					 {}
				~IntToBool() {}
			};
			
			class IntToString : public Instruction 
			{ 
			private:
				
			public:
				IntToString()
					 {}
				~IntToString() {}
			};
			
			class AddByte : public Instruction 
			{ 
			private:
				
			public:
				AddByte()
					 {}
				~AddByte() {}
			};
			
			class AddInt : public Instruction 
			{ 
			private:
				
			public:
				AddInt()
					 {}
				~AddInt() {}
			};
			
			class AddLong : public Instruction 
			{ 
			private:
				
			public:
				AddLong()
					 {}
				~AddLong() {}
			};
			
			class AddFloat : public Instruction 
			{ 
			private:
				
			public:
				AddFloat()
					 {}
				~AddFloat() {}
			};
			
			class AddDouble : public Instruction 
			{ 
			private:
				
			public:
				AddDouble()
					 {}
				~AddDouble() {}
			};
			
			class AddBool : public Instruction 
			{ 
			private:
				
			public:
				AddBool()
					 {}
				~AddBool() {}
			};
			
			class AddString : public Instruction 
			{ 
			private:
				
			public:
				AddString()
					 {}
				~AddString() {}
			};
			
			class SubByte : public Instruction 
			{ 
			private:
				
			public:
				SubByte()
					 {}
				~SubByte() {}
			};
			
			class SubInt : public Instruction 
			{ 
			private:
				
			public:
				SubInt()
					 {}
				~SubInt() {}
			};
			
			class SubLong : public Instruction 
			{ 
			private:
				
			public:
				SubLong()
					 {}
				~SubLong() {}
			};
			
			class SubFloat : public Instruction 
			{ 
			private:
				
			public:
				SubFloat()
					 {}
				~SubFloat() {}
			};
			
			class SubDouble : public Instruction 
			{ 
			private:
				
			public:
				SubDouble()
					 {}
				~SubDouble() {}
			};
			
			class SubBool : public Instruction 
			{ 
			private:
				
			public:
				SubBool()
					 {}
				~SubBool() {}
			};
			
			class SubString : public Instruction 
			{ 
			private:
				
			public:
				SubString()
					 {}
				~SubString() {}
			};
			
			/* InstructionDefine_End */
		}
	}
}