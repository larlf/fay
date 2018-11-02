﻿#pragma once

#include <fay_object.h>
#include <fay_const.h>

namespace fay
{
	//指令
	class FayInst : public fay::FayObject
	{
	private:
		InstType _class;

	public:
		FayInst(InstType type) : _class(type) {}
		virtual ~FayInst() {}

		inline InstType type() { return this->_class; }

		virtual void toString(mirror::utils::StringBuilder* sb) { sb->endl(); }
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
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class Goto : public fay::FayInst 
		{ 
		public:
			int32_t target;
			Goto(int32_t target) : FayInst(InstType::Goto),target(target) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
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
		
		class VoidToVoid : public fay::FayInst 
		{ 
		public:
			VoidToVoid() : FayInst(InstType::VoidToVoid) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToBool : public fay::FayInst 
		{ 
		public:
			VoidToBool() : FayInst(InstType::VoidToBool) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToByte : public fay::FayInst 
		{ 
		public:
			VoidToByte() : FayInst(InstType::VoidToByte) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToInt : public fay::FayInst 
		{ 
		public:
			VoidToInt() : FayInst(InstType::VoidToInt) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToLong : public fay::FayInst 
		{ 
		public:
			VoidToLong() : FayInst(InstType::VoidToLong) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToFloat : public fay::FayInst 
		{ 
		public:
			VoidToFloat() : FayInst(InstType::VoidToFloat) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToDouble : public fay::FayInst 
		{ 
		public:
			VoidToDouble() : FayInst(InstType::VoidToDouble) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToString : public fay::FayInst 
		{ 
		public:
			VoidToString() : FayInst(InstType::VoidToString) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToObject : public fay::FayInst 
		{ 
		public:
			VoidToObject() : FayInst(InstType::VoidToObject) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class VoidToFunction : public fay::FayInst 
		{ 
		public:
			VoidToFunction() : FayInst(InstType::VoidToFunction) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToVoid : public fay::FayInst 
		{ 
		public:
			BoolToVoid() : FayInst(InstType::BoolToVoid) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToBool : public fay::FayInst 
		{ 
		public:
			BoolToBool() : FayInst(InstType::BoolToBool) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToByte : public fay::FayInst 
		{ 
		public:
			BoolToByte() : FayInst(InstType::BoolToByte) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToInt : public fay::FayInst 
		{ 
		public:
			BoolToInt() : FayInst(InstType::BoolToInt) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToLong : public fay::FayInst 
		{ 
		public:
			BoolToLong() : FayInst(InstType::BoolToLong) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToFloat : public fay::FayInst 
		{ 
		public:
			BoolToFloat() : FayInst(InstType::BoolToFloat) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToDouble : public fay::FayInst 
		{ 
		public:
			BoolToDouble() : FayInst(InstType::BoolToDouble) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToString : public fay::FayInst 
		{ 
		public:
			BoolToString() : FayInst(InstType::BoolToString) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToObject : public fay::FayInst 
		{ 
		public:
			BoolToObject() : FayInst(InstType::BoolToObject) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class BoolToFunction : public fay::FayInst 
		{ 
		public:
			BoolToFunction() : FayInst(InstType::BoolToFunction) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToVoid : public fay::FayInst 
		{ 
		public:
			ByteToVoid() : FayInst(InstType::ByteToVoid) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToBool : public fay::FayInst 
		{ 
		public:
			ByteToBool() : FayInst(InstType::ByteToBool) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToByte : public fay::FayInst 
		{ 
		public:
			ByteToByte() : FayInst(InstType::ByteToByte) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToInt : public fay::FayInst 
		{ 
		public:
			ByteToInt() : FayInst(InstType::ByteToInt) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToLong : public fay::FayInst 
		{ 
		public:
			ByteToLong() : FayInst(InstType::ByteToLong) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToFloat : public fay::FayInst 
		{ 
		public:
			ByteToFloat() : FayInst(InstType::ByteToFloat) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToDouble : public fay::FayInst 
		{ 
		public:
			ByteToDouble() : FayInst(InstType::ByteToDouble) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToString : public fay::FayInst 
		{ 
		public:
			ByteToString() : FayInst(InstType::ByteToString) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToObject : public fay::FayInst 
		{ 
		public:
			ByteToObject() : FayInst(InstType::ByteToObject) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ByteToFunction : public fay::FayInst 
		{ 
		public:
			ByteToFunction() : FayInst(InstType::ByteToFunction) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToVoid : public fay::FayInst 
		{ 
		public:
			IntToVoid() : FayInst(InstType::IntToVoid) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToBool : public fay::FayInst 
		{ 
		public:
			IntToBool() : FayInst(InstType::IntToBool) {}
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
		
		class IntToString : public fay::FayInst 
		{ 
		public:
			IntToString() : FayInst(InstType::IntToString) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToObject : public fay::FayInst 
		{ 
		public:
			IntToObject() : FayInst(InstType::IntToObject) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class IntToFunction : public fay::FayInst 
		{ 
		public:
			IntToFunction() : FayInst(InstType::IntToFunction) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToVoid : public fay::FayInst 
		{ 
		public:
			LongToVoid() : FayInst(InstType::LongToVoid) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToBool : public fay::FayInst 
		{ 
		public:
			LongToBool() : FayInst(InstType::LongToBool) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToByte : public fay::FayInst 
		{ 
		public:
			LongToByte() : FayInst(InstType::LongToByte) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToInt : public fay::FayInst 
		{ 
		public:
			LongToInt() : FayInst(InstType::LongToInt) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToLong : public fay::FayInst 
		{ 
		public:
			LongToLong() : FayInst(InstType::LongToLong) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToFloat : public fay::FayInst 
		{ 
		public:
			LongToFloat() : FayInst(InstType::LongToFloat) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToDouble : public fay::FayInst 
		{ 
		public:
			LongToDouble() : FayInst(InstType::LongToDouble) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToString : public fay::FayInst 
		{ 
		public:
			LongToString() : FayInst(InstType::LongToString) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToObject : public fay::FayInst 
		{ 
		public:
			LongToObject() : FayInst(InstType::LongToObject) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class LongToFunction : public fay::FayInst 
		{ 
		public:
			LongToFunction() : FayInst(InstType::LongToFunction) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToVoid : public fay::FayInst 
		{ 
		public:
			FloatToVoid() : FayInst(InstType::FloatToVoid) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToBool : public fay::FayInst 
		{ 
		public:
			FloatToBool() : FayInst(InstType::FloatToBool) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToByte : public fay::FayInst 
		{ 
		public:
			FloatToByte() : FayInst(InstType::FloatToByte) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToInt : public fay::FayInst 
		{ 
		public:
			FloatToInt() : FayInst(InstType::FloatToInt) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToLong : public fay::FayInst 
		{ 
		public:
			FloatToLong() : FayInst(InstType::FloatToLong) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToFloat : public fay::FayInst 
		{ 
		public:
			FloatToFloat() : FayInst(InstType::FloatToFloat) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToDouble : public fay::FayInst 
		{ 
		public:
			FloatToDouble() : FayInst(InstType::FloatToDouble) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToString : public fay::FayInst 
		{ 
		public:
			FloatToString() : FayInst(InstType::FloatToString) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToObject : public fay::FayInst 
		{ 
		public:
			FloatToObject() : FayInst(InstType::FloatToObject) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FloatToFunction : public fay::FayInst 
		{ 
		public:
			FloatToFunction() : FayInst(InstType::FloatToFunction) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToVoid : public fay::FayInst 
		{ 
		public:
			DoubleToVoid() : FayInst(InstType::DoubleToVoid) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToBool : public fay::FayInst 
		{ 
		public:
			DoubleToBool() : FayInst(InstType::DoubleToBool) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToByte : public fay::FayInst 
		{ 
		public:
			DoubleToByte() : FayInst(InstType::DoubleToByte) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToInt : public fay::FayInst 
		{ 
		public:
			DoubleToInt() : FayInst(InstType::DoubleToInt) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToLong : public fay::FayInst 
		{ 
		public:
			DoubleToLong() : FayInst(InstType::DoubleToLong) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToFloat : public fay::FayInst 
		{ 
		public:
			DoubleToFloat() : FayInst(InstType::DoubleToFloat) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToDouble : public fay::FayInst 
		{ 
		public:
			DoubleToDouble() : FayInst(InstType::DoubleToDouble) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToString : public fay::FayInst 
		{ 
		public:
			DoubleToString() : FayInst(InstType::DoubleToString) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToObject : public fay::FayInst 
		{ 
		public:
			DoubleToObject() : FayInst(InstType::DoubleToObject) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DoubleToFunction : public fay::FayInst 
		{ 
		public:
			DoubleToFunction() : FayInst(InstType::DoubleToFunction) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToVoid : public fay::FayInst 
		{ 
		public:
			StringToVoid() : FayInst(InstType::StringToVoid) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToBool : public fay::FayInst 
		{ 
		public:
			StringToBool() : FayInst(InstType::StringToBool) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToByte : public fay::FayInst 
		{ 
		public:
			StringToByte() : FayInst(InstType::StringToByte) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToInt : public fay::FayInst 
		{ 
		public:
			StringToInt() : FayInst(InstType::StringToInt) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToLong : public fay::FayInst 
		{ 
		public:
			StringToLong() : FayInst(InstType::StringToLong) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToFloat : public fay::FayInst 
		{ 
		public:
			StringToFloat() : FayInst(InstType::StringToFloat) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToDouble : public fay::FayInst 
		{ 
		public:
			StringToDouble() : FayInst(InstType::StringToDouble) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToString : public fay::FayInst 
		{ 
		public:
			StringToString() : FayInst(InstType::StringToString) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToObject : public fay::FayInst 
		{ 
		public:
			StringToObject() : FayInst(InstType::StringToObject) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class StringToFunction : public fay::FayInst 
		{ 
		public:
			StringToFunction() : FayInst(InstType::StringToFunction) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToVoid : public fay::FayInst 
		{ 
		public:
			ObjectToVoid() : FayInst(InstType::ObjectToVoid) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToBool : public fay::FayInst 
		{ 
		public:
			ObjectToBool() : FayInst(InstType::ObjectToBool) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToByte : public fay::FayInst 
		{ 
		public:
			ObjectToByte() : FayInst(InstType::ObjectToByte) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToInt : public fay::FayInst 
		{ 
		public:
			ObjectToInt() : FayInst(InstType::ObjectToInt) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToLong : public fay::FayInst 
		{ 
		public:
			ObjectToLong() : FayInst(InstType::ObjectToLong) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToFloat : public fay::FayInst 
		{ 
		public:
			ObjectToFloat() : FayInst(InstType::ObjectToFloat) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToDouble : public fay::FayInst 
		{ 
		public:
			ObjectToDouble() : FayInst(InstType::ObjectToDouble) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToString : public fay::FayInst 
		{ 
		public:
			ObjectToString() : FayInst(InstType::ObjectToString) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToObject : public fay::FayInst 
		{ 
		public:
			ObjectToObject() : FayInst(InstType::ObjectToObject) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class ObjectToFunction : public fay::FayInst 
		{ 
		public:
			ObjectToFunction() : FayInst(InstType::ObjectToFunction) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToVoid : public fay::FayInst 
		{ 
		public:
			FunctionToVoid() : FayInst(InstType::FunctionToVoid) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToBool : public fay::FayInst 
		{ 
		public:
			FunctionToBool() : FayInst(InstType::FunctionToBool) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToByte : public fay::FayInst 
		{ 
		public:
			FunctionToByte() : FayInst(InstType::FunctionToByte) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToInt : public fay::FayInst 
		{ 
		public:
			FunctionToInt() : FayInst(InstType::FunctionToInt) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToLong : public fay::FayInst 
		{ 
		public:
			FunctionToLong() : FayInst(InstType::FunctionToLong) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToFloat : public fay::FayInst 
		{ 
		public:
			FunctionToFloat() : FayInst(InstType::FunctionToFloat) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToDouble : public fay::FayInst 
		{ 
		public:
			FunctionToDouble() : FayInst(InstType::FunctionToDouble) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToString : public fay::FayInst 
		{ 
		public:
			FunctionToString() : FayInst(InstType::FunctionToString) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToObject : public fay::FayInst 
		{ 
		public:
			FunctionToObject() : FayInst(InstType::FunctionToObject) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class FunctionToFunction : public fay::FayInst 
		{ 
		public:
			FunctionToFunction() : FayInst(InstType::FunctionToFunction) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddVoid : public fay::FayInst 
		{ 
		public:
			AddVoid() : FayInst(InstType::AddVoid) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddBool : public fay::FayInst 
		{ 
		public:
			AddBool() : FayInst(InstType::AddBool) {}
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
		
		class AddString : public fay::FayInst 
		{ 
		public:
			AddString() : FayInst(InstType::AddString) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddObject : public fay::FayInst 
		{ 
		public:
			AddObject() : FayInst(InstType::AddObject) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class AddFunction : public fay::FayInst 
		{ 
		public:
			AddFunction() : FayInst(InstType::AddFunction) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubVoid : public fay::FayInst 
		{ 
		public:
			SubVoid() : FayInst(InstType::SubVoid) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubBool : public fay::FayInst 
		{ 
		public:
			SubBool() : FayInst(InstType::SubBool) {}
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
		
		class SubString : public fay::FayInst 
		{ 
		public:
			SubString() : FayInst(InstType::SubString) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubObject : public fay::FayInst 
		{ 
		public:
			SubObject() : FayInst(InstType::SubObject) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class SubFunction : public fay::FayInst 
		{ 
		public:
			SubFunction() : FayInst(InstType::SubFunction) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulVoid : public fay::FayInst 
		{ 
		public:
			MulVoid() : FayInst(InstType::MulVoid) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulBool : public fay::FayInst 
		{ 
		public:
			MulBool() : FayInst(InstType::MulBool) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulByte : public fay::FayInst 
		{ 
		public:
			MulByte() : FayInst(InstType::MulByte) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulInt : public fay::FayInst 
		{ 
		public:
			MulInt() : FayInst(InstType::MulInt) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulLong : public fay::FayInst 
		{ 
		public:
			MulLong() : FayInst(InstType::MulLong) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulFloat : public fay::FayInst 
		{ 
		public:
			MulFloat() : FayInst(InstType::MulFloat) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulDouble : public fay::FayInst 
		{ 
		public:
			MulDouble() : FayInst(InstType::MulDouble) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulString : public fay::FayInst 
		{ 
		public:
			MulString() : FayInst(InstType::MulString) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulObject : public fay::FayInst 
		{ 
		public:
			MulObject() : FayInst(InstType::MulObject) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class MulFunction : public fay::FayInst 
		{ 
		public:
			MulFunction() : FayInst(InstType::MulFunction) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivVoid : public fay::FayInst 
		{ 
		public:
			DivVoid() : FayInst(InstType::DivVoid) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivBool : public fay::FayInst 
		{ 
		public:
			DivBool() : FayInst(InstType::DivBool) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivByte : public fay::FayInst 
		{ 
		public:
			DivByte() : FayInst(InstType::DivByte) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivInt : public fay::FayInst 
		{ 
		public:
			DivInt() : FayInst(InstType::DivInt) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivLong : public fay::FayInst 
		{ 
		public:
			DivLong() : FayInst(InstType::DivLong) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivFloat : public fay::FayInst 
		{ 
		public:
			DivFloat() : FayInst(InstType::DivFloat) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivDouble : public fay::FayInst 
		{ 
		public:
			DivDouble() : FayInst(InstType::DivDouble) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivString : public fay::FayInst 
		{ 
		public:
			DivString() : FayInst(InstType::DivString) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivObject : public fay::FayInst 
		{ 
		public:
			DivObject() : FayInst(InstType::DivObject) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		class DivFunction : public fay::FayInst 
		{ 
		public:
			DivFunction() : FayInst(InstType::DivFunction) {}
			virtual void toString(mirror::utils::StringBuilder* sb) override;
		};
		
		//InstEnd
	}
}