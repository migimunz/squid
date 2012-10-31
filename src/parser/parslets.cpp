#include "parslets.hpp"
#include "parser.hpp"
#include "tokens.hpp"
#include "squid_lexer.hpp"

namespace squid
{


ast_node_ptr unary_operator_parslet(unary_op_node::unary_op_type type,
	parser &parser,
	token &tok)
{
	ast_node_ptr operand = parser.parse_expression(parser.get_prefix_precedence(tok.type));
	return new unary_op_node(type, operand);
}

ast_node_ptr binary_operator_parslet(
	binary_op_node::binary_op_type type,
	parser &parser,
	ast_node_ptr left,
	token &tok)
{
	ast_node_ptr right = parser.parse_expression(parser.get_infix_precedence(tok.type));
	return new binary_op_node(type, left, right);
}

ast_node_ptr prefix_identifier(parser &parser, token &tok)
{
	return new identifier_node(tok.text.str());
}

ast_node_ptr prefix_unary_minus(parser &parser, token &tok)
{
	return unary_operator_parslet(unary_op_node::MINUS, parser, tok);
}

ast_node_ptr prefix_unary_plus(parser &parser, token &tok)
{
	return unary_operator_parslet(unary_op_node::PLUS, parser, tok);
}

ast_node_ptr prefix_unary_not(parser &parser, token &tok)
{
	return unary_operator_parslet(unary_op_node::NOT, parser, tok);
}

ast_node_ptr infix_add(parser &parser, ast_node_ptr left, token &tok)
{
	return binary_operator_parslet(binary_op_node::ADD, parser, left, tok);
}

ast_node_ptr infix_sub(parser &parser, ast_node_ptr left, token &tok)
{
	return binary_operator_parslet(binary_op_node::SUB, parser, left, tok);
}

ast_node_ptr infix_mult(parser &parser, ast_node_ptr left, token &tok)
{
	return binary_operator_parslet(binary_op_node::MULT, parser, left, tok);
}

ast_node_ptr infix_div(parser &parser, ast_node_ptr left, token &tok)
{
	return binary_operator_parslet(binary_op_node::DIV, parser, left, tok);
}

ast_node_ptr infix_and(parser &parser, ast_node_ptr left, token &tok)
{
	return binary_operator_parslet(binary_op_node::AND, parser, left, tok);
}

ast_node_ptr infix_or(parser &parser, ast_node_ptr left, token &tok)
{
	return binary_operator_parslet(binary_op_node::OR, parser, left, tok);
}

ast_node_ptr infix_member_access(parser &parser, ast_node_ptr left, token &tok)
{
	//FIXME: check that ast_node_ptr right is an identifier maybe?
	ast_node_ptr right = parser.parse_expression(parser.get_infix_precedence(tok.type)); 
	return new member_access_node(left, right);
}

ast_node_ptr prefix_paren_open(parser &parser, token &tok)
{
	parser.start_nested_exp();
	ast_node_ptr exp = parser.parse_expression(0);
	/**
	 ** FIXME: CHECK FOR TUPLES
	 **/
	parser.tok_iter.consume_expected(PAREN_CLOSED);
	parser.end_nested_exp();
	return exp;
}

ast_node_ptr prefix_function_def(parser &parser, token &)
{
	function_def_node_ptr func = new function_def_node();
	token tok;
	parser.tok_iter.peek(tok);
	if(tok.type == IDENTIFIER) // optional function name
	{
		parser.tok_iter.next(tok);
		func->name = tok.text.str();
	}

	parser.parse_comma_separated_exprs(func->args, PAREN_OPEN, PAREN_CLOSED); // (arg1, arg2, ...)
	//FIXME: Parse optional where clause?
	parser.tok_iter.consume_expected(ARROW);			// ->
	func->body = parser.parse_indented_block_or_expr();
	return func;
}

ast_node_ptr infix_function_call(parser &parser, ast_node_ptr left, token &tok)
{
	return NULL;
}

}