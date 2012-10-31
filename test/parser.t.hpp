#include <cxxtest/TestSuite.h>
#include <parser/squid_lexer.hpp>
#include <parser/parser.hpp>
#include <visitors/sexp_visitor.hpp>
#include <iostream>

using namespace squid;

class SquidParserTestSuite : public CxxTest::TestSuite
{
public:

	void test_unclosed_paren()
	{
		squid_lexer lexer("(a.m + -b.m.m) * -(-c.m b");
		parser parser(lexer);
		TS_ASSERT_THROWS_ANYTHING(parser.parse_expression(0));
	}

	void test_parse_block_expr()
	{
		const char *text = ""
		"\n    a + b"
		"\n    c - d"
		"\ne + f";
		squid_lexer lexer(text);
		parser p(lexer);
		ast_node_ptr root;
		TS_ASSERT_THROWS_NOTHING(root = p.parse_indented_block());
		TS_ASSERT_EQUALS(sexp_visitor::to_sexp(root), "(expr-list (+ a b) (- c d))");
		TS_ASSERT_THROWS(root = p.parse_indented_block(), parser_exception);

	}

	void test_parse_comma_separated()
	{
		const char *text = "(a + b, c / d, (a + b + c))";
		squid_lexer lexer(text);
		parser p(lexer);
		ast_node_list nodes;
		p.parse_comma_separated_exprs(nodes, PAREN_OPEN, PAREN_CLOSED);
		TS_ASSERT_EQUALS(sexp_visitor::to_sexp(nodes[0]), "(+ a b)");
		TS_ASSERT_EQUALS(sexp_visitor::to_sexp(nodes[1]), "(/ c d)");
		TS_ASSERT_EQUALS(sexp_visitor::to_sexp(nodes[2]), "(+ (+ a b) c)");
	}

	void test_parse_script()
	{
		const char *text = ""
		"\na + b"
		"\nc - d"
		"\ne + f";
		squid_lexer lexer(text);
		parser p(lexer);
		ast_node_ptr root;
		TS_ASSERT_THROWS_NOTHING(root = p.parse_script());
		TS_ASSERT_EQUALS(sexp_visitor::to_sexp(root), "(expr-list (+ a b) (- c d) (+ e f))");
	}


	void test_parse_bad_block_expr()
	{
		const char *text = ""
		"\na + b"
		"\nc - d"
		"\ne + f";
		squid_lexer lexer(text);
		parser p(lexer);
		ast_node_ptr root;
		TS_ASSERT_THROWS(root = p.parse_indented_block(), parser_exception);
	}

	void test_line_break_at_prefix()
	{
		squid_lexer lex("a + b \n * c * d");
		parser p(lex);
		TS_ASSERT_EQUALS(sexp_visitor::to_sexp(p.parse_expression()), "(+ a b)");
		TS_ASSERT_THROWS_ANYTHING(sexp_visitor::to_sexp(p.parse_expression()));
	}

	void test_line_break_at_infix()
	{
		squid_lexer lex("a + b + \n c + d");
		parser p(lex);
		TS_ASSERT_EQUALS(sexp_visitor::to_sexp(p.parse_expression()), "(+ (+ (+ a b) c) d)");
	}

	void test_line_break_in_nested()
	{
		squid_lexer lex("((a\n + b) \n * (c + d\n))");
		parser p(lex);
		TS_ASSERT_EQUALS(sexp_visitor::to_sexp(p.parse_expression()), "(* (+ a b) (+ c d))");
	}

	void test_expr1()
	{
		squid_lexer lex("(a + b) * c + d");
		parser p(lex);
		TS_ASSERT_EQUALS(sexp_visitor::to_sexp(p.parse_expression()), "(+ (* (+ a b) c) d)");
	}

};