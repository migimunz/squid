#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <boost/intrusive_ptr.hpp>

namespace squid
{

typedef std::string register_t;
typedef std::vector<std::string> string_list_t;
typedef std::stringstream code_t;
typedef std::vector<std::stringstream> module_code_t;
typedef std::vector<register_t> register_stack_t;
typedef std::unordered_map<std::string, int, std::hash<std::string> > labels_map_t;
typedef std::unordered_map<std::string, std::string, std::hash<std::string> > locals_map_t;

class scope_context
{
	std::string name;
	code_t code;

	register_stack_t register_stack;
	int register_counter;

	locals_map_t locals;
	labels_map_t labels;

	int ref_cnt;
	friend void intrusive_ptr_add_ref(scope_context*);
	friend void intrusive_ptr_release(scope_context*);
public:
	typedef boost::intrusive_ptr<scope_context> scope_context_ptr;
	
	scope_context(const std::string &name, const string_list_t &modifiers = string_list_t());
	~scope_context();

	void end_scope(module_code_t &module);
	void end_scope();
	std::string get_code();
	std::string get_name();
	void push_register(const std::string &reg);
	std::string pop_register();
	std::string &current_register();

	std::string new_register(const char *type = "P", bool push = true);
	std::string new_label(const std::string &name);

	std::string emit_local(const std::string &name);
	void emit_opcode(const std::string &name);
	void emit_opcode(const std::string &name, const std::string &a1);
	void emit_opcode(const std::string &name, const std::string &a1, const std::string &a2);
	void emit_opcode(const std::string &name, const std::string &a1, const std::string &a2, const std::string &a3);
	void emit_directive(const std::string &name);
	void emit_directive(const std::string &name, const std::string &a1);
	void emit_directive(const std::string &name, const std::string &a1, const std::string &a2);
	void emit_directive(const std::string &name, const std::string &a1, const std::string &a2, const std::string &a3);
	void emit_label(const std::string &label);
	void emit_condition(const std::string &name, const std::string &condition, const std::string &label);
	void emit_neq_jump(const std::string &op1, const std::string &op2, const std::string &label);
	void emit_throw(const std::string &msg);
	void emit_return(const std::string &status, const std::string &retval);
	void emit_return();

private:
	void start_scope(const string_list_t &modifiers);
	void write_separated(const string_list_t &strs);
};

typedef scope_context::scope_context_ptr scope_context_ptr;

}