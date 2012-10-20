#include "dot_visitor.hpp"
#include <sstream>

namespace squid
{

dot_node::dot_node(int id)
	:attributes(),
	 id(id)
{
}

dot_node &dot_node::operator=(const dot_node &rhs)
{
	if(this != &rhs)
	{
		attributes = rhs.attributes;
	}
	return *this;
}

dot_node &dot_node::operator=(const dot_node &&rhs)
{
	if(this != &rhs)
	{
		attributes = std::move(rhs.attributes);
	}
	return *this;
}

dot_edge::dot_edge()
	:parent_id(dot_edge::INVALID),
	 child_id(dot_edge::INVALID)
{
}

dot_edge::dot_edge(int parent_id, int child_id)
	:parent_id(parent_id),
	 child_id(child_id)
{
}

dot_edge &dot_edge::operator=(const dot_edge &rhs)
{
	parent_id = rhs.parent_id;
	child_id = rhs.child_id;
	return *this;
}


dot_visitor::dot_visitor()
	:node_list(),
	 edge_list(),
	 prev_id(dot_edge::INVALID)
{
}

dot_node &dot_visitor::create_node()
{
	node_list.push_back(dot_node(node_list.size()));
	dot_node &node = node_list.back();
	if(prev_id != dot_edge::INVALID)
	{
		edge_list.push_back(dot_edge(prev_id, node.id));
	}
	return node;
}



void dot_visitor::visit_child(int parent_id, ast_node *node)
{
	if(node)
	{
		prev_id = parent_id;
		node->accept(*this);
	}
}

IMPL_VISIT(dot_visitor, identifier, node)
{
	dot_node &dnode = create_node();
	dnode.attributes["label"] = node->text;
	return node;
}

IMPL_VISIT(dot_visitor, binary_op, node)
{
	dot_node &dnode = create_node();
	dnode.attributes["label"] = node->get_operator_str();
	dnode.attributes["shape"] = "oval";
	visit_child(dnode.id, node->get_left());
	visit_child(dnode.id, node->get_right());
	return node;
}

IMPL_VISIT(dot_visitor, unary_op, node)
{
	dot_node &dnode = create_node();
	dnode.attributes["label"] = node->get_operator_str();
	dnode.attributes["shape"] = "oval";
	visit_child(dnode.id, node->get_operand());
	return node;
}

IMPL_VISIT(dot_visitor, member_access, node)
{
	dot_node &dnode = create_node();
	dnode.attributes["label"] = ".";
	dnode.attributes["shape"] = "oval";
	visit_child(dnode.id, node->get_parent());
	visit_child(dnode.id, node->get_child());
	return node;
}

void dot_visitor::print_begin(std::ofstream &out)
{
	out << "digraph AST\n"
	<< "{\n" 
	<< "node [shape=box,width=0.2,height=0.2,fontname=Courier,fontsize=10.0,penwidth=0.5];\n"
	<< "edge [weight=1.2,penwidth=0.5,];\n";
}

void dot_visitor::print_end(std::ofstream &out)
{
	out << "}";
}

void dot_visitor::print_node(std::ofstream &out, dot_node &node)
{
	out << NODE_ID(node.id) << " [";
	auto begin = node.attributes.begin();
	for(auto iter = begin; iter != node.attributes.end(); ++iter)
	{
		if(iter != begin)
			out << ",";
		out << iter->first << "=\"" << iter->second << "\"";
	}
	out << "];\n";
}

void dot_visitor::print_edge(std::ofstream &out, dot_edge &edge)
{
	out << NODE_ID(edge.parent_id) << " -> " << NODE_ID(edge.child_id) << ";\n";
}


void dot_visitor::write_to_file(const char *fname)
{
	std::ofstream out(fname);
	if(out.fail())
		return;
	print_begin(out);
	for(auto iter = node_list.begin(); iter != node_list.end(); ++iter)
	{
		print_node(out, *iter);
	}
	for(auto iter = edge_list.begin(); iter != edge_list.end(); ++iter)
	{
		print_edge(out, *iter);
	}
	print_end(out);
	out.flush();
}

void dot_visitor::write_to_file(const char *fname, ast_node *node)
{
	dot_visitor visitor;
	node->accept(visitor);
	visitor.write_to_file(fname);
}

}