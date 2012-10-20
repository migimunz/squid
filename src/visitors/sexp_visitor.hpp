#pragma once
#include <sstream>
#include "../parser/ast_visitor.hpp"
#include "../parser/ast.hpp"

namespace squid
{

class sexp_expression
{
	std::ostringstream &out;
	bool first, write_parens;
public:
	sexp_expression(std::ostringstream &out, bool write_parens = true);
	~sexp_expression();
	sexp_expression &operator<<(const std::string &elem);
};

class sexp_visitor : public ast_visitor
{
	std::ostringstream out;
	void subexpr(sexp_expression &exp, ast_node *node);
public:
	sexp_visitor();
	std::string str();
	static std::string to_sexp(ast_node *node);
	DECL_VISIT(identifier);
	DECL_VISIT(unary_op);
	DECL_VISIT(binary_op);
	DECL_VISIT(member_access);
};

}