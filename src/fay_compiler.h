#pragma once

#include <fay_object.h>
#include <fay_lang.h>

namespace fay
{
	//������
	class FayCompileResult
	{
	public:
		void warn(PTR(FilePart) part, const std::string &msg);
		void error(PTR(FilePart) part, const std::string &msg);
	};

	//�������񣬶��������ܻ᷵�ص�һ����������
	class FayCompileTask
	{

	};
}