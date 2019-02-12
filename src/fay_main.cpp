#include <iostream>
#include <mirror_utils_log.h>
#include <mirror_utils_cmdline.h>
#include <fay_project.h>

using namespace mirror::utils;
using namespace fay;

int main(int argc, char** argv)
{
	mirror::utils::CmdlineParser parser;
	parser.add(MKPTR(CmdlineParam)("filename", "f", false, "Filename"));
	parser.add(MKPTR(CmdlineParam)("pipe", "", true, "Test param"));
	parser.parse(argc, argv);
	parser.debug();

	//没有可处理的文件
	if(parser.cmds.size() < 2)
		return 0;

	FayProject project(parser.cmds[1]);
	project.parse();
	project.build();

	return 0;
}
