#include "mirror_utils_cmdline.h"
#include "mirror_utils_cmdline.h"
#include <mirror_utils_log.h>

using namespace mirror::utils;

size_t mirror::utils::CmdlineParser::getParamIndex(const std::string & name)
{
	for (auto i = 0; i < this->paramDefs.size(); ++i)
	{
		if (this->paramDefs[i]->name == name)
			return i + 1;
	}

	return 0;
}

void mirror::utils::CmdlineParser::set(PTR(CmdlineOptionDef) param)
{
	this->optionDefs[param->name] = param;
}

void mirror::utils::CmdlineParser::set(PTR(CmdlineParamDef) def)
{
	this->paramDefs.push_back(def);
}

void mirror::utils::CmdlineParser::parse(int argc, char** argv)
{
	PTR(CmdlineOptionDef) param;  //当前正在处理的参数

	for (auto i = 0; i < argc; ++i)
	{
		string str(argv[i]);
		if (str.at(0) == '-')
		{
			str = str.substr(1);
			param.reset();

			for (auto it : this->optionDefs)
			{
				if (it.second->name == str || it.second->shortName == str)
				{
					param = it.second;
					break;
				}
			}

			if (!param)
			{
				LOG_ERROR("Cannot find param : " << str);
				continue;
			}

			//处理不需要内容的参数
			if (param->isSign)
			{
				this->options[param->name] = "";
				param.reset();
			}
		}
		else if (param)
		{
			this->options[param->name] = str;
			param.reset();
		}
		else
			this->params.push_back(str);
	}

	//用于防止最后一个参数漏处理
	if (param)
		this->options[param->name] = "";
}

std::string mirror::utils::CmdlineParser::get(const std::string & name)
{
	//从选项中找
	auto it = this->options.find(name);
	if (it != this->options.end())
		return it->second;

	//从参数中找
	auto index = this->getParamIndex(name);
	if (index > 0)
		return this->get(index);

	return "";
}

std::string mirror::utils::CmdlineParser::get(size_t index)
{
	if (index < this->params.size())
		return this->params[index];

	return "";
}

std::string mirror::utils::CmdlineParser::help()
{
	std::string str = "Format : " + this->params[0];

	for (auto it : this->paramDefs)
	{
		str.append(" <").append(it->name).append(">");
	}

	for (auto it : this->paramDefs)
	{
		str.append("\n\t").append(it->name).append(" : ").append(it->desc);
	}

	if (this->optionDefs.size() > 0)
	{
		str.append("\nOptions:");

		for (auto it : this->optionDefs)
		{
			str.append("\n\t-").append(it.second->name);
			if (it.second->shortName.size() > 0)
			{
				str.append(", -").append(it.second->shortName);
			}
			str.append(" : ").append(it.second->desc);
		}
	}

	return str;
}

void mirror::utils::CmdlineParser::debug()
{
	std::string str;

	for (auto it : this->params)
	{
		if (str.size() > 0)
			str.append(" ");

		str += it;
	}

	for (auto it : this->options)
	{
		str += "\n-";
		str += it.first;
		str += " : ";
		str += it.second;
	}

	PRINT(str);
}
