#pragma once
#include <gtest/gtest.h>
#include <vector>
#include <filesystem>

namespace test
{
	class TestUtils : public testing::Test
	{
	public:
		static void findFiles(std::vector<std::string> &list, const std::experimental::filesystem::path &path);
	};
}