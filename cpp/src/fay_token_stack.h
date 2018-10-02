#pragma once

#include "fay_token.h"

namespace fay
{
	class TokenStack
	{
	private:
		PTR(std::vector<PTR(Token)>) _tokens;
		int _pos = 0;
		int _sign = -1;

	public:
		static PTR(Token) EndToken;

		TokenStack(PTR(std::vector<PTR(Token)>) tokens)
			: _tokens(tokens) {}

		int pos() { return this->_pos; }

		PTR(Token) now();
	};
}