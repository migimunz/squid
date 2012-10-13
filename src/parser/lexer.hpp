#pragma once
#include <vector>
#include <deque>
#include <stdexcept>
#include <string>
#include "tokens.hpp"

namespace squid
{

typedef std::vector<token_match_func> token_matcher_map;
typedef std::vector<std::string> token_name_map;
typedef std::deque<token> token_deque;

class lexer_exception : std::runtime_error
{
public:
	str_iter position;
	lexer_exception(const std::string &, str_iter);
};

class lexer
{
	token_matcher_map token_matchers;
	token_name_map token_names;
	token_deque tokens_ahead;
	token_type skip;

	str_iter beginning, current;

	void init();
	void register_token(token_type, token_match_func, const std::string &);
	bool match_single_token(token &t, token_type type);
	bool match_any_token(token &t);
	bool match_expected_token(token &t, token_type type);
public:
	lexer();
	lexer(const str_iter &);

	bool valid_token_type(token_type) const;
	std::string get_token_name(token_type) const;
	token_match_func get_token_matcher(token_type) const;

	void set_text(const str_iter &);

	token read_token();
	str_iter read_token(token &);
	token look_ahead(int distance);
	token consume();

	//bool look_ahead(token_type )
};

}