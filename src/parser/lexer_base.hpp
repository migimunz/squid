#pragma once
#include "lexer.hpp"
#include "string_iterator.hpp"

namespace squid
{

// enum skip_pattern
// {
// 	SKIP_NONE = 0,
// 	SKIP_NEWLINE = 1,
// 	SKIP_INDENT = 2,
// 	SKIP_ALL = 3
// };


class lexer_base
{
	
protected:
	friend class token_iterator;

	token_matcher_map token_matchers;
	token_name_map token_names;
	token_type_list always_match_list;

	str_iter beginning;
	const char *space_indent;

	void init();
public:
	lexer_base(const str_iter &);

	/*!
	 * Register a token. Maps token type to a match function, a name and
	 * optionally includes or excludes token from the match list.
	 */
	void register_token(token_type, token_match_func, const std::string &, bool);
	std::string get_token_name(const str_iter &current, token_type) const;
	token_match_func get_token_matcher(const str_iter &current, token_type) const;
	bool valid_token_type(token_type) const;
	str_iter match_single_token(const str_iter &current, token &t, token_type type) const;
	str_iter match_any_token(const str_iter &current, token &t) const;

	// bool is_token_whitespace(skip_pattern sp, const token &tok);
};


class token_iterator
{
	lexer_base &lexer;
	str_iter current;
	int indent_level;

	int token_iterator::compute_indent_level(const string &indent_str) const;
public:
	token_iterator(lexer_base &lexer, int indent_level = 0);
	token_iterator(const token_iterator &rhs);
	token_iterator &operator=(const token_iterator &rhs);

	/*!
	 * Get the current position in the text
	 */
	int get_position() const;

	/*!
	 * Match any registered token (in always_match list).
	 * Does not consume the token.
	 */
	bool peek(token &tok_out) const;
	token peek() const;

	/*!
	 * Match any registered token (in always_match list).
	 * Consumes the token (alters the iterator).
	 */
	bool next(token &tok_out);
	token next();

	/*!
	 * Consumes an expected token or throws.
	 */
	void consume_expected(token_type type);

	/*!
	 * Attempt to match a newline or a series of newlines.
	 * Does not consume the token.
	 */
	bool peek_newline() const;

	/*!
	 * Attempt to match a newline or a series of newlines.
	 * Consumes the token.
	 */
	void consume_newline();

	/*!
	 * Matches a single INDENT, DEDENT or INDENT_UNCHANGED token 
	 * but doesn't consume it.
	 */
	bool peek_indentation(token &tok) const;
	bool peek_indentation(token_type type) const;

	/*!
	 * Matches a single INDENT, DEDENT or INDENT_UNCHANGED token 
	 * and consumes it.
	 */
	bool consume_indentation(token &tok);
	token consume_indentation();

};


}