#include "mirror_utils_cmdline.h"
#include "mirror_utils_cmdline.h"
#include <mirror_utils_log.h>

using namespace mirror::utils;

void mirror::utils::CmdlineParser::add(PTR(CmdlineParam) param)
{
	this->paramsDefine[param->name] = param;
}

void mirror::utils::CmdlineParser::parse(int argc, char** argv)
{
	PTR(CmdlineParam) param;  //��ǰ���ڴ���Ĳ���

	for(auto i = 0; i < argc; ++i)
	{
		string str(argv[i]);
		if(str.at(0) == '-')
		{
			str = str.substr(1);
			param.reset();

			for(auto it : this->paramsDefine)
			{
				if(it.second->name == str || it.second->shortName == str)
				{
					param = it.second;
					break;
				}
			}

			if(!param)
			{
				LOG_ERROR("Cannot find param : " << str);
				continue;
			}

			//������Ҫ���ݵĲ���
			if(param->isSign)
			{
				this->params[param->name] = "";
				param.reset();
			}
		}
		else if(param)
		{
			this->params[param->name] = str;
			param.reset();
		}
		else
			this->cmds.push_back(str);
	}

	//���ڷ�ֹ���һ������©����
	if(param)
		this->params[param->name] = "";
}

std::string mirror::utils::CmdlineParser::help()
{
	std::string str;

	return str;
}

void mirror::utils::CmdlineParser::debug()
{
	std::string str;

	for(auto it : this->cmds)
	{
		if(str.size() > 0)
			str.append(" ");

		str += it;
	}

	for(auto it : this->params)
	{
		str += "\n-";
		str += it.first;
		str += " : ";
		str += it.second;
	}

	PRINT(str);
}
