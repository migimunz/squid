#pragma once
#include "../parser/ast_visitor.hpp"
#include "../parser/ast.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <map>

#define NODE_ID(id) "node_" << id

namespace squid
{

typedef std::map<std::string, std::string> attr_map;

struct dot_node
{
	attr_map attributes;
	int id;

	dot_node(int id);
	dot_node &operator=(const dot_node &rhs);
	dot_node &operator=(const dot_node &&rhs);
};

struct dot_edge
{
	static const int INVALID = -1;
	int parent_id, child_id;

	dot_edge();
	dot_edge(int parent_id, int child_id);
	dot_edge &operator=(const dot_edge &rhs);
};

typedef std::vector<dot_node> dot_node_list;
typedef std::vector<dot_edge> dot_edge_list;

class dot_visitor : public ast_visitor
{
	dot_node &create_node();
	void visit_child(int parent_id, ast_node *node);
	void set_parent(int id);

	void print_begin(std::ofstream &out);
	void print_end(std::ofstream &out);
	void print_node(std::ofstream &out, dot_node &node);
	void print_edge(std::ofstream &out, dot_edge &edge);

public:
	dot_node_list node_list;
	dot_edge_list edge_list;
	int prev_id;

	dot_visitor();

	void write_to_file(const char *fname);
	static void write_to_file(const char *fname, ast_node *node);
	DECL_VISIT(identifier);
	DECL_VISIT(unary_op);
	DECL_VISIT(binary_op);
	DECL_VISIT(member_access);
};

}