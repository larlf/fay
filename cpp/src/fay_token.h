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
		Token(TokenType type, PTR(ByteData) data, int pos, int size, int line, int col)
		{
			this->_type = type;
			this->_text = std::string((char*)data->data(), pos, size);
			this->_line = line;
			this->_col = col;
		}

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
		virtual PTR(Token) match(PTR(ByteData) data, int pos, int line, int col) = 0;
	};

	class CharTokenRule : ITokenRule
	{
	public:
		using ITokenRule::ITokenRule;

		// 通过 ITokenRule 继承
		virtual PTR(Token) match(PTR(ByteData) data, int pos, int line, int col) override;

	};
}