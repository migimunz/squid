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
	write(elem, true);
	return *this;
}

void sexp_expression::write(const std::string &elem, bool separate)
{
	if(first)
	{
		first = false;
	}
	else if(separate)
	{
		out << " ";
	}
	out << elem;
}

sexp_visitor::sexp_visitor()
	:out()
{
}

void sexp_visitor::subexpr(sexp_expression &exp, ast_node_ptr node)
{
	exp << "";
	node->accept(*this);
}

std::string sexp_visitor::str()
{
	return out.str();
}

std::string sexp_visitor::to_sexp(ast_node_ptr node)
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

IMPL_VISIT(sexp_visitor, expression_list, node)
{
	sexp_expression exp(out);
	exp << "expr-list";
	auto &children = node->get_children();
	for(auto iter = children.begin(); iter != children.end(); ++iter)
	{
		subexpr(exp, *iter);
	}
	return node;
}

IMPL_VISIT(sexp_visitor, function_def, node)
{
	sexp_expression exp(out);
	exp << "def";
	if(!node->name.empty())
	{
		exp << node->name;
	}
	exp.write("(", true);
	auto &args = node->args;
	for(auto iter = args.begin(); iter != args.end(); ++iter)
	{
		subexpr(exp, *iter);
	}
	exp.write(")", false);

	subexpr(exp, node->body);
	return node;
}

}