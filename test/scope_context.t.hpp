#include <cxxtest/TestSuite.h>
#include <visitors/scope_context.hpp>
#include <boost/assign/list_of.hpp>

using namespace squid;

class ScopeContextTestSuite : public CxxTest::TestSuite
{
public:
	void test_test()
	{
		scope_context context("test", boost::assign::list_of(":main")(":anon"));
		TS_ASSERT(true);
	}

	void test_register_new()
	{
		scope_context context("test");
		TS_ASSERT_EQUALS(context.new_register(   ), "$P0");
		TS_ASSERT_EQUALS(context.new_register(   ), "$P1");
		TS_ASSERT_EQUALS(context.new_register("I"), "$I2");
		TS_ASSERT_EQUALS(context.pop_register(   ), "$I2");
		TS_ASSERT_EQUALS(context.pop_register(   ), "$P1");
		TS_ASSERT_EQUALS(context.pop_register(   ), "$P0");
	}

	void test_label_new()
	{
		scope_context context("test");
		TS_ASSERT_EQUALS(context.new_label("LABEL"), "LABEL0");
		TS_ASSERT_EQUALS(context.new_label("LABEL"), "LABEL1");
		TS_ASSERT_EQUALS(context.new_label("LABEL"), "LABEL2");
		TS_ASSERT_EQUALS(context.new_label("LOLBEL"), "LOLBEL0");
		TS_ASSERT_EQUALS(context.new_label("LOLBEL"), "LOLBEL1");
	}

	void test_emit_opcode_directive()
	{
		const char *text = ""
		".sub 'test' \n"
		".dir arg1, arg2\n"
		"opcode arg1, arg2, arg3\n"
		".lex 'a', $P0\n"
		".lex 'b', $P1\n"
		"LABEL0:\n"
		".end\n";
		scope_context context("test");
		context.emit_directive("dir", "arg1", "arg2");
		context.emit_opcode("opcode", "arg1", "arg2", "arg3");
		auto r1 = context.emit_local("a");
		auto r2 = context.emit_local("b");
		context.emit_label(context.new_label("LABEL"));
		context.end_scope();

		TS_ASSERT_EQUALS(context.get_code(), text);
		TS_ASSERT_EQUALS(r1, "$P0");
		TS_ASSERT_EQUALS(r2, "$P1");
	}

	void test_emit_condition()
	{
		const char *text = ""
		".sub 'test' \n"
		"if $P0 goto LABEL0\n"
		"if $P1 != $P2 goto LABEL1\n"
		".end\n";
		scope_context context("test");
		context.emit_condition(
			"if", 
			"$P0", 
			context.new_label("LABEL"));
		context.emit_neq_jump(
			"$P1", 
			"$P2", 
			context.new_label("LABEL"));
		context.end_scope();
		TS_ASSERT_EQUALS(context.get_code(), text);

	}

	void test_emit_exception()
	{
		const char *text = ""
		".sub 'test' \n"
		"new $P0, 'Exception'\n"
		"box $P1, 'message'\n"
		"set $P0['message'], $P1\n"
		"throw $P0\n"
		".end\n";
		scope_context context("test");
		context.emit_throw("message");
		context.end_scope();
		TS_ASSERT_EQUALS(context.get_code(), text);

	}

};