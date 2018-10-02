#pragma once

#include <mirror_sys_const.h>
#include <mirror_data_buffer.h>
#include <fay_const.h>

using namespace mirror::data;
using namespace fay;

namespace fay
{
	/**
	 * 词法解析后的Token
	 */
	class Token
	{
	private:
		std::string _text;
		TokenType _type;
		int _line = -1;
		int _col = -1;

	public:
		Token(TokenType type, ByteData &data, int pos, int size, int line, int col)
		{
			this->_type = type;
			this->_text = std::string((char*)data.data(), pos, size);
			this->_line = line;
			this->_col = col;
		}

		TokenType type() { return this->_type; }
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
		TokenType _type = TokenType::None;
		LexMode _mode = LexMode::Program;

	public:
		TokenType type() { return this->_type; }
		LexMode mode() { return this->_mode; }

		ITokenRule(LexMode mode, TokenType type)
			: _mode(mode), _type(type) {}

		//是否生成Token
		virtual Token* match(ByteData &data, int pos, int line, int col) = 0;
	};

	/**
	* 字符常量
	*/
	class CharTokenRule : ITokenRule
	{
	public:
		using ITokenRule::ITokenRule;

		// 通过 ITokenRule 继承
		virtual Token* match(ByteData &data, int pos, int line, int col) override;

	};

	/**
	* 数字常量
	*/
	class NumberTokenRule : ITokenRule
	{
	public:
		using ITokenRule::ITokenRule;
		// Inherited via ITokenRule
		virtual Token* match(ByteData &data, int pos, int line, int col) override;
	};

	/**
	* 字符串常量
	*/
	class StringTokenRule : ITokenRule
	{
	public:
		using ITokenRule::ITokenRule;

		// Inherited via ITokenRule
		virtual Token* match(ByteData &data, int pos, int line, int col) override;
	};

	class SymbolTokenRule : ITokenRule
	{
	private:
		char _value;

	public:
		SymbolTokenRule(LexMode mode, TokenType type, const char &c)
			: ITokenRule(mode, type), _value(c) {}

		// Inherited via ITokenRule
		virtual Token* match(ByteData &data, int pos, int line, int col) override;
	};

	/**
	* 单个词的匹配规则
	*/
	class WordTokenRule : ITokenRule
	{
	private:
		std::string _word;

	public:
		WordTokenRule(LexMode mode, TokenType type, const std::string &word)
			: ITokenRule(mode, type), _word(word) {}

		virtual Token* match(ByteData &data, int pos, int line, int col) override;
	};

	/**
	* 多个词的匹配规则
	*/
	class WordsTokenRule : ITokenRule
	{
	private:
		//std::vector<std::string> _words;
		std::vector<std::string> _words;

	public:
		WordsTokenRule(LexMode mode, TokenType type, std::vector<std::string> words)
			: ITokenRule(mode, type), _words(words) {}

		// Inherited via ITokenRule
		virtual Token* match(ByteData &data, int pos, int line, int col) override;
	};

	/**
	* 变量名的匹配规则
	*/
	class IDTokenRule : ITokenRule
	{
	public:
		using ITokenRule::ITokenRule;

		// Inherited via ITokenRule
		virtual Token* match(ByteData &data, int pos, int line, int col) override;
	};

	/**
	* 单行注释的匹配规则
	*/
	class SingleCommentTokenRule : ITokenRule
	{
	public:
		using ITokenRule::ITokenRule;

		// Inherited via ITokenRule
		virtual Token * match(ByteData & data, int pos, int line, int col) override;

	};
}