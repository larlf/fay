#include <fay_i18n.h>
#include <fay_log.h>
#include <jansson.h>
#include <mirror.h>

using namespace mirror;

MAP<std::string, std::string> fay::I18N::_Dict;

void fay::I18N::Init(const std::string &filename)
{
	//读取国际化信息文件
	std::string text = utils::FileUtils::ReadTextFile(filename);
	if(text.empty())
	{
		LogBus::Error(TOSTR("Bad i18n config : " << filename));
		exit(0);
	}

	json_error_t error;
	json_t* root = json_loads(text.c_str(), 0, &error);

	if(!root)
	{
		LOG_ERROR("Parse i18n data error at line:" << error.line << std::endl << error.text);
		return;
	}

	if(!json_is_object(root))
	{
		LOG_ERROR("Bad i18n data format");
		return;
	}

	const char* key;
	json_t* value;
	json_object_foreach(root, key, value)
	{
		if(!json_is_string(value))
		{
			LOG_ERROR("I18n data is not a string : " << key);
			continue;
		}

		std::string keyStr(key);
		std::string valueStr(json_string_value(value));

		_Dict[keyStr] = valueStr;
	}

	json_delete(root);
}

std::string fay::I18N::Get(const std::string &key, std::vector<std::string> &words)
{
	auto it = _Dict.find(key);
	if(it == _Dict.end())
	{
		//LOG_ERROR("Cannot find i18n data : " << key);
		std::string str;
		str.append(key);
		for each(auto it2 in words)
		{
			str.append(",");
			str.append(it2);
		}
		return str;
	}
	else
	{
		std::string str = it->second;
		for(int i = 0; i < words.size(); ++i)
			str = utils::StringUtils::ReplaceAll(str, "{" + std::to_string(i + 1) + "}", words[i]);
		return str;
	}
}
