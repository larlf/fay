#if WIN32
//#include <vld.h>
#endif
#include <gtest/gtest.h>
#include <test_utils.h>
#include <test_fay.h>

int main(int argc, char** argv)
{
	//VLDDisable();
	//testing::FLAGS_gtest_death_test_style = "fast";

	//testing::InitGoogleTest(&argc, argv);
	//return RUN_ALL_TESTS();

	test::TestFay::SetUpTestCase();
	auto type = FayDomain::FindClass("fay.dev.test.FlowTest");
	std::vector<PTR(FayFun)> funs;
	FayValue rs;
	funs = type->findFunByName("test7", true);
	PRINT(funs[0]->toString());
	rs = FayVM::Run(funs[0]);

	//return 0;
}

