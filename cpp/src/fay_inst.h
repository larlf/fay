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
	};

	class InstCall : public FayInst
	{
	private:
		std::string _p1;
		std::string _p2;
		int32_t _p3;
	public:
		InstCall(const std::string &p1, const std::string &p2, int32_t p3)
			: _p1(p1), _p2(p2), _p3(p3) {}

	};
}