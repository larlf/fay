#if WIN32
#include <vld.h>
#endif
#include "test_utils.h"
#include "test_fay.h"
#include <mirror_utils_log.h>
#include <fay_lexer.h>

void main()
{
	try
	{
		test::UtilsTest::testLog();
		test::UtilsTest::testString();

		test::FayTests::Init();
		test::FayTests::TestLexer();
	}
	catch (const std::exception &e)
	{
		LOG_ERROR(e.what());
	}

}

