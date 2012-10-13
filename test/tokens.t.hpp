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