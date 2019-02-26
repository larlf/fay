#pragma once
#include <mirror.h>
#include <fay_const.h>

namespace fay
{
	/**
	 * 词法解析后的Token
	 */
	class Token
	{
	public:
		TokenType type = TokenType::None;
		std::string text;
		int line = -1;
		int col = -1;
		int count = 0;

		Token() {}
		Token(TokenType type, mirror::data::ByteData &data, int pos, int count, int line, int col)
			:  type(type), line(line), col(col), count(count)
		{
			this->text = std::string((char*)data.data(), pos, count);
		}

		bool is(TokenType type) { return this->type == type; }
		bool is(const std::string &text) { return this->text == text; }

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
		virtual Token* match(mirror::data::ByteData &data, int pos, int line, int col) = 0;
	};

	/**
	* 字符常量
	*/
	class CharTokenRule : public ITokenRule
	{
	public:
		using ITokenRule::ITokenRule;

		// 通过 ITokenRule 继承
		virtual Token* match(mirror::data::ByteData &data, int pos, int line, int col) override;

	};

	/**
	* 数字常量
	*/
	class NumberTokenRule : public ITokenRule
	{
	public:
		using ITokenRule::ITokenRule;
		// Inherited via ITokenRule
		virtual Token* match(mirror::data::ByteData &data, int pos, int line, int col) override;
	};

	/**
	* 字符串常量
	*/
	class StringTokenRule : public ITokenRule
	{
	public:
		using ITokenRule::ITokenRule;

		// Inherited via ITokenRule
		virtual Token* match(mirror::data::ByteData &data, int pos, int line, int col) override;
	};

	class SymbolTokenRule : public ITokenRule
	{
	private:
		char _value;

	public:
		SymbolTokenRule(LexMode mode, TokenType type, const char &c)
			: ITokenRule(mode, type), _value(c) {}

		// Inherited via ITokenRule
		virtual Token* match(mirror::data::ByteData &data, int pos, int line, int col) override;
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

		virtual Token* match(mirror::data::ByteData &data, int pos, int line, int col) override;
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
		virtual Token* match(mirror::data::ByteData &data, int pos, int line, int col) override;
	};

	/**
	* 变量名的匹配规则
	*/
	class IDTokenRule : public ITokenRule
	{
	public:
		using ITokenRule::ITokenRule;

		// Inherited via ITokenRule
		virtual Token* match(mirror::data::ByteData &data, int pos, int line, int col) override;
	};

	/**
	* 单行注释的匹配规则
	*/
	class SingleCommentTokenRule : public ITokenRule
	{
	public:
		using ITokenRule::ITokenRule;

		// Inherited via ITokenRule
		virtual Token* match(mirror::data::ByteData &data, int pos, int line, int col) override;

	};
}