#pragma once

#include <fay_object.h>
#include <fay_lang.h>

namespace fay
{
	//������
	class FayCompileResult
	{
	public:
		void warn(PTR(FayPart) part, const std::string &msg);
		void error(PTR(FayPart) part, const std::string &msg);
	};

	//�������񣬶��������ܻ᷵�ص�һ����������
	class FayCompileTask
	{

	};
}