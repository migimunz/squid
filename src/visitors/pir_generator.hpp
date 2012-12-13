#pragma once
#include "../parser/ast_visitor.hpp"
#include "../parser/ast.hpp"
#include "scope_context.hpp"
#include <ostream>
#include <vector>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace squid
{

typedef std::vector<scope_context_ptr> context_stack_t;

class pir_code_generator : public ast_visitor
{
	module_code_t module;
	scope_context_ptr context;
	context_stack_t context_stack;
	boost::uuids::random_generator gen;

	std::string unique_name();
public:

	pir_code_generator();
	~pir_code_generator();

	scope_context_ptr push_context(const scope_context_ptr &c);
	scope_context_ptr pop_context();
	void push_scope();
	std::string pop_scope(const std::string &status = "1", const std::string &retval = "0");
	void generate(ast_node_ptr root);
	const module_code_t &get_module() const;

	// DECL_VISIT(identifier);
	DECL_VISIT(number);
	DECL_VISIT(unary_op);
	DECL_VISIT(binary_op);
	// DECL_VISIT(match);
	// DECL_VISIT(member_access);
	DECL_VISIT(expression_list);
	// DECL_VISIT(function_def);
};

}

std::ostream& operator<<(std::ostream& stream, const squid::pir_code_generator& generator);