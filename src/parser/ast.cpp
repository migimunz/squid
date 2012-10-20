#include "ast.hpp"
#include "ast_visitor.hpp"

#define VISIT_ACCEPT_IMPL(node) \
	ast_node * node##_node::accept(ast_visitor &v) \
	{ \
		return v.visit_##node (this);\
	} \
	ast_node * ast_visitor::visit_##node (node##_node * node) { return node; }


namespace squid
{

ast_node::ast_node(node_type type)
	:type(type)
{
}

ast_node::~ast_node()
{
}

ast_visitor::~ast_visitor() {}


identifier_node::identifier_node(const id_str &text)
	:ast_node(IDENTIFIER_NODE),
	text(text)
{
}

binary_op_node::binary_op_node(binary_op_type op_type, ast_node *left, ast_node *right)
	:ast_node(BINARY_OP_NODE),
	op_type(op_type),
	left(left),
	right(right)
{
}

ast_node *binary_op_node::get_left()
{
	return left.get();
}

ast_node *binary_op_node::get_right()
{
	return right.get();
}

const char *binary_op_node::get_operator_str()
{
	switch(op_type)
	{
		case binary_op_node::ADD:
			return "+";
		case binary_op_node::SUB:
			return "-";
		case binary_op_node::MULT:
			return "*";
		case binary_op_node::DIV:
			return "/";
		case binary_op_node::AND:
			return "and";
		case binary_op_node::OR:
			return "or";
		default: //To stop the compiler from whinin
			return "?";
	}
}

unary_op_node::unary_op_node(unary_op_type op_type, ast_node *operand)
	:ast_node(UNARY_OP_NODE),
	 op_type(op_type),
	 operand(operand)
{
}

ast_node *unary_op_node::get_operand()
{
	return operand.get();
}

const char *unary_op_node::get_operator_str()
{
	switch(op_type)
	{
		case unary_op_node::PLUS:
			return "+";
		case unary_op_node::MINUS:
			return "-";
		case unary_op_node::NOT:
			return "not";
		default: //To stop the compiler from whining
			return "?";
	}
}

member_access_node::member_access_node(ast_node *parent, ast_node *child)
	:ast_node(MEMBER_ACCESS),
	 parent(parent),
	 child(child)
{
}

ast_node *member_access_node::get_parent()
{
	return parent.get();
}

ast_node *member_access_node::get_child()
{
	return child.get();
}
/**
 * accepting visitors
 */


// IMPL_VISIT(ast_visitor, identifier, node) 		{ return node; }
// IMPL_VISIT(ast_visitor, binary_op, node) 		{ return node; }
// IMPL_VISIT(ast_visitor, unary_op, node) 		{ return node; }

VISIT_ACCEPT_IMPL(identifier)
VISIT_ACCEPT_IMPL(binary_op)
VISIT_ACCEPT_IMPL(unary_op)
VISIT_ACCEPT_IMPL(member_access)

}