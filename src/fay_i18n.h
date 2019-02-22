#pragma once

#include <mirror_sys_const.h>
#include <iostream>
#include <string>

namespace fay
{
	class I18N
	{
	private:
		static MAP<std::string, std::string> _Dict;

	public:
		static void Init(const std::string &filename);

		template<typename... Params>
		static std::string Get(const std::string &key, Params... args)
		{
			std::vector<std::string> words{ args... };
			return I18N::Get(key, words);
		}

		static std::string Get(const std::string &key, std::vector<std::string> &words);
	};
}