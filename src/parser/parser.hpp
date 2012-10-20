#pragma once
#include <unordered_map>
#include <functional>
#include <vector>
#include <stdexcept>
#include <string>
#include "ast.hpp"
#include "tokens.hpp"

namespace squid
{

class parser;

typedef std::function<ast_node *(parser &, token &)> prefix_parslet;
typedef std::function<ast_node *(parser &, ast_node *, token &)> infix_parslet;

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
protected:
	prefix_parslet_map prefix_map;
	infix_parslet_map infix_map;
	precedence_map prefix_precedence_map;
	precedence_map infix_precedence_map;

	void set_infix_precedence(token_type t, int precedence);
	void set_prefix_precedence(token_type t, int precedence);
	void init_precedences();
	void init_parslets();

public:

	parser(squid_lexer &lexer);
	int get_infix_precedence(token_type t) const;
	int get_prefix_precedence(token_type t) const;
	ast_node *parse();

	/** PARSE METHODS **/
	ast_node *parse_expression(int precedence = 0);
	ast_node *parse_statement_expression();

};

}