#include "ast.hpp"
#include "ast_visitor.hpp"
#include <stdexcept>

#define VISIT_ACCEPT_IMPL(node) \
	ast_node_ptr  node##_node::accept(ast_visitor &v) \
	{ \
		return v.visit_##node (this);\
	} \
	ast_node_ptr  ast_visitor::visit_##node (node##_node * node) { return node; }


namespace squid
{

void intrusive_ptr_add_ref(ast_node *ptr)
{
	++(ptr->ref_cnt);
}

void intrusive_ptr_release(ast_node *ptr)
{
	--(ptr->ref_cnt);
	if(ptr->ref_cnt == 0)
	{
		delete ptr;
	}
}

ast_node::ast_node(node_type type)
	:ref_cnt(0),
	type(type),
	position(0)
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

binary_op_node::binary_op_node(binary_op_type op_type, ast_node_ptr left, ast_node_ptr right)
	:ast_node(BINARY_OP_NODE),
	op_type(op_type),
	left(left),
	right(right)
{
}

ast_node_ptr binary_op_node::get_left()
{
	return left;
}

ast_node_ptr binary_op_node::get_right()
{
	return right;
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
		default: //To stop the compiler from whining
			return "?";
	}
}

unary_op_node::unary_op_node(unary_op_type op_type, ast_node_ptr operand)
	:ast_node(UNARY_OP_NODE),
	 op_type(op_type),
	 operand(operand)
{
}

ast_node_ptr unary_op_node::get_operand()
{
	return operand;
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

member_access_node::member_access_node(ast_node_ptr parent, ast_node_ptr child)
	:ast_node(MEMBER_ACCESS),
	 parent(parent),
	 child(child)
{
}

ast_node_ptr member_access_node::get_parent()
{
	return parent;
}

ast_node_ptr member_access_node::get_child()
{
	return child;
}


expression_wrapper_node::expression_wrapper_node(expression_wrapper_type wrapper_type, ast_node_ptr child)
	:ast_node(EXPRESSION_WRAPPER),
	wrapper_type(wrapper_type),
	child(child)
{
}

ast_node_ptr expression_wrapper_node::get_child()
{
	return child;
}

expression_list_node::expression_list_node()
	:ast_node(EXPRESSION_LIST),
	children()
{
}

expr_node_list &expression_list_node::get_children()
{
	return children;
}

void expression_list_node::append_child(expression_wrapper_node_ptr child)
{
	children.push_back(child);
}

void expression_list_node::convert_last()
{
	children.back()->wrapper_type = expression_wrapper_node::LAST_EXPR;
}

function_def_node::function_def_node()
	:ast_node(FUNCTION_DEF),
	 name(),
	 args(),
	 body()
{
}

void function_def_node::append_arg(ast_node_ptr arg)
{
	args.push_back(arg);
}


/**
 * accepting visitors
 */

VISIT_ACCEPT_IMPL(identifier)
VISIT_ACCEPT_IMPL(binary_op)
VISIT_ACCEPT_IMPL(unary_op)
VISIT_ACCEPT_IMPL(member_access)
VISIT_ACCEPT_IMPL(expression_wrapper)
VISIT_ACCEPT_IMPL(expression_list)
VISIT_ACCEPT_IMPL(function_def)

}