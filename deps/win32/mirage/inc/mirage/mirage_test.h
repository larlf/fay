#pragma once

#include <mirage/mirage_utils.h>
#include <memory>
#include <functional>

namespace mirage
{
	/**
	 * 测试用例的运行环境<br>
	 * 用于对所有的测试用例进行注册、管理和执行
	 */
	class TestsRunTime
	{
	public:
		/// 所有的测试用例集合
		static unordered_map<string, function<void()>> TestCases;  

		/**
		 * 添加测试用例 
		 */
		static void AddTestCase(const string &name, function<void()> fun);

		/**
		 * 运行指定的测试用例 
		 */
		static void RunTestCase(const string &name);

		/**
		 * 运行指定前缀的测试用例
		 */
		static void RunTestsStartWith(const string &prefix);
	};

	/**
	 * 测试用例的判定类
	 */
	class TestAssert
	{
	public:
		template<typename T>
		static void EQ(const T a, const T b, const string &msg)
		{
			if (a != b)
			{
				LOG_ERROR("Assert Fail : " << a << " != " << b << (msg.size() ? (", " + msg) : ""))
			}
			else
			{
				if(msg.size())
					LOG_INFO("Assert Pass : " << msg)
				else
					LOG_INFO("Assert Pass : " << a << " == " << b)
			}
		}

		template<typename T>
		static void EQ(const T a, const T b)
		{
			EQ<T>(a, b, "");
		}
	};




}
