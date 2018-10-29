#pragma once

#include <fay_object.h>
#include <fay_const.h>

namespace fay
{
	//指令
	class FayInst : public fay::FayObject
	{
	private:
		InstType _type;

	public:
		FayInst(InstType type) : _type(type) {}
		virtual ~FayInst() {}

		inline InstType type() { return this->_type; }

		virtual void toString(mirror::utils::StringBuilder* sb) { sb->endl(); }
	};
}

namespace fay
{
	namespace inst
	{
		//InstStart
		
		class PushBool : public fay::FayInst 
		{ 
		public:
			bool val;
			PushBool(bool val) : FayInst(InstType::PushBool),val(val) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PushByte : public fay::FayInst 
		{ 
		public:
			unsigned char val;
			PushByte(unsigned char val) : FayInst(InstType::PushByte),val(val) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PushInt : public fay::FayInst 
		{ 
		public:
			int32_t val;
			PushInt(int32_t val) : FayInst(InstType::PushInt),val(val) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PushLong : public fay::FayInst 
		{ 
		public:
			int64_t val;
			PushLong(int64_t val) : FayInst(InstType::PushLong),val(val) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PushFloat : public fay::FayInst 
		{ 
		public:
			float val;
			PushFloat(float val) : FayInst(InstType::PushFloat),val(val) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PushDouble : public fay::FayInst 
		{ 
		public:
			double val;
			PushDouble(double val) : FayInst(InstType::PushDouble),val(val) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PushString : public fay::FayInst 
		{ 
		public:
			std::string val;
			PushString(const std::string &val) : FayInst(InstType::PushString),val(val) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class Pop : public fay::FayInst 
		{ 
		public:
			Pop() : FayInst(InstType::Pop) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PopTo : public fay::FayInst 
		{ 
		public:
			int32_t count;
			PopTo(int32_t count) : FayInst(InstType::PopTo),count(count) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToByte : public fay::FayInst 
		{ 
		public:
			IntToByte() : FayInst(InstType::IntToByte) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToInt : public fay::FayInst 
		{ 
		public:
			IntToInt() : FayInst(InstType::IntToInt) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToLong : public fay::FayInst 
		{ 
		public:
			IntToLong() : FayInst(InstType::IntToLong) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToFloat : public fay::FayInst 
		{ 
		public:
			IntToFloat() : FayInst(InstType::IntToFloat) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToDouble : public fay::FayInst 
		{ 
		public:
			IntToDouble() : FayInst(InstType::IntToDouble) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToBool : public fay::FayInst 
		{ 
		public:
			IntToBool() : FayInst(InstType::IntToBool) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToString : public fay::FayInst 
		{ 
		public:
			IntToString() : FayInst(InstType::IntToString) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddByte : public fay::FayInst 
		{ 
		public:
			AddByte() : FayInst(InstType::AddByte) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddInt : public fay::FayInst 
		{ 
		public:
			AddInt() : FayInst(InstType::AddInt) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddLong : public fay::FayInst 
		{ 
		public:
			AddLong() : FayInst(InstType::AddLong) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddFloat : public fay::FayInst 
		{ 
		public:
			AddFloat() : FayInst(InstType::AddFloat) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddDouble : public fay::FayInst 
		{ 
		public:
			AddDouble() : FayInst(InstType::AddDouble) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddBool : public fay::FayInst 
		{ 
		public:
			AddBool() : FayInst(InstType::AddBool) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddString : public fay::FayInst 
		{ 
		public:
			AddString() : FayInst(InstType::AddString) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubByte : public fay::FayInst 
		{ 
		public:
			SubByte() : FayInst(InstType::SubByte) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubInt : public fay::FayInst 
		{ 
		public:
			SubInt() : FayInst(InstType::SubInt) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubLong : public fay::FayInst 
		{ 
		public:
			SubLong() : FayInst(InstType::SubLong) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubFloat : public fay::FayInst 
		{ 
		public:
			SubFloat() : FayInst(InstType::SubFloat) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubDouble : public fay::FayInst 
		{ 
		public:
			SubDouble() : FayInst(InstType::SubDouble) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubBool : public fay::FayInst 
		{ 
		public:
			SubBool() : FayInst(InstType::SubBool) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubString : public fay::FayInst 
		{ 
		public:
			SubString() : FayInst(InstType::SubString) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class CallStatic : public fay::FayInst 
		{ 
		public:
			int32_t outsideFunIndex;
			int32_t paramCount;
			int32_t typeIndex;
			int32_t funIndex;
			CallStatic(int32_t outsideFunIndex,int32_t paramCount) : FayInst(InstType::CallStatic),outsideFunIndex(outsideFunIndex),paramCount(paramCount) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class CallVirtual : public fay::FayInst 
		{ 
		public:
			int32_t outsideFunIndex;
			int32_t paramCount;
			int32_t typeIndex;
			int32_t funIndex;
			CallVirtual(int32_t outsideFunIndex,int32_t paramCount) : FayInst(InstType::CallVirtual),outsideFunIndex(outsideFunIndex),paramCount(paramCount) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SetLocal : public fay::FayInst 
		{ 
		public:
			int32_t varIndex;
			SetLocal(int32_t varIndex) : FayInst(InstType::SetLocal),varIndex(varIndex) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SetField : public fay::FayInst 
		{ 
		public:
			int32_t fieldIndex;
			SetField(int32_t fieldIndex) : FayInst(InstType::SetField),fieldIndex(fieldIndex) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SetStatic : public fay::FayInst 
		{ 
		public:
			int32_t typeIndex;
			int32_t fieldIndex;
			SetStatic(int32_t typeIndex,int32_t fieldIndex) : FayInst(InstType::SetStatic),typeIndex(typeIndex),fieldIndex(fieldIndex) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LoadLocal : public fay::FayInst 
		{ 
		public:
			int32_t varIndex;
			LoadLocal(int32_t varIndex) : FayInst(InstType::LoadLocal),varIndex(varIndex) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LoadField : public fay::FayInst 
		{ 
		public:
			int32_t fieldIndex;
			LoadField(int32_t fieldIndex) : FayInst(InstType::LoadField),fieldIndex(fieldIndex) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LoadStatic : public fay::FayInst 
		{ 
		public:
			int32_t typeIndex;
			int32_t fieldIndex;
			LoadStatic(int32_t typeIndex,int32_t fieldIndex) : FayInst(InstType::LoadStatic),typeIndex(typeIndex),fieldIndex(fieldIndex) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		//InstEnd
	}
}