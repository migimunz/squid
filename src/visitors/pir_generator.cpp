#include "pir_generator.hpp"
#include <boost/assign/list_of.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace squid
{

pir_code_generator::pir_code_generator()
	:module(),
	 context(),
	 context_stack(),
	 gen()
{
	auto main_context = new scope_context(
		"test", 
		boost::assign::list_of(":main"));
	push_context(scope_context_ptr(main_context));
}

pir_code_generator::~pir_code_generator()
{
}

void pir_code_generator::generate(ast_node_ptr root)
{
	visit_child(root);
	context->end_scope(module);
}

scope_context_ptr pir_code_generator::push_context(const scope_context_ptr &c)
{
	context_stack.push_back(c);
	this->context = c;
	return c;
}

scope_context_ptr pir_code_generator::pop_context()
{
	auto popped = context_stack.back();
	context_stack.pop_back();
	popped->end_scope(module);
	this->context = context_stack.back();
	return popped;
}

std::string pir_code_generator::unique_name()
{
	boost::uuids::uuid uuid = gen();
	std::string suuid = to_string(uuid);
	std::string name = "'sub_";
	name += suuid + "'";
	return name;
}

void pir_code_generator::push_scope()
{
	auto name = unique_name();
	std::string outer = ":outer(";
	outer += context->get_name() + ")";

	string_list_t modifiers;
	modifiers.push_back(":anon");
	modifiers.push_back(outer);

	auto new_context = new scope_context(name, modifiers);
	push_context(scope_context_ptr(new_context));
}

std::string pir_code_generator::pop_scope(const std::string &status, const std::string &retval)
{
	context->emit_return(status, retval);
	auto popped = pop_context();
	return popped->get_name();
}

const module_code_t &pir_code_generator::get_module() const
{
	return module;
}

IMPL_VISIT(pir_code_generator, number, node)
{
	context->emit_opcode("box", context->new_register(), node->text);
	return node;
}

IMPL_VISIT(pir_code_generator, binary_op, node)
{
	const char *opcode;
	switch(node->op_type)
	{
		case binary_op_node::ADD:
			opcode = "add";
			break;
		case binary_op_node::SUB:
			opcode = "sub";
			break;
		case binary_op_node::MULT:
			opcode = "mul";
			break;
		case binary_op_node::DIV:
			opcode = "div";
			break;
		case binary_op_node::AND:
			opcode = "and";
			break;
		case binary_op_node::OR:
			opcode = "or";
			break;
	}
	visit_child(node->left);
	visit_child(node->right);
	register_t right_reg 	= context->pop_register();
	register_t left_reg 	= context->pop_register();
	register_t result_reg 	= context->new_register();
	context->emit_opcode(opcode, result_reg, left_reg, right_reg);
	return node;
}

IMPL_VISIT(pir_code_generator, unary_op, node)
{
	const char *opcode;
	switch(node->op_type)
	{
		case unary_op_node::PLUS:
			return visit_child(node->operand);
		case unary_op_node::MINUS:
			opcode = "neg";
			break;
		case unary_op_node::NOT:
			opcode = "not";
			break;
	}
	visit_child(node->operand);
	register_t operand_reg 	= context->pop_register();
	register_t result_reg 	= context->new_register();
	context->emit_opcode(opcode, result_reg, operand_reg);
	return node;
}

IMPL_VISIT(pir_code_generator, expression_list, node)
{
	for(auto iter = node->children.begin(); iter != node->children.end(); ++iter)
	{
		visit_child(*iter);
		if(!(iter + 1 == node->children.end()))
			context->pop_register();
	}
	return node;
}

	// DECL_VISIT(identifier);
	// DECL_VISIT(match);
	// DECL_VISIT(member_access);
	// DECL_VISIT(function_def);

}

std::ostream& operator<<(std::ostream& stream, const squid::pir_code_generator& generator)
{
	const squid::module_code_t &module = generator.get_module();
	for(auto iter = module.rbegin(); iter != module.rend(); ++iter)
	{
		stream << iter->str();
	}
	return stream;
}