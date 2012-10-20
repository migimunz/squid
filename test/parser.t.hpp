#include <cxxtest/TestSuite.h>
#include <parser/squid_lexer.hpp>
#include <parser/parser.hpp>
#include <visitors/sexp_visitor.hpp>
#include <iostream>

using namespace squid;

class SquidParserTestSuite : public CxxTest::TestSuite
{
public:
	void test_create_parser()
	{
		squid_lexer l;
		parser p(l);
	}

	void test_unclosed_paren()
	{
		squid_lexer lexer("(a.m + -b.m.m) * -(-c.m b");
		parser parser(lexer);
		TS_ASSERT_THROWS_ANYTHING(parser.parse_expression(0));
	}

	void test_expr1()
	{
		squid_lexer lex("(a + b) * c + d");
		parser p(lex);
		TS_ASSERT_EQUALS(sexp_visitor::to_sexp(p.parse_expression()), "(+ (* (+ a b) c) d)");
	}

};