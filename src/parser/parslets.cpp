#include "parslets.hpp"
#include "parser.hpp"
#include "tokens.hpp"
#include "ast.hpp"
#include "squid_lexer.hpp"

namespace squid
{


ast_node *unary_operator_parslet(unary_op_node::unary_op_type type,
	parser &parser,
	token &tok)
{
	ast_node *operand = parser.parse_expression(parser.get_prefix_precedence(tok.type));
	return new unary_op_node(type, operand);
}

ast_node *binary_operator_parslet(
	binary_op_node::binary_op_type type,
	parser &parser,
	ast_node *left,
	token &tok)
{
	ast_node *right = parser.parse_expression(parser.get_infix_precedence(tok.type));
	return new binary_op_node(type, left, right);
}

ast_node *prefix_identifier(parser &parser, token &tok)
{
	return new identifier_node(tok.text.str());
}

ast_node *prefix_unary_minus(parser &parser, token &tok)
{
	return unary_operator_parslet(unary_op_node::MINUS, parser, tok);
}

ast_node *prefix_unary_plus(parser &parser, token &tok)
{
	return unary_operator_parslet(unary_op_node::PLUS, parser, tok);
}

ast_node *prefix_unary_not(parser &parser, token &tok)
{
	return unary_operator_parslet(unary_op_node::NOT, parser, tok);
}

ast_node *infix_add(parser &parser, ast_node *left, token &tok)
{
	return binary_operator_parslet(binary_op_node::ADD, parser, left, tok);
}

ast_node *infix_sub(parser &parser, ast_node *left, token &tok)
{
	return binary_operator_parslet(binary_op_node::SUB, parser, left, tok);
}

ast_node *infix_mult(parser &parser, ast_node *left, token &tok)
{
	return binary_operator_parslet(binary_op_node::MULT, parser, left, tok);
}

ast_node *infix_div(parser &parser, ast_node *left, token &tok)
{
	return binary_operator_parslet(binary_op_node::DIV, parser, left, tok);
}

ast_node *infix_and(parser &parser, ast_node *left, token &tok)
{
	return binary_operator_parslet(binary_op_node::AND, parser, left, tok);
}

ast_node *infix_or(parser &parser, ast_node *left, token &tok)
{
	return binary_operator_parslet(binary_op_node::OR, parser, left, tok);
}

ast_node *infix_member_access(parser &parser, ast_node *left, token &tok)
{
	//FIXME: check that ast_node *right is an identifier maybe?
	ast_node *right = parser.parse_expression(
		parser.get_infix_precedence(tok.type) - 1); //right associative
	return new member_access_node(left, right);
}

ast_node *prefix_paren_open(parser &parser, token &tok)
{
	ast_node *exp = parser.parse_expression(0);
	/**
	 ** FIXME: CHECK FOR TUPLES
	 **/
	parser.lexer.consume(PAREN_CLOSED);
	return exp;
}


}