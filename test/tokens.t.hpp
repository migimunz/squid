#include <cxxtest/TestSuite.h>
#include <parser/tokens.hpp>

using namespace squid;

class TokensTestSuite : public CxxTest::TestSuite
{
public:
	void test_match_id()
	{
		TS_ASSERT_EQUALS(match_identifier("_"), "_");
		TS_ASSERT_EQUALS(match_identifier("a + b + c"), "a");
		TS_ASSERT_EQUALS(match_identifier("abc"), "abc");
		TS_ASSERT_EQUALS(match_identifier("a8_cbf__"), "a8_cbf__");
	}

	void test_match_invalid_id()
	{
		TS_ASSERT(!match_identifier("8abc").valid());
		TS_ASSERT(!match_identifier("#abc").valid());
		TS_ASSERT(!match_identifier("$@5@abc").valid());
	}

	void test_match_operators()
	{
		TS_ASSERT_EQUALS(match_plus("+"), "+");
		TS_ASSERT_EQUALS(match_minus("-"), "-");
		TS_ASSERT_EQUALS(match_multiply("*"), "*");
		TS_ASSERT_EQUALS(match_divide("/"), "/");
		TS_ASSERT(!match_divide("*").valid());
	}

	void test_match_int()
	{
		TS_ASSERT_EQUALS(match_integer("1234"), "1234");
		TS_ASSERT_EQUALS(match_integer("123.4"), "123");
	}

	void test_match_int_hex()
	{
		TS_ASSERT_EQUALS(match_hex_integer("0x1234"), "0x1234");
		TS_ASSERT_EQUALS(match_hex_integer("0X123.4"), "0X123");
		TS_ASSERT(!match_hex_integer("1234").valid());
	}

	void test_match_float()
	{
		TS_ASSERT_EQUALS(match_float("12.34"), "12.34");
		TS_ASSERT_EQUALS(match_float("1.3"), "1.3");
		TS_ASSERT_EQUALS(match_float(".4"), ".4");
		TS_ASSERT_EQUALS(match_float("1.4e2"), "1.4e2");
		TS_ASSERT_EQUALS(match_float("1.4E+123"), "1.4E+123");
		TS_ASSERT_EQUALS(match_float("1.4e-123"), "1.4e-123");
		TS_ASSERT_EQUALS(match_float(".2E2"), ".2E2");
		TS_ASSERT(!match_float(".2E*2").valid());
		TS_ASSERT(!match_float("22").valid());
	}

	void test_end_of_text()
	{
		TS_ASSERT(match_end_of_text("").valid());
		TS_ASSERT(!match_end_of_text("a").valid());
	}

	void test_token_create_copy()
	{
		token t1(MATCH);
		token t2 = t1;
		TS_ASSERT_EQUALS(t1.type, t2.type);
	}

	void test_token_match()
	{

	}
};