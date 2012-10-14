#include "tokens.hpp"
#include <stdexcept>

namespace squid
{

token::token()
	:type(INVALID),
	 text(),
	 position()
{
}

token::token(token_type type, const string &text, int position)
	:type(type),
	 text(text),
	 position(position)
{
}

token::token(const token &rhs)
	:type(rhs.type),
	 text(rhs.text),
	 position(rhs.position)
{ 	
}

token &token::operator=(const token &rhs)
{
	type = rhs.type;
	text = rhs.text;
	position = rhs.position;
	return *this;
}

bool token::valid() const
{
	return type > 0 && type < TOKEN_TYPE_COUNT;
}

size_t token_hasher::operator()(const token &t) const
{
	return t.type;
}

string match_invalid(const str_iter &)
{
	throw std::runtime_error("Lexer error: match_invalid applied");
}

string match_always_fail(const str_iter &)
{
	return string();
}

string match_identifier(const str_iter &iter)
{
	if(!iter.valid()) return string();

	str_iter matched = iter.alpha() | iter.match("_");
	if(matched.valid())
	{
		matched = matched.zero_or_more([](str_iter matched, str_iter::str_ptr) 
			{ 
				return matched.alpha() | matched.digit() | matched.match("_"); 
			}, NULL) | matched;
		return string(iter, matched);
	}
	return string();
}

string match_plus(const str_iter &iter)
{
	if(!iter.valid()) return string();
	return string(iter, iter.match("+"));
}

string match_minus(const str_iter &iter)
{
	if(!iter.valid()) return string();
	return string(iter, iter.match("-"));
}

string match_multiply(const str_iter &iter)
{
	if(!iter.valid()) return string();
	return string(iter, iter.match("*"));
}

string match_divide(const str_iter &iter)
{
	if(!iter.valid()) return string();
	return string(iter, iter.match("/"));
}

string match_match(const str_iter &iter)
{
	if(!iter.valid()) return string();
	return string(iter, iter.match("="));
}

string match_indent_frag(const str_iter &iter)
{
	if(!iter.valid()) return string();
	return string(iter, iter.match("\n").zero_or_more(match, "\t"));
}

string match_whitespace(const str_iter &iter)
{
	if(!iter.valid()) return string();
	return string(iter, iter.one_or_more(any, " \r\n\t"));
}

string match_end_of_text(const str_iter &iter)
{
	if(!iter.valid() || !iter.end()) 
		return string();
	else
		return string(iter, iter);
}

}