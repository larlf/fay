#pragma once

#include <iostream>

namespace test
{
	class TestA
	{
	private:
		std::string str;

	public:
		TestA(const std::string &str) : str(str)
		{
			std::cout << "Create TestA" << std::endl;
		}

		TestA(const TestA& value)
		{
			std::cout << "Copy Me" << std::endl;
			this->str = value.str;
		}

		~TestA()
		{
			std::cout << "Destory TestA" << std::endl;
		}

		void fun1()
		{
			std::cout << this->str.c_str() << std::endl;
		}
	};

	class ClassA
	{
	};

	class ClassB : public ClassA
	{

	};

	class ClassC : public ClassB
	{

	};

	class FayTests
	{
	public:
		static void Init();
		static void TestLexer();
		static void Test1();
		static void TestRTTI();
		static void TestInternalFun();
	};
}
