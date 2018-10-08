#pragma once
#include <iostream>

namespace test
{
	class UtilsTest
	{
	public:
		static void testLog();
		static void testString();
		static void testString(std::string* strs);
		static void testTrace();
	};
}