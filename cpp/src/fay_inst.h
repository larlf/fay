#pragma once

namespace fay
{
	//指令
	class FayInst
	{

	};

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
		int32_t _p2;
	public:
		InstCall(const std::string &p1, int32_t p2)
			: _p1(p1), _p2(p2) {}

	};
}