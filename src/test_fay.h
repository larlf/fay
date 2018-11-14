#pragma once
#include <gtest/gtest.h>
#include <fay_type.h>

namespace test
{
	class TestValue // : public std::enable_shared_from_this<FayValue>
	{
	private:
		fay::ValueType type;
		fay::FayValueUnion val;
	};

	class FayLang : public testing::Test
	{
	public:
		static void SetUpTestCase();
		static fay::FayValue makeValue(const std::string &str);
	};

}


