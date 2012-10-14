#include <cxxtest/TestSuite.h>
#include <parser/lexer.hpp>
#include <stdexcept>
#include <sstream>
#include <iostream>

using namespace squid;

class LexerTestSuite : public CxxTest::TestSuite
{
public:
	void test_get_some_names()
	{
		lexer lex;
		TS_ASSERT_EQUALS(lex.get_token_name(PLUS), "'+'");
		TS_ASSERT_EQUALS(lex.get_token_name(INVALID), "INVALID TOKEN");
		TS_ASSERT_THROWS(lex.get_token_name((token_type)-1), lexer_exception);
	}

	void test_get_some_matchers()
	{
		lexer lex;
		TS_ASSERT_EQUALS(lex.get_token_matcher(PLUS), match_plus);
		TS_ASSERT_EQUALS(lex.get_token_matcher(INVALID), match_invalid);
		TS_ASSERT_THROWS(lex.get_token_matcher((token_type)-1), lexer_exception);
	}

	void test_read_token()
	{
		lexer lex("foo + b * c");
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

	void test_consume_expected_tokens()
	{
		lexer lex("foo + b * c");
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
		lexer lex(text);
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