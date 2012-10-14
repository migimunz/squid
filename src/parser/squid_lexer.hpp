#pragma once
#include "lexer.hpp"
#include "tokens.hpp"

namespace squid
{

class squid_lexer : public lexer
{
protected:
	void init();
public:
	squid_lexer();
	squid_lexer(const str_iter &);
	virtual ~squid_lexer();


};

}