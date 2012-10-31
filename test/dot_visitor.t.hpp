#include <cxxtest/TestSuite.h>
#include <visitors/dot_visitor.hpp>
#include <parser/ast.hpp>
#include <parser/squid_lexer.hpp>
#include <parser/parser.hpp>
#include <memory>

using namespace squid;

class DotVisitorTestSuite : public CxxTest::TestSuite
{
public:
	struct test_struct
	{
		int a, b;
		test_struct(int a, int b)
			:a(a),
			 b(b)
		{
		}
	};

	void test_manual_ast()
	{
		ast_node *left = new binary_op_node(binary_op_node::MULT, 
			new binary_op_node(binary_op_node::SUB,
				new identifier_node("a"),
				new identifier_node("b")),
			new binary_op_node(binary_op_node::ADD,
				new identifier_node("c"),
				new identifier_node("d")));
		ast_node *right = new binary_op_node(binary_op_node::DIV, 
			new binary_op_node(binary_op_node::MULT,
				new unary_op_node(unary_op_node::MINUS, new identifier_node("e")),
				new identifier_node("f")),
			new binary_op_node(binary_op_node::MULT,
				new identifier_node("g"),
				new identifier_node("h")));
		ast_node *root = new binary_op_node(binary_op_node::ADD, left, right);
		dot_visitor::write_to_file("test/test_manual_ast.dot", root);
	}

	void test_expression_ast()
	{
		squid_lexer lexer("(a.m + -b.m.m) * -(-c.m)");
		parser parser(lexer);
		ast_node_ptr root = parser.parse_expression(0);
		dot_visitor::write_to_file("test/test_expression_ast.dot", root);
	}

	void test_ematch_ast()
	{
		squid_lexer lexer("foo = bar * -baz");
		parser parser(lexer);
		ast_node_ptr root = parser.parse_expression(0);
		dot_visitor::write_to_file("test/test_ematch_ast.dot", root);
	}

	void test_funcdef_ast()
	{
		const char *text = ""
		"\ndef square(a, b) -> a * b"
		"\ndef dotProduct(a, b) -> "
		"\n    a.x * b.x + a.y * b.y"
		"\n";
		squid_lexer lexer(text);
		parser parser(lexer);
		ast_node_ptr root = parser.parse_script();
		dot_visitor::write_to_file("test/test_funcdef_ast.dot", root, text);
	}

};