#pragma once

#include <string>
#include <mirror_sys_const.h>

namespace mirror
{
	namespace utils
	{
		//命令行中的选项定义
		class CmdlineOptionDef
		{
		public:
			std::string name;
			std::string shortName;
			bool isSign = false;  //如果是标识后面没有参数
			std::string desc;

			//name : 选项的名称
			//shortName : 选项的短名称
			//isSign : 是否是标志，如果是标志的话后面不跟参数
			//desc : 说明信息
			CmdlineOptionDef(const std::string &name, const std::string &shortName, bool isSign, const std::string &desc)
				: name(name), shortName(shortName), isSign(isSign), desc(desc) {}
		};

		//命令行中的参数定义
		class CmdlineParamDef
		{
		public:
			std::string name;
			std::string desc;

			//name : 参数的名称
			//desc : 参数的说明信息
			CmdlineParamDef(const std::string &name, const std::string &desc)
				: name(name), desc(desc) {}
		};

		//命令行的解析器
		//COMMAND OPTIONS PARAMETERS
		class CmdlineParser
		{
		private:
			//选项定义
			MAP<std::string, PTR(CmdlineOptionDef)> optionDefs;
			//参数定义
			std::vector<PTR(CmdlineParamDef)> paramDefs;
			//选项值
			MAP<std::string, std::string> options;
			//参数值
			std::vector<std::string> params;

			//取得参数的序列，没有找到返回0
			size_t getParamIndex(const std::string &name);

		public:
			//参数的数量
			size_t paramCount() { return this->params.size() - 1; }

			//设置选项格式
			void set(PTR(CmdlineOptionDef) param);
			//设置参数格式
			void set(PTR(CmdlineParamDef) def);
			//对命令进行解析
			void parse(int argc, char** argv);
			//根据名称取得值
			std::string get(const std::string &name);
			//根据参数的位置取得值
			//注意，参数值是从1开始的
			std::string get(size_t index);
			//输出帮助信息
			std::string help();
			//输出调试信息
			void debug();
		};
	}
}