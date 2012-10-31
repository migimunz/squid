#pragma once
#include "ast.hpp"

namespace squid
{
// typedef std::function<ast_node_ptr (parser &, token &)> prefix_parslet;
// typedef std::function<ast_node_ptr (parser &, ast_node_ptr , token &)> infix_parslet;
class parser;
class token;

ast_node_ptr prefix_identifier(parser &, token &);
ast_node_ptr prefix_unary_minus(parser &parser, token &);
ast_node_ptr prefix_unary_plus(parser &parser, token &);
ast_node_ptr prefix_unary_not(parser &parser, token &);
ast_node_ptr prefix_paren_open(parser &parser, token &);
ast_node_ptr prefix_function_def(parser &parser, token &);

ast_node_ptr infix_add(parser &parser, ast_node_ptr left, token &);
ast_node_ptr infix_sub(parser &parser, ast_node_ptr left, token &);
ast_node_ptr infix_mult(parser &parser, ast_node_ptr left, token &);
ast_node_ptr infix_div(parser &parser, ast_node_ptr left, token &);
ast_node_ptr infix_and(parser &parser, ast_node_ptr left, token &);
ast_node_ptr infix_or(parser &parser, ast_node_ptr left, token &);
ast_node_ptr infix_member_access(parser &parser, ast_node_ptr left, token &);
ast_node_ptr infix_function_call(parser &parser, ast_node_ptr left, token &);
}