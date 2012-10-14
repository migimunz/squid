#include "squid_lexer.hpp"

namespace squid
{

squid_lexer::squid_lexer()
	:lexer()
{
	init();
}

squid_lexer::squid_lexer(const str_iter &text)
	:lexer(text)
{
	init();
}

squid_lexer::~squid_lexer()
{
}

void squid_lexer::init()
{
	register_token(INVALID, 	match_invalid, 		"INVALID TOKEN",	false);
	register_token(END_OF_TEXT, match_end_of_text, 	"END_OF_TEXT",		true);
	register_token(INDENT_FRAG, match_indent_frag, 	"IDENT FRAGMENT",	true);
	register_token(WHITESPACE, 	match_whitespace, 	"WHITESPACE",		true);
	register_token(PLUS, 		match_plus, 		"'+'", 				true);
	register_token(MINUS,		match_minus, 		"'-'", 				true);
	register_token(MULTIPLY,	match_multiply, 	"'*'", 				true);
	register_token(DIVIDE,		match_divide, 		"'/'", 				true);
	register_token(MATCH,		match_match, 		"'='", 				true);
	register_token(IDENTIFIER, 	match_identifier, 	"IDENTIFIER", 		true);
	/**
	 * INDENT and DEDENT are never matched in text, only
	 * generated by the lexer when matching INDENT FRAGMENT
	 */
	register_token(INDENT,		match_always_fail,	"INDENT",			false);
	register_token(DEDENT,		match_always_fail,	"DEDENT",			false);
}


}