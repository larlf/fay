#pragma once

#include <mirror_sys_const.h>
#include <iostream>
#include <string>

namespace fay
{
	enum class I18nLang
	{
		EN = 1,
		CN = 2,
	};

	enum class I18n
	{
		//I18NCodeStart
		Location,
		Err_NoMain,
		Err_MultiMain,
		Err_Lexical,
		Err_AST,
		Err_Expect,
		Err_NoDefaultValue,
		Err_NotSupportOP,
		Err_MissRightValue,
		Err_NoClass,
		Err_NoFun,
		Err_RepeatedClass,
		Err_NoType,
		Err_UnknowType,
		Err_NoField,
		Err_NoStaticField,
		Err_NoVar,
		Err_CannotConvert,
		Err_CannotConvertFromVoid,
		Err_CannotConvertToVoid,
		Err_BadBranch,
		Err_NotNumberTyep,
		Err_NotIntType,
		Err_UnknowPerOP,
		Err_BadOP,
		Err_BadPreOP,
		Err_BadPostOP,
		Err_ReturnValue,
		Err_NoLeftValue,
		Err_NoRightValue,
		Err_BadFormat,
		Err_NoKeyword,
		Err_BadID,
		Err_BadTokenType,
		Err_UnknowKeyword,
		Err_NoFunBody,
		Err_UnknowStatment,
		Err_ExprError,
		Err_NoBody,
		Err_BadIndexValue,
		Err_BadType,
		Err_BadParam
		//I18NCodeEnd
	};

	class I18nBus
	{
	private:
		static MAP<I18n, std::string> _Dict;

	public:
		static void Init(I18nLang lang);

		template<typename... Params>
		static std::string Get(I18n key, Params... args)
		{
			std::vector<std::string> words{ args... };
			return I18nBus::Get(key, words);
		}

		static std::string Get(I18n key, std::vector<std::string> &words);
	};
}