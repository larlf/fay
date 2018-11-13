#pragma once
#include <gtest/gtest.h>
#include <fay_type.h>

namespace test
{
	class FayLang : public testing::Test
	{
	public:
		static void SetUpTestCase();
		static fay::FayValue makeValue();
	};

}


