#pragma once

#include <iostream>

namespace test
{
	class TestA
	{
	public:
		TestA()
		{
			std::cout << "Create TestA" << std::endl;
		}

		~TestA()
		{
			std::cout << "Destory TestA" << std::endl;
		}

		void fun1()
		{
			std::cout << "This is fun1()" << std::endl;
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
	};
}
