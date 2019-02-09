#pragma once

#include <string>
#include <mirror_sys_const.h>

namespace mirror
{
	namespace utils
	{
		class CmdlineParam
		{
		public:
			std::string name;
			std::string shortName;
			bool isSign = false;  //如果是标识后面没有参数
			std::string desc;

			CmdlineParam(const std::string &name, const std::string &shortName, bool isSign, const std::string &desc)
				: name(name), shortName(shortName), isSign(isSign), desc(desc) {}
		};

		class CmdlineParser
		{
		private:
			MAP<std::string, PTR(CmdlineParam)> paramsDefine;

		public:
			MAP<std::string, std::string> params;
			std::vector<std::string> cmds;

			void add(PTR(CmdlineParam) param);
			void parse(int argc, char** argv);
			void debug();
		};
	}
}