#include "dot_visitor.hpp"
#include <sstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>

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
	 child_id(dot_edge::INVALID),
	 label()
{
}

dot_edge::dot_edge(int parent_id, int child_id, const std::string &label)
	:parent_id(parent_id),
	 child_id(child_id),
	 label(label)
{
}

dot_edge &dot_edge::operator=(const dot_edge &rhs)
{
	parent_id = rhs.parent_id;
	child_id = rhs.child_id;
	label = rhs.label;
	return *this;
}


dot_visitor::dot_visitor()
	:node_list(),
	 edge_list(),
	 prev_id(dot_edge::INVALID),
	 next_edge_label(""),
	 graph_label("")
{
}

dot_node &dot_visitor::create_node()
{
	node_list.push_back(dot_node(node_list.size()));
	dot_node &node = node_list.back();
	if(prev_id != dot_edge::INVALID)
	{
		edge_list.push_back(dot_edge(prev_id, node.id, next_edge_label));
		next_edge_label = "";
	}
	return node;
}



void dot_visitor::visit_child(int parent_id, ast_node_ptr node)
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

IMPL_VISIT(dot_visitor, match, node)
{
	dot_node &dnode = create_node();
	dnode.attributes["label"] = node->get_operator_str();
	dnode.attributes["shape"] = "oval";
	visit_child(dnode.id, node->pattern);
	visit_child(dnode.id, node->target);
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

IMPL_VISIT(dot_visitor, number, node)
{
	dot_node &dnode = create_node();
	dnode.attributes["label"] = node->text;
	return node;
}

IMPL_VISIT(dot_visitor, expression_list, node)
{
	dot_node &dnode = create_node();
	dnode.attributes["label"] = "expr list";
	dnode.attributes["shape"] = "box";
	auto& children = node->get_children();
	for(auto iter = children.begin(); iter != children.end(); ++iter)
	{
		visit_child(dnode.id, *iter);
	}
	return node;
}

IMPL_VISIT(dot_visitor, function_def, node)
{
	dot_node &dnode = create_node();
	std::stringstream label;
	label << "def " << node->name;
	dnode.attributes["label"] = label.str();
	dnode.attributes["shape"] = "box";
	auto &args = node->args;
	for(auto iter = args.begin(); iter != args.end(); ++iter)
	{
		next_edge_label = "arg";
		visit_child(dnode.id, *iter);
	}
	next_edge_label = "body";
	visit_child(dnode.id, node->body);
	return node;
}

void dot_visitor::print_begin(std::ofstream &out)
{
	out << "digraph AST\n"
	<< "{\n" 
	<< "graph [fontname=Courier,fontsize=10.0,labeljust=l];\n"
	<< "node [shape=box,width=0.2,height=0.2,fontname=Courier,fontsize=10.0,penwidth=0.5];\n"
	<< "edge [weight=1.2,penwidth=0.5,fontname=Courier,fontsize=10.0,labeljust=c];\n"
	<< "labelloc=\"t\";\n"
    << "label=\"" << graph_label << "\";\n";

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
	out << NODE_ID(edge.parent_id) << " -> " << NODE_ID(edge.child_id);
	if(!edge.label.empty())
		out << "[label=\"" << edge.label << "\"]";
	out << ";\n";
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

void dot_visitor::write_to_file(const char *fname, ast_node_ptr node, const std::string &graph_label)
{
	dot_visitor visitor;
	visitor.graph_label = graph_label;
	boost::replace_all(visitor.graph_label, "\n", "\\l");
	node->accept(visitor);
	visitor.write_to_file(fname);
}

}