#include <test_utils.h>
#include <mirror_utils_log.h>
#include <mirror_utils_lang.h>
#include <vector>

using namespace mirror;

void test::UtilsTest::testLog()
{
	std::string str = mirror::utils::TimeUtils::MSTimeString();
	std::cout << str << std::endl;

	PRINT("This is print");
	LOG_DEBUG("This is debug");
	LOG_INFO("This is info");
	LOG_WARN("This is warn");
	LOG_ERROR("This is error");
}

void test::UtilsTest::testString()
{
	std::string strs[] = {"ab","aaaaaaaa","bbbb","d"};

	std::vector<std::string> a;

	LOG_DEBUG(strs->size());

	int size= sizeof(strs) / sizeof(strs[0]);

	for (auto i = 0; i < size; ++i)
		LOG_DEBUG(strs[i]);

	std::string strs2[] = {"abc","aaa","ccc","eee"};
	auto s1 = utils::LangUtils::SizeOfArray(strs2);
	LOG_DEBUG("Size : " << s1);

	test::UtilsTest::testString(new std::string[3] { "aaa","bbb","ccc" });

	std::vector<std::string> s3 = {"aaa","bbb","ccc"};
}

void test::UtilsTest::testTrace()
{
	LOG_INFO("Test Trace");
	std::string stackMsg = mirror::log::SysTrace::TraceInfo();
	LOG_DEBUG(stackMsg);
}

void test::UtilsTest::testString(std::string* strs)
{
	int size = utils::LangUtils::SizeOfArray(*strs);
	LOG_DEBUG("Size : " << size);
	delete[] strs;
}


