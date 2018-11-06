#if WIN32
#include <vld.h>
#endif
#include <test_utils.h>
#include <test_fay.h>
#include <mirror_utils_log.h>
#include <fay_lexer.h>
#include <fay_parser.h>
#include <gtest/gtest.h>

int main(int argc, char** argv)
{

	VLDDisable();
	testing::FLAGS_gtest_death_test_style = "fast";

	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();

	//try
	//{
	//	//test::UtilsTest::testLog();
	//	//test::UtilsTest::testString();
	//	//test::UtilsTest::testTrace();

	//	test::FayTests::Init();
	//	test::FayTests::TestLexer();
	//	//test::FayTests::Test1();
	//	//test::FayTests::TestRTTI();
	//	//test::FayTests::TestInternalFun();
	//}
	//catch (fay::FayCompileException &e)
	//{
	//	PRINT("Error : " << e.what());
	//	PRINT("Line:" << e.line() << " Col:" << e.col() << " @ " << e.file()->filename());
	//	PRINT(e.source());
	//	PRINT(e.trace());
	//}

}

