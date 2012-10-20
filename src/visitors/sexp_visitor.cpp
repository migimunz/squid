#include "sexp_visitor.hpp"

namespace squid
{

sexp_expression::sexp_expression(std::ostringstream &out, bool write_parens)
	:out(out),
	 first(true),
	 write_parens(write_parens)
{
	if(write_parens)
		out << "(";
}

sexp_expression::~sexp_expression()
{
	if(write_parens)
		out << ")";
}

sexp_expression &sexp_expression::operator<<(const std::string &elem)
{
	if(first)
	{
		first = false;
	}
	else
	{
		out << " ";
	}
	out << elem;
	return *this;
}

sexp_visitor::sexp_visitor()
	:out()
{
}

void sexp_visitor::subexpr(sexp_expression &exp, ast_node *node)
{
	exp << "";
	node->accept(*this);
}

std::string sexp_visitor::str()
{
	return out.str();
}

std::string sexp_visitor::to_sexp(ast_node *node)
{
	sexp_visitor visitor;
	if(node)
	{
		node->accept(visitor);
	}
	return visitor.str();
}

IMPL_VISIT(sexp_visitor, identifier, node)
{
	sexp_expression exp(out, false);
	exp << node->text;
	return node;
}

IMPL_VISIT(sexp_visitor, unary_op, node)
{
	sexp_expression exp(out);
	exp << node->get_operator_str();
	subexpr(exp, node->get_operand());
	return node;
}

IMPL_VISIT(sexp_visitor, binary_op, node)
{
	sexp_expression exp(out);
	exp << node->get_operator_str();
	subexpr(exp, node->get_left());
	subexpr(exp, node->get_right());
	return node;
}

IMPL_VISIT(sexp_visitor, member_access, node)
{
	sexp_expression exp(out);
	exp << ".";
	subexpr(exp, node->get_parent());
	subexpr(exp, node->get_child());
	return node;
}

}