﻿#pragma once

#include <fay_object.h>
#include <fay_const.h>

namespace fay
{
	//指令
	class FayInst : public FayObject
	{
	private:
		InstType _type;

	public:
		FayInst(InstType type) : _type(type) {}
		virtual ~FayInst() {}

		inline InstType type() { return this->_type; }

		virtual void toString(mirror::utils::StringBuilder* sb) { sb->endl(); }
	};

	class InstPushString : public FayInst
	{
	private:
		std::string _p1;
	public:
		InstPushString(const std::string &p1)
			: FayInst(InstType::PushString), _p1(p1) {}

		inline const std::string &p1() { return this->_p1; }

		virtual void toString(mirror::utils::StringBuilder* sb) override;

	};

	class InstCall : public FayInst
	{
	public:
		int32_t p1;
		int32_t p2;
		int32_t v1;
		int32_t v2;

		InstCall(pos_t p1, int32_t p2)
			: FayInst(InstType::Call), p1(p1), p2(p2) {}

		virtual void toString(mirror::utils::StringBuilder* sb) override;

	};
}