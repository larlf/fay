#if WIN32
#include <vld.h>
#endif
#include <test_utils.h>
#include <test_fay.h>
#include <mirror_utils_log.h>
#include <fay_lexer.h>
#include <fay_parser.h>

void main()
{
	try
	{
		//test::UtilsTest::testLog();
		//test::UtilsTest::testString();
		//test::UtilsTest::testTrace();

		test::FayTests::Init();
		test::FayTests::TestLexer();
		//test::FayTests::Test1();
		//test::FayTests::TestRTTI();
		//test::FayTests::TestInternalFun();
	}
	catch (fay::ParseException &e)
	{
		LOG_ERROR(e.what());
		PRINT(e.trace());
	}
	catch (fay::BuildException &e)
	{
		LOG_ERROR(e.what());
		PRINT(e.trace());
	}

}

