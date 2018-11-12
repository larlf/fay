#if WIN32
#include <vld.h>
#endif
#include <fay_const.h>
#include <gtest/gtest.h>
#include <test_utils.h>
#include <test_fay.h>

int main(int argc, char** argv)
{
	//VLDDisable();
	testing::FLAGS_gtest_death_test_style = "fast";
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

