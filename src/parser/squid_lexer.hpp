#pragma once
#include "lexer.hpp"
#include "tokens.hpp"

namespace squid
{

int compute_indent_level(str_iter iter, str_iter end);

class squid_lexer : public lexer
{
	str_iter::str_ptr space_indent;
	int indent_level;
protected:
	void init();
	int compute_indent_level(str_iter iter, str_iter end);

public:
	squid_lexer();
	squid_lexer(const str_iter &);

	bool try_consume_indent_token(token_type);
	void consume_indent_token(token_type);
};

}