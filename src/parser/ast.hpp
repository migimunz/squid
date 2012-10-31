#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>
#include <boost/intrusive_ptr.hpp>

namespace squid
{

/***
 ** WHEN ADDING NEW NODES, ADD VISIT_ACCEPT_IMPL!
 ** ADD TO VISITORS
 ** ADD TO ENUM
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
	EXPRESSION_WRAPPER,
	EXPRESSION_LIST,
	FUNCTION_DEF,
	NODE_TYPE_COUNT
};

class ast_visitor;

class ast_node
{
	int ref_cnt;
	friend void intrusive_ptr_add_ref(ast_node*);
	friend void intrusive_ptr_release(ast_node*);
public:
	typedef ast_node self_type;
	typedef boost::intrusive_ptr<ast_node> ast_node_ptr;

	node_type type;
	int position;

	ast_node(node_type type);
	virtual ~ast_node();
	virtual ast_node_ptr accept(ast_visitor &) = 0;
};

typedef std::string id_str;
typedef ast_node::ast_node_ptr ast_node_ptr;
typedef std::vector<ast_node_ptr> ast_node_list;

/**
 * Memory management for ast_nodes is handled using intrusive reference counting
 * pointers. This assumes no circular references, which shouldn't be a problem.
 */ 
void intrusive_ptr_add_ref(ast_node *ptr);
void intrusive_ptr_release(ast_node *ptr);

#define DECL_VISIT(node) \
	virtual ast_node_ptr visit_##node (node##_node *)

#define IMPL_VISIT(self, node, varname) \
	ast_node_ptr self ::visit_##node (node##_node * varname)

class identifier_node : public ast_node
{
public:
	id_str text;
	identifier_node(const id_str &text);
	virtual ast_node_ptr accept(ast_visitor &);
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
	ast_node_ptr left, right;

	binary_op_node(binary_op_type op_type, ast_node_ptr left, ast_node_ptr right);
	virtual ast_node_ptr accept(ast_visitor &);
	ast_node_ptr get_left();
	ast_node_ptr get_right();
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
	ast_node_ptr operand;

	unary_op_node(unary_op_type op_type, ast_node_ptr operand);
	virtual ast_node_ptr accept(ast_visitor &);
	ast_node_ptr get_operand();
	const char *get_operator_str();
};

class member_access_node : public ast_node
{
	ast_node_ptr parent, child;
public:

	member_access_node(ast_node_ptr parent, ast_node_ptr child);
	virtual ast_node_ptr accept(ast_visitor &);
	ast_node_ptr get_parent();
	ast_node_ptr get_child();

};

class expression_wrapper_node : public ast_node
{
public:
	enum expression_wrapper_type
	{
		STATEMENT_EXPR,
		LAST_EXPR
	};

	ast_node_ptr child;
	expression_wrapper_type wrapper_type;

	expression_wrapper_node(expression_wrapper_type wrapper_type, ast_node_ptr child);
	virtual ast_node_ptr accept(ast_visitor &); 
	ast_node_ptr get_child();
};

typedef boost::intrusive_ptr<expression_wrapper_node> expression_wrapper_node_ptr;
typedef std::vector<expression_wrapper_node_ptr> expr_node_list;

class expression_list_node : public ast_node
{
	expr_node_list children;
public:
	expression_list_node();
	virtual ast_node_ptr accept(ast_visitor &); 
	expr_node_list &get_children();
	void append_child(expression_wrapper_node_ptr child);
	void convert_last();
};

typedef boost::intrusive_ptr<expression_list_node> expression_list_node_ptr;


class function_def_node : public ast_node
{
public:
	//FIXME: add where clause
	std::string name;
	ast_node_list args;
	ast_node_ptr body;

	function_def_node();
	virtual ast_node_ptr accept(ast_visitor &); 
	void append_arg(ast_node_ptr arg);
};

typedef boost::intrusive_ptr<function_def_node> function_def_node_ptr;

// typedef std::vector<ast_node_uptr> tuple_members_list;

// class tuple_node : public ast_node
// {
// 	tuple_members_list;
// public:
// 	tuple_node();

// };

}