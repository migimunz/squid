#pragma once

namespace squid
{
// typedef std::function<ast_node *(parser &, token &)> prefix_parslet;
// typedef std::function<ast_node *(parser &, ast_node *, token &)> infix_parslet;
class ast_node;
class parser;
class token;

ast_node *prefix_identifier(parser &, token &);
ast_node *prefix_unary_minus(parser &parser, token &tok);
ast_node *prefix_unary_plus(parser &parser, token &tok);
ast_node *prefix_unary_not(parser &parser, token &tok);
ast_node *prefix_paren_open(parser &parser, token &tok);

ast_node *infix_add(parser &parser, ast_node *left, token &tok);
ast_node *infix_sub(parser &parser, ast_node *left, token &tok);
ast_node *infix_mult(parser &parser, ast_node *left, token &tok);
ast_node *infix_div(parser &parser, ast_node *left, token &tok);
ast_node *infix_and(parser &parser, ast_node *left, token &tok);
ast_node *infix_or(parser &parser, ast_node *left, token &tok);
ast_node *infix_member_access(parser &parser, ast_node *left, token &tok);
}