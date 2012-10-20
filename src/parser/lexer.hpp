#pragma once
#include <vector>
#include <deque>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "tokens.hpp"

namespace squid
{

typedef std::unordered_map<token_type, token_match_func, token_hasher> token_matcher_map;
typedef std::unordered_map<token_type, std::string, token_hasher> token_name_map;
typedef std::vector<token_type> token_type_list;
typedef std::deque<token> token_deque;


class lexer_exception : public std::runtime_error
{
public:
	str_iter position;
	lexer_exception(const std::string &, str_iter);
};

class token_expectation_exception : public std::runtime_error
{
public:
	token_type found, expected;
	token_expectation_exception(token_type found, token_type expected);
};



class lexer
{
protected:
	token_matcher_map token_matchers;
	token_name_map token_names;
	token_type_list always_match_list;
	token_deque tokens_ahead;
	token_type skip;

	str_iter beginning, current;

	/*!
	 * Register a token. Maps token type to a match function, a name and
	 * optionally includes or excludes token from the match list.
	 */
	void register_token(token_type, token_match_func, const std::string &, bool);

	/*!
	 * Matches a single token, given token_type. If matching fails, the current
	 * text iterator isn't changed.
	 */
	bool match_single_token(token &, token_type);

	/*!
	 * Attempts to match every token from the match list, 
	 * updates `t to first successful match and returns true if anything matched. 
	 */
	bool match_any_token(token &);

	/*!
	 * Matches an expected token, returns a bool indicating success or failure. 
	 * Updates `t to the matched token. Optionally, skips the skip-token.
	 */
	bool match_expected_token(token &t, token_type type, bool match_skip);

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
	token consume(token_type, bool match_skip = true);
	bool try_consume(token &, token_type, bool match_skip = true);
	void rollback();
	void emit(token_type);

	void report_warning(const char *, int position);
	int get_position();
};

}