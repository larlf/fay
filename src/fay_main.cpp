#include <iostream>
#include <mirror_utils_log.h>
#include <mirror_utils_cmdline.h>
#include <cmdline.h>

using namespace mirror::utils;

int main(int argc, char** argv)
{
	mirror::utils::CmdlineParser parser;
	parser.addParam(MKPTR(CmdlineParam)("filename", "f", false, "Filename"));
	parser.addParam(MKPTR(CmdlineParam)("pipe", "", true, "Test param"));
	parser.parse(argc, argv);
	parser.debug();

	return 0;
}
