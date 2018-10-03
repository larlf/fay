#include "fay_token_stack.h"

using namespace fay;

PTR(Token) fay::TokenStack::EndToken(new Token());


PTR(Token) fay::TokenStack::now()
{
	if (this->_pos>=0 && this->_pos < this->_tokens->size())
		return (*this->_tokens)[this->_pos];

	return this->EndToken;
}

PTR(Token) fay::TokenStack::before()
{
	if (this->_pos >= 1 && this->_pos < this->_tokens->size() + 1)
		return (*this->_tokens)[this->_pos -1];

	return this->EndToken;
}

PTR(Token) fay::TokenStack::after()
{
	if (this->_pos >= -1 && this->_pos < this->_tokens->size() - 1)
		return (*this->_tokens)[this->_pos + 1];

	return this->EndToken;
}

PTR(Token) fay::TokenStack::next()
{
	if (this->_pos < this->_tokens->size())
		this->_pos++;

	//跳过注释
	if (this->now()->is(TokenType::Comment))
		this->next();

	return this->now();
}

PTR(Token) fay::TokenStack::move()
{
	auto token = this->now();
	this->next();
	return token;
}

PTR(Token) fay::TokenStack::findNextToken(const std::vector<TokenType>& types)
{
	for (auto i = this->_pos; i < this->_tokens->size(); ++i)
	{
		for (auto j = 0; j < types.size(); ++j)
		{
			if ((*this->_tokens)[i]->type() == types[j])
				return (*this->_tokens)[i];
		}
	}

	return nullptr;
}
