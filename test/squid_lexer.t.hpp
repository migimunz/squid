#include <cxxtest/TestSuite.h>
#include <parser/squid_lexer.hpp>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <iostream>

using namespace squid;

class SquidLexerTestSuite : public CxxTest::TestSuite
{
public:
	void test_get_some_names()
	{
		squid_lexer lex;
		TS_ASSERT_EQUALS(lex.get_token_name(PLUS), "'+'");
		TS_ASSERT_EQUALS(lex.get_token_name(INVALID), "INVALID TOKEN");
		TS_ASSERT_THROWS(lex.get_token_name((token_type)-1), lexer_exception);
	}

	void test_fail_recover()
	{
		squid_lexer lex("foo *");
		token t;
		lex.try_consume(t, PLUS);
		TS_ASSERT(lex.try_consume(t, IDENTIFIER));
	}

	void test_get_some_matchers()
	{
		squid_lexer lex;
		TS_ASSERT_EQUALS(lex.get_token_matcher(PLUS), match_plus);
		TS_ASSERT_EQUALS(lex.get_token_matcher(INVALID), match_invalid);
		TS_ASSERT_THROWS(lex.get_token_matcher((token_type)-1), lexer_exception);
	}

	void test_keyword_separation()
	{
		TS_ASSERT_EQUALS(squid_lexer("def foo").consume().type, DEF);
		TS_ASSERT_EQUALS(squid_lexer("deffoo").consume().type, IDENTIFIER);
		TS_ASSERT_EQUALS(squid_lexer("not foo").consume().type, NOT);
		TS_ASSERT_EQUALS(squid_lexer("notfoo").consume().type, IDENTIFIER);
	}

	void test_read_token()
	{
		squid_lexer lex("foo + b * c");
		token tok = lex.read_token();
		TS_ASSERT_EQUALS(tok.type, IDENTIFIER);
		TS_ASSERT_EQUALS(tok.text, "foo");

		tok = lex.read_token();
		TS_ASSERT_EQUALS(tok.type, PLUS);

		tok = lex.read_token();
		TS_ASSERT_EQUALS(tok.type, IDENTIFIER);
		TS_ASSERT_EQUALS(tok.text, "b");

		tok = lex.read_token();
		TS_ASSERT_EQUALS(tok.type, MULTIPLY);

		tok = lex.read_token();
		TS_ASSERT_EQUALS(tok.type, IDENTIFIER);
		TS_ASSERT_EQUALS(tok.text, "c");

		tok = lex.read_token();
		TS_ASSERT_EQUALS(tok.type, END_OF_TEXT);
	}

	void test_consume_indent_frag()
	{
		TS_ASSERT_THROWS_NOTHING(squid_lexer("\n    ").consume(INDENT_FRAG, false));
		TS_ASSERT_THROWS_NOTHING(squid_lexer("\n        ").consume(INDENT_FRAG, false));
		TS_ASSERT_THROWS_NOTHING(squid_lexer("\n	    	").consume(INDENT_FRAG, false));
		TS_ASSERT_THROWS_NOTHING(squid_lexer("\n	    	  ").consume(INDENT_FRAG, false));
	}

	void test_consume_indentation()
	{
		squid_lexer l1(
			"\nfoo"
			"\n    bar"
			"\n        baz"
			"\nfiz"
			"\nwub"
			);
		TS_ASSERT_THROWS_NOTHING(l1.consume(IDENTIFIER)); //foo
		TS_ASSERT_THROWS_NOTHING(l1.consume_indent_token(INDENT));
		TS_ASSERT_THROWS_NOTHING(l1.consume(IDENTIFIER)); //bar
		TS_ASSERT_THROWS_NOTHING(l1.consume_indent_token(INDENT));
		TS_ASSERT_THROWS_NOTHING(l1.consume(IDENTIFIER)); //baz
		TS_ASSERT_THROWS_NOTHING(l1.consume_indent_token(DEDENT));
		TS_ASSERT_THROWS_NOTHING(l1.consume_indent_token(DEDENT));
		TS_ASSERT_THROWS_NOTHING(l1.consume(IDENTIFIER)); //fiz
		TS_ASSERT_THROWS_NOTHING(l1.consume(IDENTIFIER)); //wub
	}

	void test_lookahead()
	{
		squid_lexer lex("foo + b * c");
		lex.look_ahead(5);
		TS_ASSERT_EQUALS(lex.consume().type, IDENTIFIER);
		TS_ASSERT_EQUALS(lex.consume().type, PLUS);
		TS_ASSERT_EQUALS(lex.consume().type, IDENTIFIER);
		TS_ASSERT_EQUALS(lex.consume().type, MULTIPLY);
		TS_ASSERT_EQUALS(lex.consume().type, IDENTIFIER);
	}

	void test_emit()
	{
		squid_lexer lex("+ b * c");
		lex.emit(IDENTIFIER);
		lex.look_ahead(5);
		TS_ASSERT_EQUALS(lex.consume().type, IDENTIFIER);
		TS_ASSERT_EQUALS(lex.consume().type, PLUS);
		TS_ASSERT_EQUALS(lex.consume().type, IDENTIFIER);
		TS_ASSERT_EQUALS(lex.consume().type, MULTIPLY);
		TS_ASSERT_EQUALS(lex.consume().type, IDENTIFIER);
	}

	void test_rollback()
	{
		squid_lexer lex("foo + b * c");
		lex.look_ahead(5);
		lex.rollback();
		lex.look_ahead(5);
		TS_ASSERT_EQUALS(lex.consume().type, IDENTIFIER);
		TS_ASSERT_EQUALS(lex.consume().type, PLUS);
		TS_ASSERT_EQUALS(lex.consume().type, IDENTIFIER);
		TS_ASSERT_EQUALS(lex.consume().type, MULTIPLY);
		TS_ASSERT_EQUALS(lex.consume().type, IDENTIFIER);
	}

	void test_consume_expected_tokens()
	{
		squid_lexer lex("foo + b * c");
		TS_ASSERT_THROWS_NOTHING(lex.consume(IDENTIFIER));
		TS_ASSERT_THROWS_NOTHING(lex.consume(PLUS));
		TS_ASSERT_THROWS_NOTHING(lex.consume(IDENTIFIER));
		TS_ASSERT_THROWS(lex.consume(MINUS), token_expectation_exception);
		TS_ASSERT_THROWS_NOTHING(lex.consume(MULTIPLY));
		TS_ASSERT_THROWS_NOTHING(lex.consume(IDENTIFIER));
		TS_ASSERT_THROWS_NOTHING(lex.consume(END_OF_TEXT));
		TS_ASSERT_THROWS(lex.consume(IDENTIFIER), token_expectation_exception);
	}

	void _test_print_tokens()
	{
		const char *text = "foo = a + b * c / d";
		squid_lexer lex(text);
		std::stringstream stream;
		stream << "\n  Tokenizing: " << text << "\n  Result: ";

		token t;
		do
		{
			t = lex.consume();
			stream << lex.get_token_name(t.type) << " ";
		} while(t.valid() && t.type != END_OF_TEXT);

		std::cout << stream.str() << std::endl;
	}


};