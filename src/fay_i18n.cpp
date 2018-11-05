#include <fay_i18n.h>
#include <jansson.h>

MAP<std::string, std::string> fay::I18N::_dict;

void fay::I18N::Init(const std::string &text)
{
	json_error_t error;
	PTR(json_t) root(json_loads(text.c_str(), 0, &error));


}
