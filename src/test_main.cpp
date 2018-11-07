#if WIN32
#include <vld.h>
#endif
#include <test_main.h>
#include <fay_const.h>
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
	//VLDDisable();

	fay::TypeDict::Init();

	testing::FLAGS_gtest_death_test_style = "fast";
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

