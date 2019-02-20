#pragma once
#include <gtest/gtest.h>
#include <fay.h>

using namespace fay;

namespace test
{
	//用于测试的类型
	class TestClass
	{
	public:
		std::string name;

		TestClass(const std::string &name) : name(name) {}
	};

	class TestValue // : public std::enable_shared_from_this<FayValue>
	{
	private:
		fay::ValueType type;
		fay::FayValueUnion val;
	};

	class TestFay : public testing::Test
	{
	public:
		static PTR(FayProject) project;

		static void SetUpTestCase();
		static fay::FayValue makeValue(const std::string &str);
	};

}


