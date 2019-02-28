#include <fay_i18n.h>
#include <fay_log.h>
#include <jansson.h>
#include <mirror.h>

using namespace mirror;
using namespace fay;

MAP<I18n, std::string> fay::I18nBus::_Dict;

void fay::I18nBus::Init(I18nLang lang)
{
	switch(lang)
	{
		case I18nLang::EN:
			//I18nEnStart
			_Dict[I18n::Location] = "";
			_Dict[I18n::Err_NoMain] = "";
			_Dict[I18n::Err_MultiMain] = "";
			_Dict[I18n::Err_Lexical] = "";
			_Dict[I18n::Err_AST] = "";
			_Dict[I18n::Err_Expect] = "";
			_Dict[I18n::Err_NoDefaultValue] = "";
			_Dict[I18n::Err_NotSupportOP] = "";
			_Dict[I18n::Err_MissRightValue] = "";
			_Dict[I18n::Err_NoClass] = "";
			_Dict[I18n::Err_NoFun] = "";
			_Dict[I18n::Err_RepeatedClass] = "";
			_Dict[I18n::Err_NoType] = "";
			_Dict[I18n::Err_UnknowType] = "";
			_Dict[I18n::Err_NoField] = "";
			_Dict[I18n::Err_NoStaticField] = "";
			_Dict[I18n::Err_NoVar] = "";
			_Dict[I18n::Err_CannotConvert] = "";
			_Dict[I18n::Err_CannotConvertFromVoid] = "";
			_Dict[I18n::Err_CannotConvertToVoid] = "";
			_Dict[I18n::Err_BadBranch] = "";
			_Dict[I18n::Err_NotNumberTyep] = "";
			_Dict[I18n::Err_NotIntType] = "";
			_Dict[I18n::Err_UnknowPerOP] = "";
			_Dict[I18n::Err_BadOP] = "";
			_Dict[I18n::Err_BadPreOP] = "";
			_Dict[I18n::Err_BadPostOP] = "";
			_Dict[I18n::Err_ReturnValue] = "";
			_Dict[I18n::Err_NoLeftValue] = "";
			_Dict[I18n::Err_NoRightValue] = "";
			_Dict[I18n::Err_BadFormat] = "";
			_Dict[I18n::Err_NoKeyword] = "";
			_Dict[I18n::Err_BadID] = "";
			_Dict[I18n::Err_BadTokenType] = "";
			_Dict[I18n::Err_UnknowKeyword] = "";
			_Dict[I18n::Err_NoFunBody] = "";
			_Dict[I18n::Err_UnknowStatment] = "";
			_Dict[I18n::Err_ExprError] = "";
			_Dict[I18n::Err_NoBody] = "";
			_Dict[I18n::Err_BadIndexValue] = "";
			_Dict[I18n::Err_BadType] = "";
			_Dict[I18n::Err_BadParam] = "";
			//I18nEnEnd
			break;
		case I18nLang::CN:
			//I18nCnStart
			_Dict[I18n::Location] = "文件：{1}，第{2}行，第{3}列。";
			_Dict[I18n::Err_NoMain] = "找不到Main()函数。";
			_Dict[I18n::Err_MultiMain] = "存在多个Main()函数：{1}。";
			_Dict[I18n::Err_Lexical] = "词法解析错误。";
			_Dict[I18n::Err_AST] = "语法解析错误。";
			_Dict[I18n::Err_Expect] = "找不到预期的{1}。";
			_Dict[I18n::Err_NoDefaultValue] = "无法生成类型 {1} 的默认值";
			_Dict[I18n::Err_NotSupportOP] = "不支持的操作，操作符：{1}，操作类型：{2}";
			_Dict[I18n::Err_MissRightValue] = "找不到右值。";
			_Dict[I18n::Err_NoClass] = "找不到类{1}。";
			_Dict[I18n::Err_NoFun] = "找不到函数{1}。";
			_Dict[I18n::Err_RepeatedClass] = "重复的Class：{1}。";
			_Dict[I18n::Err_NoType] = "没有指定类型。";
			_Dict[I18n::Err_UnknowType] = "未知的数据类型{1}。";
			_Dict[I18n::Err_NoField] = "未知的字段{2}在类型{1}中。";
			_Dict[I18n::Err_NoStaticField] = "未知的静态字段{2}在类型{1}中。";
			_Dict[I18n::Err_NoVar] = "未知的变量{1}。";
			_Dict[I18n::Err_CannotConvert] = "无法从{1}转换到{2}。";
			_Dict[I18n::Err_CannotConvertFromVoid] = "无法从void转换到其它类型。";
			_Dict[I18n::Err_CannotConvertToVoid] = "无法从其它类型转换到void。";
			_Dict[I18n::Err_BadBranch] = "错误的分支";
			_Dict[I18n::Err_NotNumberTyep] = "{1}不是数字类型";
			_Dict[I18n::Err_NotIntType] = "不是整数类型";
			_Dict[I18n::Err_UnknowPerOP] = "未知的前置操作符{1}";
			_Dict[I18n::Err_BadOP] = "错误的操作，类型{1}，操作{2}。";
			_Dict[I18n::Err_BadPreOP] = "错误的前置操作{1}";
			_Dict[I18n::Err_BadPostOP] = "错误的后置操作{1}";
			_Dict[I18n::Err_ReturnValue] = "返回值错误";
			_Dict[I18n::Err_NoLeftValue] = "没有左值";
			_Dict[I18n::Err_NoRightValue] = "没有右值";
			_Dict[I18n::Err_BadFormat] = "{1}格式错误";
			_Dict[I18n::Err_NoKeyword] = "没有关键字{1}";
			_Dict[I18n::Err_BadID] = "错误的ID命名";
			_Dict[I18n::Err_BadTokenType] = "错误的Token类型{1}";
			_Dict[I18n::Err_UnknowKeyword] = "未知的关键字{1}";
			_Dict[I18n::Err_NoFunBody] = "找不到函数实现";
			_Dict[I18n::Err_UnknowStatment] = "未知语句";
			_Dict[I18n::Err_ExprError] = "表达式错误";
			_Dict[I18n::Err_NoBody] = "找不到语句体";
			_Dict[I18n::Err_BadIndexValue] = "索引值错误";
			_Dict[I18n::Err_BadType] = "错误的类型";
			_Dict[I18n::Err_BadParam] = "错误的参数";
			//I18nCnEnd
			break;
	}
}

std::string fay::I18nBus::Get(I18n key, std::vector<std::string> &words)
{
	auto it = _Dict.find(key);
	if(it == _Dict.end())
	{
		//LOG_ERROR("Cannot find i18n data : " << key);
		std::string str;
		str.append(std::to_string((int)key));
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
