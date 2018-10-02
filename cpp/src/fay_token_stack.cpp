#include "fay_token_stack.h"

using namespace fay;

PTR(Token) fay::TokenStack::EndToken(new Token());


PTR(Token) fay::TokenStack::now()
{
	if (this->_tokens == nullptr)
		return this->EndToken;

	if (this->_pos < this->_tokens->size())
		return (*this->_tokens)[this->_pos];

	return this->EndToken;
}
