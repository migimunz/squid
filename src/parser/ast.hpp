#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>

#define DECL_VISIT(node) \
	virtual ast_node * visit_##node (node##_node *)

#define IMPL_VISIT(self, node, varname) \
	ast_node * self ::visit_##node (node##_node * varname)

namespace squid
{

/***
 ** WHEN ADDING NEW NODES, ADD VISIT_ACCEPT_IMPL!
***/

/**
 * types of AST nodes
 */
enum node_type
{
	IDENTIFIER_NODE = 0,
	UNARY_OP_NODE,
	BINARY_OP_NODE,
	MEMBER_ACCESS,
	NODE_TYPE_COUNT
};

class ast_visitor;

class ast_node
{
public:
	typedef ast_node self_type;
	typedef std::vector<self_type> node_list;

	node_type type;

	ast_node(node_type type);
	virtual ~ast_node();
	virtual ast_node *accept(ast_visitor &) = 0;
};

typedef ast_node::node_list node_list;
typedef std::unique_ptr<ast_node> ast_node_uptr;
typedef std::string id_str;

class identifier_node : public ast_node
{
public:
	id_str text;
	identifier_node(const id_str &text);
	virtual ast_node *accept(ast_visitor &);
};

class binary_op_node : public ast_node
{
public:
	enum binary_op_type
	{
		ADD,
		SUB,
		MULT,
		DIV,
		AND,
		OR
	};

	binary_op_type op_type;
	ast_node_uptr left, right;

	binary_op_node(binary_op_type op_type, ast_node *left, ast_node *right);
	virtual ast_node *accept(ast_visitor &);
	ast_node *get_left();
	ast_node *get_right();
	const char *get_operator_str();
};

class unary_op_node : public ast_node
{
public:
	enum unary_op_type
	{
		PLUS,
		MINUS,
		NOT
	};

	unary_op_type op_type;
	ast_node_uptr operand;

	unary_op_node(unary_op_type op_type, ast_node *operand);
	virtual ast_node *accept(ast_visitor &);
	ast_node *get_operand();
	const char *get_operator_str();
};

class member_access_node : public ast_node
{
	ast_node_uptr parent, child;
public:

	member_access_node(ast_node *parent, ast_node *child);
	virtual ast_node *accept(ast_visitor &); 
	ast_node *get_parent();
	ast_node *get_child();

};

// typedef std::vector<ast_node_uptr> tuple_members_list;

// class tuple_node : public ast_node
// {
// 	tuple_members_list;
// public:
// 	tuple_node();
	
// };

}