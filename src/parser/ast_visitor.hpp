#pragma once
#include "ast.hpp"




namespace squid
{

class ast_visitor
{
public:

	ast_node_ptr visit_child(const ast_node_ptr &child);
	DECL_VISIT(identifier);
	DECL_VISIT(number);
	DECL_VISIT(unary_op);
	DECL_VISIT(binary_op);
	DECL_VISIT(match);
	DECL_VISIT(member_access);
	DECL_VISIT(expression_list);
	DECL_VISIT(function_def);
	// ast_visitor();
	virtual ~ast_visitor();
};

}