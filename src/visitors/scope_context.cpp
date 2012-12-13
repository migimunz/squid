#include "scope_context.hpp"
#include <algorithm>
#include <iterator>
#include <utility>

namespace squid
{

void intrusive_ptr_add_ref(scope_context *ptr)
{
	++(ptr->ref_cnt);
}

void intrusive_ptr_release(scope_context *ptr)
{
	--(ptr->ref_cnt);
	if(ptr->ref_cnt == 0)
	{
		//delete ptr;
	}
}

scope_context::scope_context(const std::string &name, 
	const string_list_t &modifiers)
	:name(name),
	 code(),
	 register_stack(),
	 register_counter(0),
	 locals(),
	 labels()
{
	start_scope(modifiers);
}

scope_context::~scope_context()
{
	
}

void scope_context::start_scope(const string_list_t &modifiers)
{
	code << ".sub " << "'" << name << "' ";
	std::copy(modifiers.begin(), modifiers.end(), 
		std::ostream_iterator<std::string>(code, " "));
	code << std::endl;
}

std::string scope_context::get_code()
{
	return code.str();
}

std::string scope_context::get_name()
{
	return name;
}

void scope_context::end_scope(module_code_t &module)
{
	end_scope();
	module.push_back(std::move(code));
}

void scope_context::end_scope()
{
	code << ".end" << std::endl;
}

void scope_context::push_register(const std::string &reg)
{
	register_stack.push_back(reg);
}

std::string scope_context::pop_register()
{
	auto reg = register_stack.back();
	register_stack.pop_back();
	return reg;
}

std::string &scope_context::current_register()
{
	return register_stack.back();
}

std::string scope_context::new_register(const char *type, bool push)
{
	std::stringstream reg;
	reg << "$" << type << register_counter;
	register_counter++;
	auto reg_str = reg.str();
	if(push)
		register_stack.push_back(reg_str);
	return reg_str;
}

std::string scope_context::new_label(const std::string &name)
{
	std::stringstream label;
	label << name;
	auto iter = labels.find(name);
	if(iter == labels.end())
	{
		labels[name] = 0;
		iter = labels.find(name);
	}
	label << iter->second;
	(iter->second)++;
	return label.str();
}

void scope_context::emit_opcode(const std::string &name)
{
	code << name << " " << std::endl;
}
void scope_context::emit_opcode(const std::string &name, const std::string &a1)
{
	code << name << " " << a1 << std::endl;
}
void scope_context::emit_opcode(const std::string &name, const std::string &a1, const std::string &a2)
{
	code << name << " " << a1 << ", " << a2 << std::endl;
}
void scope_context::emit_opcode(const std::string &name, const std::string &a1, const std::string &a2, const std::string &a3)
{
	code << name << " " << a1 << ", " << a2 << ", " << a3 << std::endl;
}

void scope_context::emit_directive(const std::string &name)
{
	code << "." << name << " " << std::endl;
}
void scope_context::emit_directive(const std::string &name, const std::string &a1)
{
	code << "." << name << " " << a1 << std::endl;
}
void scope_context::emit_directive(const std::string &name, const std::string &a1, const std::string &a2)
{
	code << "." << name << " " << a1 << ", " << a2 << std::endl;
}
void scope_context::emit_directive(const std::string &name, const std::string &a1, const std::string &a2, const std::string &a3)
{
	code << "." << name << " " << a1 << ", " << a2 << ", " << a3 << std::endl;
}

std::string scope_context::emit_local(const std::string &name)
{
	auto reg = new_register("P", false);
	code << ".lex '" << name << "', " << reg << std::endl;
	return reg;
}

void scope_context::emit_return(const std::string &status, const std::string &retval)
{
	code << ".return(" << status << ", " << retval << ")" << std::endl;
}

void scope_context::emit_return()
{
	emit_return("1", "0");
}

void scope_context::emit_label(const std::string &label)
{
	code << label << ":" << std::endl;
}

void scope_context::emit_condition(const std::string &name, const std::string &condition, const std::string &label)
{
	code << name << " " << condition << " goto " << label << std::endl;
}

void scope_context::emit_neq_jump(const std::string &op1, const std::string &op2, const std::string &label)
{
	code << "if " << op1 << " != " << op2 << " goto " << label << std::endl;
}

void scope_context::emit_throw(const std::string &msg)
{
	auto exception_reg = new_register("P", false);
	auto message_reg = new_register("P", false);
	std::string errmsg = "'";
	errmsg += msg;
	errmsg += "'";
	emit_opcode("new", exception_reg, "'Exception'");
	emit_opcode("box", message_reg, errmsg);
	emit_opcode("set", exception_reg + "['message']", message_reg);
	emit_opcode("throw", exception_reg);
}

}