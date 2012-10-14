#pragma once
#include "string_iterator.hpp"
#include "string.hpp"

namespace squid
{


enum token_type
{
	INVALID = 0,
	END_OF_TEXT,
	INDENT_FRAG,
	WHITESPACE,
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE, 
	MATCH,
	IDENTIFIER,
	INDENT,
	DEDENT,
	TOKEN_TYPE_COUNT //number of token types
};

typedef string(*token_match_func)(const str_iter &);

struct token
{
	token_type type;
	string text;
	int position;

	token();
	token(token_type type, const string &text = string(), int position = 0);
	token(const token &rhs);

	token &operator=(const token &rhs);
	bool valid() const;
};

struct token_hasher
{
	size_t operator()(const token &) const;
};

string match_invalid(const str_iter &);
string match_always_fail(const str_iter &);
string match_identifier(const str_iter &);
string match_plus(const str_iter &);
string match_minus(const str_iter &);
string match_multiply(const str_iter &);
string match_divide(const str_iter &);
string match_match(const str_iter &);
string match_indent_frag(const str_iter &);
string match_whitespace(const str_iter &);
string match_end_of_text(const str_iter &);


}