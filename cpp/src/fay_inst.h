#pragma once

#include <fay_object.h>

namespace fay
{
	class InstPushString : public FayInst
	{
	private:
		std::string _p1;
	public:
		InstPushString(const std::string &p1)
			: _p1(p1) {}

		virtual void toString(mirror::utils::StringBuilder* sb) override;

	};

	class InstCall : public FayInst
	{
	private:
		int32_t _p1;
		int32_t _p2;
	public:
		InstCall(pos_t p1, int32_t p2)
			: _p1(p1), _p2(p2) {}

		virtual void toString(mirror::utils::StringBuilder* sb) override;

	};
}