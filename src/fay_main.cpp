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

	//编译项目
	FayProject project(parser.get("path"));
	project.build();

	//编译没有错误才会运行代码
	if(LogBus::Count(LogType::Error) <= 0)
	{
		//找到Main()方法并执行
		PTR(std::vector<PTR(FayFun)>) mainFuns = project.lib()->findMainFun();
		if(mainFuns->empty())
			LogBus::Error(I18N::Get("err.no_main"));
		else if(mainFuns->size() > 1)
		{
			//显示找到的所有方法名
			std::string str = "";
			for(const auto &it : *mainFuns)
			{
				if(!str.empty())
					str.append(", ");
				str.append(it->fullname());
			}

			LogBus::Error(I18N::Get("err.multi_main", str));
		}
		else
			FayVM::Run((*mainFuns)[0]);
	}

	return 0;
}
