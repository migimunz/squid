#include "lexer.hpp"
#include <iostream>

namespace squid
{

lexer_exception::lexer_exception(const std::string &msg, str_iter position)
	:std::runtime_error(msg),
	 position(position)
{
}

token_expectation_exception::token_expectation_exception(
	token_type found, 
	token_type expected)
	:std::runtime_error("Unexpected token encountered."),
	 found(found),
	 expected(expected)
{
}

lexer::lexer()
	:token_matchers(),
	 token_names(),
	 tokens_ahead(),
	 skip(WHITESPACE)
{
}

lexer::~lexer()
{
}

lexer::lexer(const str_iter &text)
	:token_matchers(),
	 token_names(),
	 tokens_ahead(),
	 skip(WHITESPACE)
{
	set_text(text);
}


bool lexer::match_single_token(token &t, token_type type)
{
	token_match_func matcher = get_token_matcher(type);
	string match = matcher(current);
	if(match.valid())
	{
		t = token(type, match, str_iter::dist(beginning, current));
		current = match.get_end_iter();
		transform_token(t);
		return true;
	}
	else
	{
		return false;
	}
}

bool lexer::match_expected_token(token &t, token_type type, bool match_skip)
{
	if(!valid_token_type(type))
		return false;

	//skipping whitespace
	if(match_skip)
	{
		token dummy;
		match_single_token(dummy, skip);
	}
	return match_single_token(t, type);
}

bool lexer::match_any_token(token &t)
{
	//for(int i = 1; i < TOKEN_TYPE_COUNT; ++i)
	for(auto iter = always_match_list.begin(); iter != always_match_list.end(); ++iter)
	{
		token_type type = (token_type)*iter;
		if(match_single_token(t, type))
			return true;
	}
	t = token(INVALID);
	return false;
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

void lexer::transform_token(token &t)
{
}

void lexer::rollback()
{
	if(!tokens_ahead.empty())
	{
		token first = tokens_ahead.front();
		current = first.text.get_start_iter();
		tokens_ahead.clear();
	}
}

token lexer::consume()
{
	token t = look_ahead(0);
	if(!tokens_ahead.empty())
		tokens_ahead.pop_front();
	return t;
}

token lexer::consume(token_type type, bool match_skip)
{
	token t;
	rollback();
	if(match_expected_token(t, type, match_skip))
	{
		return t;
	}
	else
	{
		t = look_ahead(0);
		throw token_expectation_exception(t.type, type);
	}
}

void lexer::emit(token_type type)
{
	tokens_ahead.push_front(token(
		type, 
		string(current, current),
		str_iter::dist(beginning, current)));
}

// token lexer::consume(token_type type)
// {
// 	token t = look_ahead(0);
// 	if(t.type != type)
// 		throw token_expectation_exception(t.type, type);
// 	else
// 		tokens_ahead.pop_front();
// 	return t;
// }

// token lexer::consume(token_type type)
// {
// 	token t(INVALID);
// 	if(tokens_ahead.empty())
// 		match_expected_token(t, type);
// 	else
// 		t = tokens_ahead.front();

// 	if(t.type != type)
// 	{	
// 		t = look_ahead(0);
// 		throw token_expectation_exception(t.type, type);
// 	}
// 	else
// 	{
// 		tokens_ahead.pop_front();
// 	}
// 	return t;
// }

// token lexer::consume(token_type type)
// {
// 	token t = look_ahead(0);
// 	if(t.type != type)
// 		throw token_expectation_exception(t.type, type);
// 	else
// 		tokens_ahead.pop_front();
// 	return t;
// }

bool lexer::valid_token_type(token_type type) const
{
	return type >= INVALID && type < TOKEN_TYPE_COUNT;
}

std::string lexer::get_token_name(token_type type) const
{
	if(!valid_token_type(type))
		throw lexer_exception("Invalid token type in get_token_name.", current);
	else
		return token_names.at(type);
}

token_match_func lexer::get_token_matcher(token_type type) const
{
	if(!valid_token_type(type))
		throw lexer_exception("Invalid token type in get_token_matcher.", current);
	else
		return token_matchers.at(type);
}

void lexer::set_text(const str_iter &text)
{
    beginning = current = text;
}

void lexer::register_token(
	token_type type, 
	token_match_func func, 
	const std::string &name,
	bool always_match)
{
	if(!valid_token_type(type))
		return;
	token_names[type] = name;
	token_matchers[type] = func;
	if(always_match)
		always_match_list.push_back(type);
}

void lexer::report_warning(const char *, int position)
{

}

int lexer::get_position()
{
	return str_iter::dist(beginning, current);
}

}