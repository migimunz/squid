#include "lexer.hpp"
#include <iostream>

namespace squid
{

lexer_exception::lexer_exception(const std::string &msg, str_iter position)
	:std::runtime_error(msg),
	 position(position)
{
}

lexer::lexer()
	:token_matchers(TOKEN_TYPE_COUNT),
	 token_names(TOKEN_TYPE_COUNT),
	 tokens_ahead(),
	 skip(WHITESPACE)
{
	init();
}

lexer::lexer(const str_iter &text)
	:token_matchers(TOKEN_TYPE_COUNT),
	 token_names(TOKEN_TYPE_COUNT),
	 tokens_ahead(),
	 skip(WHITESPACE)
{
	init();
	set_text(text);
}

void lexer::init()
{
	register_token(INVALID, 	match_invalid, 		"INVALID TOKEN");
	register_token(END_OF_TEXT, match_end_of_text, 	"END_OF_TEXT");
	register_token(INDENT_FRAG, match_indent_frag, 	"IDENT");
	register_token(WHITESPACE, 	match_whitespace, 	"WHITESPACE");
	register_token(IDENTIFIER, 	match_identifier, 	"IDENTIFIER");
	register_token(PLUS, 		match_plus, 		"'+'");
	register_token(MINUS,		match_minus, 		"'-'");
	register_token(MULTIPLY,	match_multiply, 	"'*'");
	register_token(DIVIDE,		match_divide, 		"'/'");
	register_token(MATCH,		match_match, 		"'='");
}

bool lexer::match_single_token(token &t, token_type type)
{
	token_match_func matcher = get_token_matcher(type);
	string match = matcher(current);
	if(match.valid())
	{
		t = token(type, match, str_iter::dist(beginning, current));
		current = match.get_end_iter();
		return true;
	}
	else
	{
		return false;
	}
}

bool lexer::match_any_token(token &t)
{
	for(int i = 1; i < TOKEN_TYPE_COUNT; ++i)
	{
		token_type type = (token_type)i;
		if(match_single_token(t, type))
			return true;
	}
	t = token(INVALID);
	return false;
}

bool lexer::match_expected_token(token &t, token_type type)
{
	if(!valid_token_type(type))
		return false;

	//skipping whitespace
	token_match_func skip_matcher = get_token_matcher(skip);
	string match = skip_matcher(current);
	if(match.valid())
		current = match.get_end_iter();

	return match_single_token(t, type);
}

token lexer::read_token()
{
	token t(INVALID);
	read_token(t);
	return t;
}

str_iter lexer::read_token(token &t_out)
{
	token t;
	do
	{
		bool read = match_any_token(t);
		if(!read) //FIXME: Better error reporting
			throw lexer_exception("Invalid token encountered.", current);
	} while(t.valid() && t.type == skip);
	t_out = t;
	return current;
}

token lexer::look_ahead(int distance)
{
	while(distance >= tokens_ahead.size())
	{
		token t;
		read_token(t);
		tokens_ahead.push_back(t);
	}
	return tokens_ahead.front();
}

token lexer::consume()
{
	token t = look_ahead(0);
	if(!tokens_ahead.empty())
		tokens_ahead.pop_front();
	return t;
}

bool lexer::valid_token_type(token_type type) const
{
	return type >= INVALID && type < TOKEN_TYPE_COUNT;
}

std::string lexer::get_token_name(token_type type) const
{
	if(!valid_token_type(type))
		throw lexer_exception("Invalid token type in get_token_name.", current);
	else
		return token_names[type];
}

token_match_func lexer::get_token_matcher(token_type type) const
{
	if(!valid_token_type(type))
		throw lexer_exception("Invalid token type in get_token_matcher.", current);
	else
		return token_matchers[type];
}

void lexer::set_text(const str_iter &text)
{
    beginning = current = text;
}

void lexer::register_token(
	token_type type, 
	token_match_func func, 
	const std::string &name)
{
	if(!valid_token_type(type))
		return;
	token_matchers[type] = func;
	token_names[type] = name;
}

}