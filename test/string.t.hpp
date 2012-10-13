#include <cxxtest/TestSuite.h>
#include <parser/string.hpp>

using namespace squid;

class StringTestSuite : public CxxTest::TestSuite
{
public:

    void test_comparison()
    {
    	str_iter i1 = "aabbcc";
    	str_iter i2 = i1.next().next();
    	string str1(i1, i2);
    	string str2 = str1;
    	TS_ASSERT_EQUALS(str1, str2);
    }

    void test_to_std_str()
    {
    	str_iter i1 = "aabbcc";
    	str_iter i2 = i1.one_or_more(match, "a").one_or_more(match, "b");
    	string str(i1, i2);
    	TS_ASSERT_EQUALS(str, "aabb");
    }
};