#include <iostream>
#include <mirror_utils_log.h>
#include <mirror_utils_cmdline.h>
#include <fay_project.h>

using namespace mirror::utils;
using namespace fay;

int main(int argc, char** argv)
{
	mirror::utils::CmdlineParser parser;
	parser.set(MKPTR(CmdlineParamDef)("path", "Project's path"));
	parser.set(MKPTR(CmdlineOptionDef)("filename", "f", false, "Filename"));
	parser.set(MKPTR(CmdlineOptionDef)("test", "", true, "Test param"));
	parser.parse(argc, argv);
	//parser.debug();

	//没有可处理的文件
	if(parser.paramCount() < 1)
	{
		PRINT(parser.help());
		return 0;
	}

	FayProject project(parser.get("path"));
	//project.parse();
	project.build();

	return 0;
}
