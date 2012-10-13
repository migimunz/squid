#include "string_iterator.hpp"

namespace squid
{

str_iter match(str_iter iter, str_ptr str)
{
	return iter.match(str);
}

str_iter any(str_iter iter, str_ptr str)
{
	return iter.any(str);
}

str_iter zero_or_more(str_iter iter, const match_func &func, str_ptr str)
{
	return iter.zero_or_more(func, str);
}

str_iter zero_or_more(str_iter iter, str_ptr str)
{
	return zero_or_more(iter, match, str);
}

str_iter one_or_more(str_iter iter, const match_func &func, str_ptr str)
{
	return iter.one_or_more(func, str);
}

str_iter one_or_more(str_iter iter, str_ptr str)
{
	return one_or_more(iter, match, str);
}

str_iter optional(str_iter iter, const match_func &func, str_ptr str)
{
	return iter.optional(func, str);
}

str_iter optional(str_iter iter, str_ptr str)
{
	return optional(iter, match, str);
}


str_iter alpha(str_iter iter, str_ptr str)
{
	return iter.alpha();
}

str_iter digit(str_iter iter, str_ptr str)
{
	return iter.digit();
}

str_iter hex_digit(str_iter iter, str_ptr)
{
	return iter.hex_digit();
}

str_iter ws(str_iter iter, str_ptr)
{
	return iter.ws();
}

}