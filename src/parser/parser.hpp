#pragma once
#include <unordered_map>
#include <functional>
#include <vector>
#include <stdexcept>
#include <string>
#include "ast.hpp"
#include "tokens.hpp"
#include "squid_lexer.hpp"

namespace squid
{

class parser;

typedef ast_node_ptr (*prefix_parslet)(parser &, token &);
typedef ast_node_ptr (*infix_parslet)(parser &, ast_node_ptr , token &);

typedef std::unordered_map<token_type, prefix_parslet, token_hasher> prefix_parslet_map;
typedef std::unordered_map<token_type, infix_parslet, token_hasher> infix_parslet_map;
typedef std::vector<int> precedence_map;

class squid_lexer;

struct parser_exception : public std::runtime_error
{
	int position;
	parser_exception(const std::string &msg, int position);
};

struct unexpected_token_exception : public std::runtime_error
{
	token tok;
	unexpected_token_exception(const token &tok);
};

class parser
{
public:
	squid_lexer &lexer;
	token_iterator tok_iter;
protected:
	prefix_parslet_map prefix_map;
	infix_parslet_map infix_map;
	precedence_map prefix_precedence_map;
	precedence_map infix_precedence_map;
	int nested_exp_depth;

	void set_infix_precedence(token_type t, int precedence);
	void set_prefix_precedence(token_type t, int precedence);
	void init_precedences();
	void init_parslets();
	bool is_expression_terminated();

public:

	parser(squid_lexer &lexer);
	int get_infix_precedence(token_type t) const;
	int get_prefix_precedence(token_type t) const;
	void start_nested_exp();
	void end_nested_exp();
	ast_node_ptr parse();

	/** PARSE METHODS **/
	ast_node_ptr parse_expression(int precedence = 0);
	expression_wrapper_node_ptr parse_wrapped_expression();
	expression_list_node_ptr parse_indented_block();
	ast_node_ptr parse_indented_block_or_expr();
	expression_list_node_ptr parse_script();
	void parse_comma_separated_exprs(ast_node_list &list, token_type, token_type);

};

struct auto_nested_expr
{
	parser &p;
	auto_nested_expr(parser &p);
	~auto_nested_expr();
};

}