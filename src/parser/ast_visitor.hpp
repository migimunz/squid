#pragma once
#include "ast.hpp"




namespace squid
{

class ast_visitor
{
public:
	DECL_VISIT(identifier);
	DECL_VISIT(unary_op);
	DECL_VISIT(binary_op);
	DECL_VISIT(member_access);
	DECL_VISIT(expression_wrapper);
	DECL_VISIT(expression_list);
	DECL_VISIT(function_def);
	virtual ~ast_visitor();
};
// class ast_node;
// class identifier_node;
// class binary_op_node;


}