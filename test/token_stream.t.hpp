#include <cxxtest/TestSuite.h>
#include <parser/lexer_base.hpp>

using namespace squid;

class TokenStreamTestSuite : public CxxTest::TestSuite
{
public:

    void test_iterate()
    {
        lexer_base lex("a + b - c");
        token_iterator i(lex);
        TS_ASSERT_EQUALS(i.next().type, IDENTIFIER);
        TS_ASSERT_EQUALS(i.next().type, PLUS);
        TS_ASSERT_EQUALS(i.next().type, IDENTIFIER);
        TS_ASSERT_EQUALS(i.next().type, MINUS);
        TS_ASSERT_EQUALS(i.next().type, IDENTIFIER);
        TS_ASSERT_EQUALS(i.next().type, END_OF_TEXT);

    }

    void test_newline_compose()
    {
        lexer_base lex("a \n  \n\n \n \n b \n\n \n c");
        token_iterator i(lex);
        TS_ASSERT_EQUALS(i.next().type, IDENTIFIER);
        TS_ASSERT_THROWS_NOTHING(i.consume_newline());
        TS_ASSERT_EQUALS(i.next().type, IDENTIFIER);
        TS_ASSERT_THROWS_NOTHING(i.consume_newline());
        TS_ASSERT_EQUALS(i.next().type, IDENTIFIER);
        TS_ASSERT_EQUALS(i.next().type, END_OF_TEXT);

    }

    void test_indent_dedent()
    {
        const char *text = ""
        "\na"
        "\n    b"
        "\n        c"
        "\n        d"
        "\ne";
        lexer_base lex(text);
        token_iterator i(lex);
        TS_ASSERT_EQUALS(i.consume_indentation().type, INDENT_UNCHANGED);
        TS_ASSERT_EQUALS(i.next().type, IDENTIFIER); //a
        TS_ASSERT(i.peek_newline());

        TS_ASSERT_EQUALS(i.consume_indentation().type, INDENT);
        TS_ASSERT_EQUALS(i.next().type, IDENTIFIER); //b
        TS_ASSERT(i.peek_newline());

        TS_ASSERT_EQUALS(i.consume_indentation().type, INDENT);
        TS_ASSERT_EQUALS(i.next().type, IDENTIFIER); //c
        TS_ASSERT(i.peek_newline());

        TS_ASSERT_EQUALS(i.consume_indentation().type, INDENT_UNCHANGED);
        TS_ASSERT_EQUALS(i.next().type, IDENTIFIER); //d
        TS_ASSERT(i.peek_newline());

        TS_ASSERT_EQUALS(i.consume_indentation().type, DEDENT);
        TS_ASSERT_EQUALS(i.consume_indentation().type, DEDENT);
        TS_ASSERT_EQUALS(i.next().type, IDENTIFIER); //e
        TS_ASSERT_EQUALS(i.next().type, END_OF_TEXT);
    }

};