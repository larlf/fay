#pragma once

#include <mirror_sys_const.h>
#include <mirror_data_buffer.h>
#include <fay_const.h>
#include <fay_object.h>

namespace fay
{
	/**
	 * 词法解析后的Token
	 */
	class Token
	{
	private:
		PTR(FayFile) _file;
		std::string _text;
		TokenType _type = TokenType::None;
		int _line = -1;
		int _col = -1;

	public:
		//Token(const std::string &text) : _text(text) {}
		Token() {}
		Token(PTR(FayFile) file, TokenType type, mirror::data::ByteData &data, int pos, int size, int line, int col)
			: _file(file), _type(type), _line(line), _col(col)
		{
			this->_text = std::string((char*)data.data(), pos, size);
		}

		TokenType type() { return this->_type; }
		const std::string &text() { return this->_text; }
		int line() { return this->_line; }
		int col() { return this->_col; }
		size_t size() { return this->_text.size(); }

		bool is(TokenType type) { return this->_type == type; }
		bool is(const std::string &text) { return this->_text == text; }

		std::string toString();
	};

	/**
	 * Token的生成规则
	 */
	class ITokenRule
	{
	protected:
		TokenType _class = TokenType::None;
		LexMode _mode = LexMode::Program;

	public:
		TokenType type() { return this->_class; }
		LexMode mode() { return this->_mode; }

		ITokenRule(LexMode mode, TokenType type)
			: _mode(mode), _class(type) {}
		virtual ~ITokenRule() {}

		//是否生成Token
		virtual Token* match(PTR(FayFile) file, mirror::data::ByteData &data, int pos, int line, int col) = 0;
	};

	/**
	* 字符常量
	*/
	class CharTokenRule : public ITokenRule
	{
	public:
		using ITokenRule::ITokenRule;

		// 通过 ITokenRule 继承
		virtual Token* match(PTR(FayFile) file, mirror::data::ByteData &data, int pos, int line, int col) override;

	};

	/**
	* 数字常量
	*/
	class NumberTokenRule : public ITokenRule
	{
	public:
		using ITokenRule::ITokenRule;
		// Inherited via ITokenRule
		virtual Token* match(PTR(FayFile) file, mirror::data::ByteData &data, int pos, int line, int col) override;
	};

	/**
	* 字符串常量
	*/
	class StringTokenRule : public ITokenRule
	{
	public:
		using ITokenRule::ITokenRule;

		// Inherited via ITokenRule
		virtual Token* match(PTR(FayFile) file, mirror::data::ByteData &data, int pos, int line, int col) override;
	};

	class SymbolTokenRule : public ITokenRule
	{
	private:
		char _value;

	public:
		SymbolTokenRule(LexMode mode, TokenType type, const char &c)
			: ITokenRule(mode, type), _value(c) {}

		// Inherited via ITokenRule
		virtual Token* match(PTR(FayFile) file, mirror::data::ByteData &data, int pos, int line, int col) override;
	};

	/**
	* 单个词的匹配规则
	*/
	class WordTokenRule : public ITokenRule
	{
	private:
		std::string _word;

	public:
		WordTokenRule(LexMode mode, TokenType type, const std::string &word)
			: ITokenRule(mode, type), _word(word) {}

		virtual Token* match(PTR(FayFile) file, mirror::data::ByteData &data, int pos, int line, int col) override;
	};

	/**
	* 多个词的匹配规则
	*/
	class WordsTokenRule : public ITokenRule
	{
	private:
		//std::vector<std::string> _words;
		std::vector<std::string> _words;

	public:
		WordsTokenRule(LexMode mode, TokenType type, std::vector<std::string> words)
			: ITokenRule(mode, type), _words(words) {}

		// Inherited via ITokenRule
		virtual Token* match(PTR(FayFile) file, mirror::data::ByteData &data, int pos, int line, int col) override;
	};

	/**
	* 变量名的匹配规则
	*/
	class IDTokenRule : public ITokenRule
	{
	public:
		using ITokenRule::ITokenRule;

		// Inherited via ITokenRule
		virtual Token* match(PTR(FayFile) file, mirror::data::ByteData &data, int pos, int line, int col) override;
	};

	/**
	* 单行注释的匹配规则
	*/
	class SingleCommentTokenRule : public ITokenRule
	{
	public:
		using ITokenRule::ITokenRule;

		// Inherited via ITokenRule
		virtual Token* match(PTR(FayFile) file, mirror::data::ByteData &data, int pos, int line, int col) override;

	};
}