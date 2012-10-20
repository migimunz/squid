#pragma once
#include "string_iterator.hpp"
#include "string.hpp"

namespace squid
{

/***
 **	WHEN ADDING NEW TOKENS, ADD THEM TO squid_lexer::init() 
 ** AND CREATE A MATCHER
 **
 ***/
enum token_type
{
	/** SPECIAL **/
	INVALID = 0,
	END_OF_TEXT,		// EOF
	INDENT_FRAG,		// '\n' ('    ' | '\t')*
	WHITESPACE,			// (' '|'\t'|'\t'|'\n')*
	/** OPERATORS AND SYMBOLS **/
	AND,				// 'and'
	OR,					// 'or'
	NOT,				// 'not'
	ARROW,				// '->'
	PLUS,				// '+'
	MINUS,				// '-'
	MULTIPLY,			// '*'
	DIVIDE, 			// '/'

	COMMA,				// ','
	DOT,				// '.'
	MATCH,				// '='
	PAREN_OPEN,			// '('
	PAREN_CLOSED,		// ')'
	IDENTIFIER,			// ('_'|'a'..'z'|'A'..'Z') ('_'|'a'..'z'|'A'..'Z'|'0'..'9')*
	/** KEYWORDS **/
	DEF,				// 'def'
	/** LITERALS AND META **/
	INDENT,				// -->
	DEDENT,				// <--
	TOKEN_TYPE_COUNT 	//number of token types
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
string match_arrow(const str_iter &);
string match_paren_open(const str_iter &);
string match_paren_closed(const str_iter &);
string match_def(const str_iter &);
string match_comma(const str_iter &iter);
string match_dot(const str_iter &iter);
string match_and(const str_iter &iter);
string match_or(const str_iter &iter);
string match_not(const str_iter &iter);
}