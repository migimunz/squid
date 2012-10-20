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

str_iter match_identifier_char(str_iter matched, str_iter::str_ptr)
{
	return matched.alpha() | matched.digit() | matched.match("_");
}

string match_identifier(const str_iter &iter)
{
	if(!iter.valid()) return string();

	str_iter matched = iter.alpha() | iter.match("_");
	if(matched.valid())
	{
		matched = matched.zero_or_more(match_identifier_char, NULL) | matched;
		return string(iter, matched);
	}
	return string();
}

string match_str(const str_iter &iter, const char *str)
{
	if(!iter.valid()) return string();
	return string(iter, iter.match(str));
}

string match_keyword(const str_iter &iter, const char *str)
{
	if(!iter.valid()) return string();
	str_iter match = iter.match(str);
	if(match.valid() && !match_identifier_char(match, NULL).valid())
	{
		return string(iter, match);
	}
	else
	{
		return string();
	}
}

string match_plus(const str_iter &iter)
{
	return match_str(iter, "+");
}

string match_minus(const str_iter &iter)
{
	return match_str(iter, "-");
}

string match_multiply(const str_iter &iter)
{
	return match_str(iter, "*");
}

string match_divide(const str_iter &iter)
{
	return match_str(iter, "/");
}

string match_match(const str_iter &iter)
{
	return match_str(iter, "=");
}

string match_arrow(const str_iter &iter)
{
	return match_str(iter, "->");
}

string match_paren_open(const str_iter &iter)
{
	return match_str(iter, "(");
}

string match_paren_closed(const str_iter &iter)
{
	return match_str(iter, ")");
}

string match_comma(const str_iter &iter)
{
	return match_str(iter, ",");
}

string match_dot(const str_iter &iter)
{
	return match_str(iter, ".");
}

/** KEYWORDS **/

string match_def(const str_iter &iter)
{
	return match_keyword(iter, "def");
}

string match_and(const str_iter &iter)
{
	return match_keyword(iter, "and");
}

string match_or(const str_iter &iter)
{
	return match_keyword(iter, "or");
}

string match_not(const str_iter &iter)
{
	return match_keyword(iter, "not");
}

string match_indent_frag(const str_iter &iter)
{
	if(!iter.valid()) return string();
	return string(iter, iter.match("\n").zero_or_more(any, " \t"));
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