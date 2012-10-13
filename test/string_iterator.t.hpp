#include <cxxtest/TestSuite.h>
#include <parser/string_iterator.hpp>

using namespace squid;

const char *text1 = "aabbcc";
const char *text2 = "ab cs sd ba";

class StringIteratorTestSuite : public CxxTest::TestSuite
{

public:
    void test_equality()
    {
    	str_iter i1(text1);
    	str_iter i2(text1);
    	str_iter i3(text2);
    	TS_ASSERT(i1 == i2);
    	TS_ASSERT(i1 != i3);
    }

    void test_iteration()
    {
    	str_iter i1(text2);
    	str_iter i2(text2);
    	TS_ASSERT(*(++i1) == 'b');
    	TS_ASSERT(*(i2.next()) == 'b');
    }

    void test_str_match()
    {
    	str_iter i1(text1);
    	str_iter i2 = match(i1, "aa");
    	str_iter i3 = match(i2, "bb");
    	str_iter i4 = match(i2, "??");
    	TS_ASSERT(i2.get_ptr() == (text1+2));
    	TS_ASSERT(i3.get_ptr() == (text1+4));
    	TS_ASSERT(!i4.valid());
    }

    void test_zero_or_more()
    {
    	str_iter i1(text1);
    	str_iter i2 = zero_or_more(i1, "a");
    	str_iter i3 = zero_or_more(i2, "b");
    	str_iter i4 = zero_or_more(i1, "??");
    	TS_ASSERT(i2.get_ptr() == (text1+2));
    	TS_ASSERT(i3.get_ptr() == (text1+4));
    	TS_ASSERT(i4.get_ptr() == text1);
    }

    void test_one_or_more()
    {
    	str_iter i1(text1);
    	str_iter i2 = one_or_more(i1, "a");
    	str_iter i3 = one_or_more(i2, "c");
    	TS_ASSERT(i2.get_ptr() == (text1+2));
    	TS_ASSERT(!i3.valid());
    }

    void test_optional()
    {
    	str_iter i1(text1);
    	str_iter i2 = optional(i1, "aa");
    	str_iter i3 = optional(i1, "bb");
    	TS_ASSERT(i2.get_ptr() == (text1+2));
    	TS_ASSERT(i3.get_ptr() == text1);
    }

    void test_any()
    {
        str_iter i1(text1);
        str_iter i2 = any(i1, "abc");
        str_iter i3 = any(i1, "bc");
        str_iter i4 = str_iter("a b c").any(" \r\n\t");
        TS_ASSERT(i2.get_ptr() == (text1+1));
        TS_ASSERT(!i3.valid());
        TS_ASSERT(!i4.valid());
    }

    void test_alternative()
    {
    	str_iter i1(text1);
    	str_iter i2 = match(i1, "aa") | match(i1, "bb");
    	str_iter i3 = match(i1, "bb") | match(i1, "aa");
    	str_iter i4 = match(i1, "cc") | match(i1, "aa");
    	str_iter i5 = match(i1, "aa") | match(i1, "cc");
    	str_iter i6 = match(i1, "cc") | match(i1, "zz");
    	TS_ASSERT(i2.get_ptr() == (text1+2));
    	TS_ASSERT(i3.get_ptr() == (text1+2));
    	TS_ASSERT(i4.get_ptr() == (text1+2));
    	TS_ASSERT(i5.get_ptr() == (text1+2));
    	TS_ASSERT(!i6.valid());
    }


    void test_chaining()
    {
    	str_iter i1(text1);
    	str_iter i2 = i1.match("aa").match("bb").match("cc");
    	str_iter i3 = i1.match("aa").any("abc").optional(match, "b").optional(match, "?").match("cc");
    	str_iter i4 = i1.match("aa").any("??").optional(match, "b").optional(match, "?").match("cc");
    	TS_ASSERT(*i2 == '\0');
    	TS_ASSERT(*i3 == '\0');
    	TS_ASSERT(!i4.valid());
    }


    void test_distance()
    {
        str_iter i = text1;
        TS_ASSERT_EQUALS(str_iter::dist(i, one_or_more(i, "a")), 2);
        TS_ASSERT_EQUALS(str_iter::dist(one_or_more(i, "a"), i), -2);
        TS_ASSERT_EQUALS(str_iter::dist(i, i), 0);
    }

    void test_cclass()
    {
    	str_iter i1 = "abcd";
    	str_iter i2 = "1234";
    	str_iter i3 = "01EF";
    	str_iter i4 = "    ";
    	TS_ASSERT(*one_or_more(i1, alpha, NULL) == '\0');
    	TS_ASSERT(*one_or_more(i2, digit, NULL) == '\0');
    	TS_ASSERT(*one_or_more(i3, hex_digit, NULL) == '\0');
    	TS_ASSERT(*one_or_more(i4, ws, NULL) == '\0');
    }

    void test_alternate_cclass()
    {
        str_iter i1 = "a!";
        str_iter i2 = "1!";
        str_iter i3 = "?!";
        str_iter o1 = i1.alpha() | i1.digit();
        str_iter o2 = i2.alpha() | i2.digit();
        str_iter o3 = i3.alpha() | i3.digit();
        TS_ASSERT(o1.valid());
        TS_ASSERT(o2.valid());
        TS_ASSERT(!o3.valid());
        TS_ASSERT_EQUALS(*o1, '!');
        TS_ASSERT_EQUALS(*o2, '!');
    }

    void test_match_match_identifier()
    {
    	str_iter ids[4];
    	ids[0] = "_match_identifier";
    	ids[1] = "_";
    	ids[2] = "_abc_bcd";
    	ids[3] = "a";
    	for(int i = 0; i < sizeof(ids) / sizeof(str_iter); ++i)
    	{
    		str_iter iter = ids[i];
    	}
    }

};