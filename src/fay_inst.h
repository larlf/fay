#pragma once

#include <fay_object.h>
#include <fay_const.h>

namespace fay
{
	//指令
	class FayInst : public fay::BaseObject
	{
	private:
		InstType _class;

	public:
		FayInst(InstType type) : _class(type) {}
		virtual ~FayInst() {}

		inline InstType type() { return this->_class; }

		virtual void buildString(mirror::utils::StringBuilder* sb) { sb->endl(); }
	};
}

namespace fay
{
	namespace inst
	{
		//InstStart
		
		class Nop : public fay::FayInst 
		{ 
		public:
			Nop() : FayInst(InstType::Nop) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class Jump : public fay::FayInst 
		{ 
		public:
			int32_t target;
			Jump(int32_t target) : FayInst(InstType::Jump),target(target) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class JumpFalse : public fay::FayInst 
		{ 
		public:
			int32_t target;
			JumpFalse(int32_t target) : FayInst(InstType::JumpFalse),target(target) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class JumpTrue : public fay::FayInst 
		{ 
		public:
			int32_t target;
			JumpTrue(int32_t target) : FayInst(InstType::JumpTrue),target(target) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class Return : public fay::FayInst 
		{ 
		public:
			Return() : FayInst(InstType::Return) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LeaveTry : public fay::FayInst 
		{ 
		public:
			LeaveTry() : FayInst(InstType::LeaveTry) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LeaveCatch : public fay::FayInst 
		{ 
		public:
			LeaveCatch() : FayInst(InstType::LeaveCatch) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LeaveFinally : public fay::FayInst 
		{ 
		public:
			LeaveFinally() : FayInst(InstType::LeaveFinally) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PushBool : public fay::FayInst 
		{ 
		public:
			bool val;
			PushBool(bool val) : FayInst(InstType::PushBool),val(val) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PushByte : public fay::FayInst 
		{ 
		public:
			unsigned char val;
			PushByte(unsigned char val) : FayInst(InstType::PushByte),val(val) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PushInt : public fay::FayInst 
		{ 
		public:
			int32_t val;
			PushInt(int32_t val) : FayInst(InstType::PushInt),val(val) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PushLong : public fay::FayInst 
		{ 
		public:
			int64_t val;
			PushLong(int64_t val) : FayInst(InstType::PushLong),val(val) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PushFloat : public fay::FayInst 
		{ 
		public:
			float val;
			PushFloat(float val) : FayInst(InstType::PushFloat),val(val) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PushDouble : public fay::FayInst 
		{ 
		public:
			double val;
			PushDouble(double val) : FayInst(InstType::PushDouble),val(val) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PushString : public fay::FayInst 
		{ 
		public:
			std::string val;
			PushString(const std::string &val) : FayInst(InstType::PushString),val(val) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class Dup : public fay::FayInst 
		{ 
		public:
			Dup() : FayInst(InstType::Dup) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class Pop : public fay::FayInst 
		{ 
		public:
			Pop() : FayInst(InstType::Pop) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PopTo : public fay::FayInst 
		{ 
		public:
			int32_t count;
			PopTo(int32_t count) : FayInst(InstType::PopTo),count(count) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class CallStatic : public fay::FayInst 
		{ 
		public:
			std::string className;
			std::string funName;
			int32_t classIndex;
			int32_t funIndex;
			CallStatic(const std::string &className,const std::string &funName) : FayInst(InstType::CallStatic),className(className),funName(funName) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class CallVirtual : public fay::FayInst 
		{ 
		public:
			std::string className;
			std::string funName;
			int32_t classIndex;
			int32_t funIndex;
			CallVirtual(const std::string &className,const std::string &funName) : FayInst(InstType::CallVirtual),className(className),funName(funName) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SetLocal : public fay::FayInst 
		{ 
		public:
			int32_t varIndex;
			SetLocal(int32_t varIndex) : FayInst(InstType::SetLocal),varIndex(varIndex) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SetField : public fay::FayInst 
		{ 
		public:
			int32_t fieldIndex;
			SetField(int32_t fieldIndex) : FayInst(InstType::SetField),fieldIndex(fieldIndex) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SetStatic : public fay::FayInst 
		{ 
		public:
			std::string className;
			std::string varName;
			int32_t classIndex;
			int32_t varIndex;
			SetStatic(const std::string &className,const std::string &varName) : FayInst(InstType::SetStatic),className(className),varName(varName) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LoadLocal : public fay::FayInst 
		{ 
		public:
			int32_t varIndex;
			LoadLocal(int32_t varIndex) : FayInst(InstType::LoadLocal),varIndex(varIndex) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LoadField : public fay::FayInst 
		{ 
		public:
			int32_t varIndex;
			LoadField(int32_t varIndex) : FayInst(InstType::LoadField),varIndex(varIndex) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LoadStatic : public fay::FayInst 
		{ 
		public:
			std::string className;
			std::string varName;
			int32_t classIndex;
			int32_t varIndex;
			LoadStatic(const std::string &className,const std::string &varName) : FayInst(InstType::LoadStatic),className(className),varName(varName) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class CopyLocal : public fay::FayInst 
		{ 
		public:
			int32_t fieldIndex;
			CopyLocal(int32_t fieldIndex) : FayInst(InstType::CopyLocal),fieldIndex(fieldIndex) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class CopyField : public fay::FayInst 
		{ 
		public:
			CopyField() : FayInst(InstType::CopyField) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class CopyStatic : public fay::FayInst 
		{ 
		public:
			CopyStatic() : FayInst(InstType::CopyStatic) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToVoid : public fay::FayInst 
		{ 
		public:
			VoidToVoid() : FayInst(InstType::VoidToVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToBool : public fay::FayInst 
		{ 
		public:
			VoidToBool() : FayInst(InstType::VoidToBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToByte : public fay::FayInst 
		{ 
		public:
			VoidToByte() : FayInst(InstType::VoidToByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToInt : public fay::FayInst 
		{ 
		public:
			VoidToInt() : FayInst(InstType::VoidToInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToLong : public fay::FayInst 
		{ 
		public:
			VoidToLong() : FayInst(InstType::VoidToLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToFloat : public fay::FayInst 
		{ 
		public:
			VoidToFloat() : FayInst(InstType::VoidToFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToDouble : public fay::FayInst 
		{ 
		public:
			VoidToDouble() : FayInst(InstType::VoidToDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToString : public fay::FayInst 
		{ 
		public:
			VoidToString() : FayInst(InstType::VoidToString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToObject : public fay::FayInst 
		{ 
		public:
			VoidToObject() : FayInst(InstType::VoidToObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToFunction : public fay::FayInst 
		{ 
		public:
			VoidToFunction() : FayInst(InstType::VoidToFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToVoid : public fay::FayInst 
		{ 
		public:
			BoolToVoid() : FayInst(InstType::BoolToVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToBool : public fay::FayInst 
		{ 
		public:
			BoolToBool() : FayInst(InstType::BoolToBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToByte : public fay::FayInst 
		{ 
		public:
			BoolToByte() : FayInst(InstType::BoolToByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToInt : public fay::FayInst 
		{ 
		public:
			BoolToInt() : FayInst(InstType::BoolToInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToLong : public fay::FayInst 
		{ 
		public:
			BoolToLong() : FayInst(InstType::BoolToLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToFloat : public fay::FayInst 
		{ 
		public:
			BoolToFloat() : FayInst(InstType::BoolToFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToDouble : public fay::FayInst 
		{ 
		public:
			BoolToDouble() : FayInst(InstType::BoolToDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToString : public fay::FayInst 
		{ 
		public:
			BoolToString() : FayInst(InstType::BoolToString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToObject : public fay::FayInst 
		{ 
		public:
			BoolToObject() : FayInst(InstType::BoolToObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToFunction : public fay::FayInst 
		{ 
		public:
			BoolToFunction() : FayInst(InstType::BoolToFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToVoid : public fay::FayInst 
		{ 
		public:
			ByteToVoid() : FayInst(InstType::ByteToVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToBool : public fay::FayInst 
		{ 
		public:
			ByteToBool() : FayInst(InstType::ByteToBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToByte : public fay::FayInst 
		{ 
		public:
			ByteToByte() : FayInst(InstType::ByteToByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToInt : public fay::FayInst 
		{ 
		public:
			ByteToInt() : FayInst(InstType::ByteToInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToLong : public fay::FayInst 
		{ 
		public:
			ByteToLong() : FayInst(InstType::ByteToLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToFloat : public fay::FayInst 
		{ 
		public:
			ByteToFloat() : FayInst(InstType::ByteToFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToDouble : public fay::FayInst 
		{ 
		public:
			ByteToDouble() : FayInst(InstType::ByteToDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToString : public fay::FayInst 
		{ 
		public:
			ByteToString() : FayInst(InstType::ByteToString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToObject : public fay::FayInst 
		{ 
		public:
			ByteToObject() : FayInst(InstType::ByteToObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToFunction : public fay::FayInst 
		{ 
		public:
			ByteToFunction() : FayInst(InstType::ByteToFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToVoid : public fay::FayInst 
		{ 
		public:
			IntToVoid() : FayInst(InstType::IntToVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToBool : public fay::FayInst 
		{ 
		public:
			IntToBool() : FayInst(InstType::IntToBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToByte : public fay::FayInst 
		{ 
		public:
			IntToByte() : FayInst(InstType::IntToByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToInt : public fay::FayInst 
		{ 
		public:
			IntToInt() : FayInst(InstType::IntToInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToLong : public fay::FayInst 
		{ 
		public:
			IntToLong() : FayInst(InstType::IntToLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToFloat : public fay::FayInst 
		{ 
		public:
			IntToFloat() : FayInst(InstType::IntToFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToDouble : public fay::FayInst 
		{ 
		public:
			IntToDouble() : FayInst(InstType::IntToDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToString : public fay::FayInst 
		{ 
		public:
			IntToString() : FayInst(InstType::IntToString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToObject : public fay::FayInst 
		{ 
		public:
			IntToObject() : FayInst(InstType::IntToObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToFunction : public fay::FayInst 
		{ 
		public:
			IntToFunction() : FayInst(InstType::IntToFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToVoid : public fay::FayInst 
		{ 
		public:
			LongToVoid() : FayInst(InstType::LongToVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToBool : public fay::FayInst 
		{ 
		public:
			LongToBool() : FayInst(InstType::LongToBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToByte : public fay::FayInst 
		{ 
		public:
			LongToByte() : FayInst(InstType::LongToByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToInt : public fay::FayInst 
		{ 
		public:
			LongToInt() : FayInst(InstType::LongToInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToLong : public fay::FayInst 
		{ 
		public:
			LongToLong() : FayInst(InstType::LongToLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToFloat : public fay::FayInst 
		{ 
		public:
			LongToFloat() : FayInst(InstType::LongToFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToDouble : public fay::FayInst 
		{ 
		public:
			LongToDouble() : FayInst(InstType::LongToDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToString : public fay::FayInst 
		{ 
		public:
			LongToString() : FayInst(InstType::LongToString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToObject : public fay::FayInst 
		{ 
		public:
			LongToObject() : FayInst(InstType::LongToObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToFunction : public fay::FayInst 
		{ 
		public:
			LongToFunction() : FayInst(InstType::LongToFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToVoid : public fay::FayInst 
		{ 
		public:
			FloatToVoid() : FayInst(InstType::FloatToVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToBool : public fay::FayInst 
		{ 
		public:
			FloatToBool() : FayInst(InstType::FloatToBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToByte : public fay::FayInst 
		{ 
		public:
			FloatToByte() : FayInst(InstType::FloatToByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToInt : public fay::FayInst 
		{ 
		public:
			FloatToInt() : FayInst(InstType::FloatToInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToLong : public fay::FayInst 
		{ 
		public:
			FloatToLong() : FayInst(InstType::FloatToLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToFloat : public fay::FayInst 
		{ 
		public:
			FloatToFloat() : FayInst(InstType::FloatToFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToDouble : public fay::FayInst 
		{ 
		public:
			FloatToDouble() : FayInst(InstType::FloatToDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToString : public fay::FayInst 
		{ 
		public:
			FloatToString() : FayInst(InstType::FloatToString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToObject : public fay::FayInst 
		{ 
		public:
			FloatToObject() : FayInst(InstType::FloatToObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToFunction : public fay::FayInst 
		{ 
		public:
			FloatToFunction() : FayInst(InstType::FloatToFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToVoid : public fay::FayInst 
		{ 
		public:
			DoubleToVoid() : FayInst(InstType::DoubleToVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToBool : public fay::FayInst 
		{ 
		public:
			DoubleToBool() : FayInst(InstType::DoubleToBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToByte : public fay::FayInst 
		{ 
		public:
			DoubleToByte() : FayInst(InstType::DoubleToByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToInt : public fay::FayInst 
		{ 
		public:
			DoubleToInt() : FayInst(InstType::DoubleToInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToLong : public fay::FayInst 
		{ 
		public:
			DoubleToLong() : FayInst(InstType::DoubleToLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToFloat : public fay::FayInst 
		{ 
		public:
			DoubleToFloat() : FayInst(InstType::DoubleToFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToDouble : public fay::FayInst 
		{ 
		public:
			DoubleToDouble() : FayInst(InstType::DoubleToDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToString : public fay::FayInst 
		{ 
		public:
			DoubleToString() : FayInst(InstType::DoubleToString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToObject : public fay::FayInst 
		{ 
		public:
			DoubleToObject() : FayInst(InstType::DoubleToObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToFunction : public fay::FayInst 
		{ 
		public:
			DoubleToFunction() : FayInst(InstType::DoubleToFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToVoid : public fay::FayInst 
		{ 
		public:
			StringToVoid() : FayInst(InstType::StringToVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToBool : public fay::FayInst 
		{ 
		public:
			StringToBool() : FayInst(InstType::StringToBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToByte : public fay::FayInst 
		{ 
		public:
			StringToByte() : FayInst(InstType::StringToByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToInt : public fay::FayInst 
		{ 
		public:
			StringToInt() : FayInst(InstType::StringToInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToLong : public fay::FayInst 
		{ 
		public:
			StringToLong() : FayInst(InstType::StringToLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToFloat : public fay::FayInst 
		{ 
		public:
			StringToFloat() : FayInst(InstType::StringToFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToDouble : public fay::FayInst 
		{ 
		public:
			StringToDouble() : FayInst(InstType::StringToDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToString : public fay::FayInst 
		{ 
		public:
			StringToString() : FayInst(InstType::StringToString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToObject : public fay::FayInst 
		{ 
		public:
			StringToObject() : FayInst(InstType::StringToObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToFunction : public fay::FayInst 
		{ 
		public:
			StringToFunction() : FayInst(InstType::StringToFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToVoid : public fay::FayInst 
		{ 
		public:
			ObjectToVoid() : FayInst(InstType::ObjectToVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToBool : public fay::FayInst 
		{ 
		public:
			ObjectToBool() : FayInst(InstType::ObjectToBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToByte : public fay::FayInst 
		{ 
		public:
			ObjectToByte() : FayInst(InstType::ObjectToByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToInt : public fay::FayInst 
		{ 
		public:
			ObjectToInt() : FayInst(InstType::ObjectToInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToLong : public fay::FayInst 
		{ 
		public:
			ObjectToLong() : FayInst(InstType::ObjectToLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToFloat : public fay::FayInst 
		{ 
		public:
			ObjectToFloat() : FayInst(InstType::ObjectToFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToDouble : public fay::FayInst 
		{ 
		public:
			ObjectToDouble() : FayInst(InstType::ObjectToDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToString : public fay::FayInst 
		{ 
		public:
			ObjectToString() : FayInst(InstType::ObjectToString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToObject : public fay::FayInst 
		{ 
		public:
			ObjectToObject() : FayInst(InstType::ObjectToObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToFunction : public fay::FayInst 
		{ 
		public:
			ObjectToFunction() : FayInst(InstType::ObjectToFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToVoid : public fay::FayInst 
		{ 
		public:
			FunctionToVoid() : FayInst(InstType::FunctionToVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToBool : public fay::FayInst 
		{ 
		public:
			FunctionToBool() : FayInst(InstType::FunctionToBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToByte : public fay::FayInst 
		{ 
		public:
			FunctionToByte() : FayInst(InstType::FunctionToByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToInt : public fay::FayInst 
		{ 
		public:
			FunctionToInt() : FayInst(InstType::FunctionToInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToLong : public fay::FayInst 
		{ 
		public:
			FunctionToLong() : FayInst(InstType::FunctionToLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToFloat : public fay::FayInst 
		{ 
		public:
			FunctionToFloat() : FayInst(InstType::FunctionToFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToDouble : public fay::FayInst 
		{ 
		public:
			FunctionToDouble() : FayInst(InstType::FunctionToDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToString : public fay::FayInst 
		{ 
		public:
			FunctionToString() : FayInst(InstType::FunctionToString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToObject : public fay::FayInst 
		{ 
		public:
			FunctionToObject() : FayInst(InstType::FunctionToObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToFunction : public fay::FayInst 
		{ 
		public:
			FunctionToFunction() : FayInst(InstType::FunctionToFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MinusVoid : public fay::FayInst 
		{ 
		public:
			MinusVoid() : FayInst(InstType::MinusVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MinusBool : public fay::FayInst 
		{ 
		public:
			MinusBool() : FayInst(InstType::MinusBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MinusByte : public fay::FayInst 
		{ 
		public:
			MinusByte() : FayInst(InstType::MinusByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MinusInt : public fay::FayInst 
		{ 
		public:
			MinusInt() : FayInst(InstType::MinusInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MinusLong : public fay::FayInst 
		{ 
		public:
			MinusLong() : FayInst(InstType::MinusLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MinusFloat : public fay::FayInst 
		{ 
		public:
			MinusFloat() : FayInst(InstType::MinusFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MinusDouble : public fay::FayInst 
		{ 
		public:
			MinusDouble() : FayInst(InstType::MinusDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MinusString : public fay::FayInst 
		{ 
		public:
			MinusString() : FayInst(InstType::MinusString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MinusObject : public fay::FayInst 
		{ 
		public:
			MinusObject() : FayInst(InstType::MinusObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MinusFunction : public fay::FayInst 
		{ 
		public:
			MinusFunction() : FayInst(InstType::MinusFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PlusVoid : public fay::FayInst 
		{ 
		public:
			PlusVoid() : FayInst(InstType::PlusVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PlusBool : public fay::FayInst 
		{ 
		public:
			PlusBool() : FayInst(InstType::PlusBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PlusByte : public fay::FayInst 
		{ 
		public:
			int32_t val;
			PlusByte(int32_t val) : FayInst(InstType::PlusByte),val(val) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PlusInt : public fay::FayInst 
		{ 
		public:
			int32_t val;
			PlusInt(int32_t val) : FayInst(InstType::PlusInt),val(val) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PlusLong : public fay::FayInst 
		{ 
		public:
			int32_t val;
			PlusLong(int32_t val) : FayInst(InstType::PlusLong),val(val) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PlusFloat : public fay::FayInst 
		{ 
		public:
			int32_t val;
			PlusFloat(int32_t val) : FayInst(InstType::PlusFloat),val(val) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PlusDouble : public fay::FayInst 
		{ 
		public:
			int32_t val;
			PlusDouble(int32_t val) : FayInst(InstType::PlusDouble),val(val) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PlusString : public fay::FayInst 
		{ 
		public:
			PlusString() : FayInst(InstType::PlusString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PlusObject : public fay::FayInst 
		{ 
		public:
			PlusObject() : FayInst(InstType::PlusObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class PlusFunction : public fay::FayInst 
		{ 
		public:
			PlusFunction() : FayInst(InstType::PlusFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddVoid : public fay::FayInst 
		{ 
		public:
			AddVoid() : FayInst(InstType::AddVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddBool : public fay::FayInst 
		{ 
		public:
			AddBool() : FayInst(InstType::AddBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddByte : public fay::FayInst 
		{ 
		public:
			AddByte() : FayInst(InstType::AddByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddInt : public fay::FayInst 
		{ 
		public:
			AddInt() : FayInst(InstType::AddInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddLong : public fay::FayInst 
		{ 
		public:
			AddLong() : FayInst(InstType::AddLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddFloat : public fay::FayInst 
		{ 
		public:
			AddFloat() : FayInst(InstType::AddFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddDouble : public fay::FayInst 
		{ 
		public:
			AddDouble() : FayInst(InstType::AddDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddString : public fay::FayInst 
		{ 
		public:
			AddString() : FayInst(InstType::AddString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddObject : public fay::FayInst 
		{ 
		public:
			AddObject() : FayInst(InstType::AddObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddFunction : public fay::FayInst 
		{ 
		public:
			AddFunction() : FayInst(InstType::AddFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubVoid : public fay::FayInst 
		{ 
		public:
			SubVoid() : FayInst(InstType::SubVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubBool : public fay::FayInst 
		{ 
		public:
			SubBool() : FayInst(InstType::SubBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubByte : public fay::FayInst 
		{ 
		public:
			SubByte() : FayInst(InstType::SubByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubInt : public fay::FayInst 
		{ 
		public:
			SubInt() : FayInst(InstType::SubInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubLong : public fay::FayInst 
		{ 
		public:
			SubLong() : FayInst(InstType::SubLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubFloat : public fay::FayInst 
		{ 
		public:
			SubFloat() : FayInst(InstType::SubFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubDouble : public fay::FayInst 
		{ 
		public:
			SubDouble() : FayInst(InstType::SubDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubString : public fay::FayInst 
		{ 
		public:
			SubString() : FayInst(InstType::SubString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubObject : public fay::FayInst 
		{ 
		public:
			SubObject() : FayInst(InstType::SubObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubFunction : public fay::FayInst 
		{ 
		public:
			SubFunction() : FayInst(InstType::SubFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulVoid : public fay::FayInst 
		{ 
		public:
			MulVoid() : FayInst(InstType::MulVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulBool : public fay::FayInst 
		{ 
		public:
			MulBool() : FayInst(InstType::MulBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulByte : public fay::FayInst 
		{ 
		public:
			MulByte() : FayInst(InstType::MulByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulInt : public fay::FayInst 
		{ 
		public:
			MulInt() : FayInst(InstType::MulInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulLong : public fay::FayInst 
		{ 
		public:
			MulLong() : FayInst(InstType::MulLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulFloat : public fay::FayInst 
		{ 
		public:
			MulFloat() : FayInst(InstType::MulFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulDouble : public fay::FayInst 
		{ 
		public:
			MulDouble() : FayInst(InstType::MulDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulString : public fay::FayInst 
		{ 
		public:
			MulString() : FayInst(InstType::MulString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulObject : public fay::FayInst 
		{ 
		public:
			MulObject() : FayInst(InstType::MulObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulFunction : public fay::FayInst 
		{ 
		public:
			MulFunction() : FayInst(InstType::MulFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivVoid : public fay::FayInst 
		{ 
		public:
			DivVoid() : FayInst(InstType::DivVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivBool : public fay::FayInst 
		{ 
		public:
			DivBool() : FayInst(InstType::DivBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivByte : public fay::FayInst 
		{ 
		public:
			DivByte() : FayInst(InstType::DivByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivInt : public fay::FayInst 
		{ 
		public:
			DivInt() : FayInst(InstType::DivInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivLong : public fay::FayInst 
		{ 
		public:
			DivLong() : FayInst(InstType::DivLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivFloat : public fay::FayInst 
		{ 
		public:
			DivFloat() : FayInst(InstType::DivFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivDouble : public fay::FayInst 
		{ 
		public:
			DivDouble() : FayInst(InstType::DivDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivString : public fay::FayInst 
		{ 
		public:
			DivString() : FayInst(InstType::DivString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivObject : public fay::FayInst 
		{ 
		public:
			DivObject() : FayInst(InstType::DivObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivFunction : public fay::FayInst 
		{ 
		public:
			DivFunction() : FayInst(InstType::DivFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ModVoid : public fay::FayInst 
		{ 
		public:
			ModVoid() : FayInst(InstType::ModVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ModBool : public fay::FayInst 
		{ 
		public:
			ModBool() : FayInst(InstType::ModBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ModByte : public fay::FayInst 
		{ 
		public:
			ModByte() : FayInst(InstType::ModByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ModInt : public fay::FayInst 
		{ 
		public:
			ModInt() : FayInst(InstType::ModInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ModLong : public fay::FayInst 
		{ 
		public:
			ModLong() : FayInst(InstType::ModLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ModFloat : public fay::FayInst 
		{ 
		public:
			ModFloat() : FayInst(InstType::ModFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ModDouble : public fay::FayInst 
		{ 
		public:
			ModDouble() : FayInst(InstType::ModDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ModString : public fay::FayInst 
		{ 
		public:
			ModString() : FayInst(InstType::ModString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ModObject : public fay::FayInst 
		{ 
		public:
			ModObject() : FayInst(InstType::ModObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ModFunction : public fay::FayInst 
		{ 
		public:
			ModFunction() : FayInst(InstType::ModFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolNot : public fay::FayInst 
		{ 
		public:
			BoolNot() : FayInst(InstType::BoolNot) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolAnd : public fay::FayInst 
		{ 
		public:
			BoolAnd() : FayInst(InstType::BoolAnd) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolOr : public fay::FayInst 
		{ 
		public:
			BoolOr() : FayInst(InstType::BoolOr) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class EqualVoid : public fay::FayInst 
		{ 
		public:
			EqualVoid() : FayInst(InstType::EqualVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class EqualBool : public fay::FayInst 
		{ 
		public:
			EqualBool() : FayInst(InstType::EqualBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class EqualByte : public fay::FayInst 
		{ 
		public:
			EqualByte() : FayInst(InstType::EqualByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class EqualInt : public fay::FayInst 
		{ 
		public:
			EqualInt() : FayInst(InstType::EqualInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class EqualLong : public fay::FayInst 
		{ 
		public:
			EqualLong() : FayInst(InstType::EqualLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class EqualFloat : public fay::FayInst 
		{ 
		public:
			EqualFloat() : FayInst(InstType::EqualFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class EqualDouble : public fay::FayInst 
		{ 
		public:
			EqualDouble() : FayInst(InstType::EqualDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class EqualString : public fay::FayInst 
		{ 
		public:
			EqualString() : FayInst(InstType::EqualString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class EqualObject : public fay::FayInst 
		{ 
		public:
			EqualObject() : FayInst(InstType::EqualObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class EqualFunction : public fay::FayInst 
		{ 
		public:
			EqualFunction() : FayInst(InstType::EqualFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class NotEqualVoid : public fay::FayInst 
		{ 
		public:
			NotEqualVoid() : FayInst(InstType::NotEqualVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class NotEqualBool : public fay::FayInst 
		{ 
		public:
			NotEqualBool() : FayInst(InstType::NotEqualBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class NotEqualByte : public fay::FayInst 
		{ 
		public:
			NotEqualByte() : FayInst(InstType::NotEqualByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class NotEqualInt : public fay::FayInst 
		{ 
		public:
			NotEqualInt() : FayInst(InstType::NotEqualInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class NotEqualLong : public fay::FayInst 
		{ 
		public:
			NotEqualLong() : FayInst(InstType::NotEqualLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class NotEqualFloat : public fay::FayInst 
		{ 
		public:
			NotEqualFloat() : FayInst(InstType::NotEqualFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class NotEqualDouble : public fay::FayInst 
		{ 
		public:
			NotEqualDouble() : FayInst(InstType::NotEqualDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class NotEqualString : public fay::FayInst 
		{ 
		public:
			NotEqualString() : FayInst(InstType::NotEqualString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class NotEqualObject : public fay::FayInst 
		{ 
		public:
			NotEqualObject() : FayInst(InstType::NotEqualObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class NotEqualFunction : public fay::FayInst 
		{ 
		public:
			NotEqualFunction() : FayInst(InstType::NotEqualFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterVoid : public fay::FayInst 
		{ 
		public:
			GreaterVoid() : FayInst(InstType::GreaterVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterBool : public fay::FayInst 
		{ 
		public:
			GreaterBool() : FayInst(InstType::GreaterBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterByte : public fay::FayInst 
		{ 
		public:
			GreaterByte() : FayInst(InstType::GreaterByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterInt : public fay::FayInst 
		{ 
		public:
			GreaterInt() : FayInst(InstType::GreaterInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterLong : public fay::FayInst 
		{ 
		public:
			GreaterLong() : FayInst(InstType::GreaterLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterFloat : public fay::FayInst 
		{ 
		public:
			GreaterFloat() : FayInst(InstType::GreaterFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterDouble : public fay::FayInst 
		{ 
		public:
			GreaterDouble() : FayInst(InstType::GreaterDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterString : public fay::FayInst 
		{ 
		public:
			GreaterString() : FayInst(InstType::GreaterString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterObject : public fay::FayInst 
		{ 
		public:
			GreaterObject() : FayInst(InstType::GreaterObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterFunction : public fay::FayInst 
		{ 
		public:
			GreaterFunction() : FayInst(InstType::GreaterFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterEqualVoid : public fay::FayInst 
		{ 
		public:
			GreaterEqualVoid() : FayInst(InstType::GreaterEqualVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterEqualBool : public fay::FayInst 
		{ 
		public:
			GreaterEqualBool() : FayInst(InstType::GreaterEqualBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterEqualByte : public fay::FayInst 
		{ 
		public:
			GreaterEqualByte() : FayInst(InstType::GreaterEqualByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterEqualInt : public fay::FayInst 
		{ 
		public:
			GreaterEqualInt() : FayInst(InstType::GreaterEqualInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterEqualLong : public fay::FayInst 
		{ 
		public:
			GreaterEqualLong() : FayInst(InstType::GreaterEqualLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterEqualFloat : public fay::FayInst 
		{ 
		public:
			GreaterEqualFloat() : FayInst(InstType::GreaterEqualFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterEqualDouble : public fay::FayInst 
		{ 
		public:
			GreaterEqualDouble() : FayInst(InstType::GreaterEqualDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterEqualString : public fay::FayInst 
		{ 
		public:
			GreaterEqualString() : FayInst(InstType::GreaterEqualString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterEqualObject : public fay::FayInst 
		{ 
		public:
			GreaterEqualObject() : FayInst(InstType::GreaterEqualObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class GreaterEqualFunction : public fay::FayInst 
		{ 
		public:
			GreaterEqualFunction() : FayInst(InstType::GreaterEqualFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessVoid : public fay::FayInst 
		{ 
		public:
			LessVoid() : FayInst(InstType::LessVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessBool : public fay::FayInst 
		{ 
		public:
			LessBool() : FayInst(InstType::LessBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessByte : public fay::FayInst 
		{ 
		public:
			LessByte() : FayInst(InstType::LessByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessInt : public fay::FayInst 
		{ 
		public:
			LessInt() : FayInst(InstType::LessInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessLong : public fay::FayInst 
		{ 
		public:
			LessLong() : FayInst(InstType::LessLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessFloat : public fay::FayInst 
		{ 
		public:
			LessFloat() : FayInst(InstType::LessFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessDouble : public fay::FayInst 
		{ 
		public:
			LessDouble() : FayInst(InstType::LessDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessString : public fay::FayInst 
		{ 
		public:
			LessString() : FayInst(InstType::LessString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessObject : public fay::FayInst 
		{ 
		public:
			LessObject() : FayInst(InstType::LessObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessFunction : public fay::FayInst 
		{ 
		public:
			LessFunction() : FayInst(InstType::LessFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessEqualVoid : public fay::FayInst 
		{ 
		public:
			LessEqualVoid() : FayInst(InstType::LessEqualVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessEqualBool : public fay::FayInst 
		{ 
		public:
			LessEqualBool() : FayInst(InstType::LessEqualBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessEqualByte : public fay::FayInst 
		{ 
		public:
			LessEqualByte() : FayInst(InstType::LessEqualByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessEqualInt : public fay::FayInst 
		{ 
		public:
			LessEqualInt() : FayInst(InstType::LessEqualInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessEqualLong : public fay::FayInst 
		{ 
		public:
			LessEqualLong() : FayInst(InstType::LessEqualLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessEqualFloat : public fay::FayInst 
		{ 
		public:
			LessEqualFloat() : FayInst(InstType::LessEqualFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessEqualDouble : public fay::FayInst 
		{ 
		public:
			LessEqualDouble() : FayInst(InstType::LessEqualDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessEqualString : public fay::FayInst 
		{ 
		public:
			LessEqualString() : FayInst(InstType::LessEqualString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessEqualObject : public fay::FayInst 
		{ 
		public:
			LessEqualObject() : FayInst(InstType::LessEqualObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LessEqualFunction : public fay::FayInst 
		{ 
		public:
			LessEqualFunction() : FayInst(InstType::LessEqualFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitNotVoid : public fay::FayInst 
		{ 
		public:
			BitNotVoid() : FayInst(InstType::BitNotVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitNotBool : public fay::FayInst 
		{ 
		public:
			BitNotBool() : FayInst(InstType::BitNotBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitNotByte : public fay::FayInst 
		{ 
		public:
			BitNotByte() : FayInst(InstType::BitNotByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitNotInt : public fay::FayInst 
		{ 
		public:
			BitNotInt() : FayInst(InstType::BitNotInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitNotLong : public fay::FayInst 
		{ 
		public:
			BitNotLong() : FayInst(InstType::BitNotLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitNotFloat : public fay::FayInst 
		{ 
		public:
			BitNotFloat() : FayInst(InstType::BitNotFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitNotDouble : public fay::FayInst 
		{ 
		public:
			BitNotDouble() : FayInst(InstType::BitNotDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitNotString : public fay::FayInst 
		{ 
		public:
			BitNotString() : FayInst(InstType::BitNotString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitNotObject : public fay::FayInst 
		{ 
		public:
			BitNotObject() : FayInst(InstType::BitNotObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitNotFunction : public fay::FayInst 
		{ 
		public:
			BitNotFunction() : FayInst(InstType::BitNotFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitAndVoid : public fay::FayInst 
		{ 
		public:
			BitAndVoid() : FayInst(InstType::BitAndVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitAndBool : public fay::FayInst 
		{ 
		public:
			BitAndBool() : FayInst(InstType::BitAndBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitAndByte : public fay::FayInst 
		{ 
		public:
			BitAndByte() : FayInst(InstType::BitAndByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitAndInt : public fay::FayInst 
		{ 
		public:
			BitAndInt() : FayInst(InstType::BitAndInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitAndLong : public fay::FayInst 
		{ 
		public:
			BitAndLong() : FayInst(InstType::BitAndLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitAndFloat : public fay::FayInst 
		{ 
		public:
			BitAndFloat() : FayInst(InstType::BitAndFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitAndDouble : public fay::FayInst 
		{ 
		public:
			BitAndDouble() : FayInst(InstType::BitAndDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitAndString : public fay::FayInst 
		{ 
		public:
			BitAndString() : FayInst(InstType::BitAndString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitAndObject : public fay::FayInst 
		{ 
		public:
			BitAndObject() : FayInst(InstType::BitAndObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitAndFunction : public fay::FayInst 
		{ 
		public:
			BitAndFunction() : FayInst(InstType::BitAndFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitOrVoid : public fay::FayInst 
		{ 
		public:
			BitOrVoid() : FayInst(InstType::BitOrVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitOrBool : public fay::FayInst 
		{ 
		public:
			BitOrBool() : FayInst(InstType::BitOrBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitOrByte : public fay::FayInst 
		{ 
		public:
			BitOrByte() : FayInst(InstType::BitOrByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitOrInt : public fay::FayInst 
		{ 
		public:
			BitOrInt() : FayInst(InstType::BitOrInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitOrLong : public fay::FayInst 
		{ 
		public:
			BitOrLong() : FayInst(InstType::BitOrLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitOrFloat : public fay::FayInst 
		{ 
		public:
			BitOrFloat() : FayInst(InstType::BitOrFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitOrDouble : public fay::FayInst 
		{ 
		public:
			BitOrDouble() : FayInst(InstType::BitOrDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitOrString : public fay::FayInst 
		{ 
		public:
			BitOrString() : FayInst(InstType::BitOrString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitOrObject : public fay::FayInst 
		{ 
		public:
			BitOrObject() : FayInst(InstType::BitOrObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitOrFunction : public fay::FayInst 
		{ 
		public:
			BitOrFunction() : FayInst(InstType::BitOrFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitXorVoid : public fay::FayInst 
		{ 
		public:
			BitXorVoid() : FayInst(InstType::BitXorVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitXorBool : public fay::FayInst 
		{ 
		public:
			BitXorBool() : FayInst(InstType::BitXorBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitXorByte : public fay::FayInst 
		{ 
		public:
			BitXorByte() : FayInst(InstType::BitXorByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitXorInt : public fay::FayInst 
		{ 
		public:
			BitXorInt() : FayInst(InstType::BitXorInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitXorLong : public fay::FayInst 
		{ 
		public:
			BitXorLong() : FayInst(InstType::BitXorLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitXorFloat : public fay::FayInst 
		{ 
		public:
			BitXorFloat() : FayInst(InstType::BitXorFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitXorDouble : public fay::FayInst 
		{ 
		public:
			BitXorDouble() : FayInst(InstType::BitXorDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitXorString : public fay::FayInst 
		{ 
		public:
			BitXorString() : FayInst(InstType::BitXorString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitXorObject : public fay::FayInst 
		{ 
		public:
			BitXorObject() : FayInst(InstType::BitXorObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BitXorFunction : public fay::FayInst 
		{ 
		public:
			BitXorFunction() : FayInst(InstType::BitXorFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LShiftVoid : public fay::FayInst 
		{ 
		public:
			LShiftVoid() : FayInst(InstType::LShiftVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LShiftBool : public fay::FayInst 
		{ 
		public:
			LShiftBool() : FayInst(InstType::LShiftBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LShiftByte : public fay::FayInst 
		{ 
		public:
			LShiftByte() : FayInst(InstType::LShiftByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LShiftInt : public fay::FayInst 
		{ 
		public:
			LShiftInt() : FayInst(InstType::LShiftInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LShiftLong : public fay::FayInst 
		{ 
		public:
			LShiftLong() : FayInst(InstType::LShiftLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LShiftFloat : public fay::FayInst 
		{ 
		public:
			LShiftFloat() : FayInst(InstType::LShiftFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LShiftDouble : public fay::FayInst 
		{ 
		public:
			LShiftDouble() : FayInst(InstType::LShiftDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LShiftString : public fay::FayInst 
		{ 
		public:
			LShiftString() : FayInst(InstType::LShiftString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LShiftObject : public fay::FayInst 
		{ 
		public:
			LShiftObject() : FayInst(InstType::LShiftObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LShiftFunction : public fay::FayInst 
		{ 
		public:
			LShiftFunction() : FayInst(InstType::LShiftFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class RShiftVoid : public fay::FayInst 
		{ 
		public:
			RShiftVoid() : FayInst(InstType::RShiftVoid) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class RShiftBool : public fay::FayInst 
		{ 
		public:
			RShiftBool() : FayInst(InstType::RShiftBool) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class RShiftByte : public fay::FayInst 
		{ 
		public:
			RShiftByte() : FayInst(InstType::RShiftByte) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class RShiftInt : public fay::FayInst 
		{ 
		public:
			RShiftInt() : FayInst(InstType::RShiftInt) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class RShiftLong : public fay::FayInst 
		{ 
		public:
			RShiftLong() : FayInst(InstType::RShiftLong) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class RShiftFloat : public fay::FayInst 
		{ 
		public:
			RShiftFloat() : FayInst(InstType::RShiftFloat) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class RShiftDouble : public fay::FayInst 
		{ 
		public:
			RShiftDouble() : FayInst(InstType::RShiftDouble) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class RShiftString : public fay::FayInst 
		{ 
		public:
			RShiftString() : FayInst(InstType::RShiftString) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class RShiftObject : public fay::FayInst 
		{ 
		public:
			RShiftObject() : FayInst(InstType::RShiftObject) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class RShiftFunction : public fay::FayInst 
		{ 
		public:
			RShiftFunction() : FayInst(InstType::RShiftFunction) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class NewObject : public fay::FayInst 
		{ 
		public:
			std::string className;
			int32_t classIndex;
			NewObject(const std::string &className) : FayInst(InstType::NewObject),className(className) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		class NullObject : public fay::FayInst 
		{ 
		public:
			std::string className;
			int32_t classIndex;
			NullObject(const std::string &className) : FayInst(InstType::NullObject),className(className) {}
			virtual void buildString(mirror::utils::StringBuilder* sb) override;
		};
		
		//InstEnd
	}
}