#include <mirror.h>
#include <fay.h>

using namespace mirror::utils;
using namespace fay;

int main(int argc, char** argv)
{
	mirror::utils::CmdlineParser parser;
	parser.set(MKPTR(CmdlineParamDef)("path", "Project's path"));
	parser.set(MKPTR(CmdlineOptionDef)("output", "o", false, "Output file name"));
	parser.set(MKPTR(CmdlineOptionDef)("log-file", "lf", false, "Log filename"));
	parser.set(MKPTR(CmdlineOptionDef)("log-level", "ll", false, "Log level (debug|info|warn|error)"));
	parser.parse(argc, argv);
	//parser.debug();

	//Test
	LogBus::Init("build.log");

	//没有可处理的文件
	if(parser.paramCount() < 1)
	{
		LogBus::Error(parser.help());
		return 0;
	}

	fay::I18N::Init("doc/i18n.cn.json");
	SystemEnv::Init();
	TypeDict::Init();
	FayDomain::InitSysLib();

	FayProject project(parser.get("path"));
	//project.parse();
	project.build2();

	return 0;
}
