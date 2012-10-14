#pragma once
#include <functional>
#include <locale>

namespace squid
{

template<typename T>
class string_iterator
{
	
public:
	typedef const T* str_ptr;
	typedef string_iterator<T> self_type;
	typedef std::function<self_type(self_type, str_ptr)> match_func;
	typedef std::function<bool(T)> pred_func;
	str_ptr ptr;

	string_iterator()
		:ptr(NULL) {}

	string_iterator(str_ptr ptr)
		:ptr(ptr) {}

	string_iterator(const self_type &rhs)
		:ptr(rhs.ptr) {}

	self_type & operator=(const self_type &rhs)
	{
		ptr = rhs.ptr;
		return *this;
	}

	self_type & operator=(str_ptr ptr)
	{
		this->ptr = ptr;
		return *this;
	}

	bool operator==(const self_type &rhs) const
	{
		return ptr == rhs.ptr;
	}

	bool operator!=(const self_type &rhs) const
	{
		return ptr != rhs.ptr;
	}

	bool operator<(const self_type &rhs) const
	{
		return ptr < rhs.ptr;
	}

	bool operator>(const self_type &rhs) const
	{
		return ptr > rhs.ptr;
	}

	bool operator<=(const self_type &rhs) const
	{
		return ptr <= rhs.ptr;
	}

	bool operator>=(const self_type &rhs) const
	{
		return ptr >= rhs.ptr;
	}

	T operator*() const
	{
		return *ptr;
	}

	self_type operator|(const self_type &rhs) const
	{
		return valid() ? *this : rhs;
	}

	self_type &operator++()
	{
		++ptr;
		return *this;
	}

	str_ptr get_ptr() const
	{
		return ptr;
	}

	self_type next() const
	{
		return self_type(ptr + 1);
	}

	bool valid() const
	{
		return ptr != NULL;
	}

	bool end() const
	{
		return *ptr == '\0';
	}

	static int dist(const self_type &a, const self_type &b)
	{
		return b.ptr - a.ptr;
	}

	self_type match(str_ptr str) const
	{
		if(!valid())
			return self_type();
		self_type iter = *this;

		while(str != NULL && *str != '\0')
		{
			if(iter.end() || *iter != *str)
				return self_type();
			++iter;
			++str;
		}
		return iter;
	}

	self_type match_char_predicate(const pred_func &pred) const
	{
		if(!valid() || !pred(*ptr))
			return self_type();
		else
			return next();
	}

	self_type alpha() const
	{
		using namespace std::placeholders;
		return match_char_predicate(std::bind(std::isalpha<T>, _1, std::locale()));
	}

	self_type digit() const
	{
		using namespace std::placeholders;
		return match_char_predicate(std::bind(std::isdigit<T>, _1, std::locale()));
	}

	self_type hex_digit() const
	{
		using namespace std::placeholders;
		return match_char_predicate(std::bind(std::isxdigit<T>, _1, std::locale()));
	}

	self_type ws() const
	{
		using namespace std::placeholders;
		return match_char_predicate(std::bind(std::isspace<T>, _1, std::locale()));
	}

	self_type any(str_ptr str) const
	{
		if(!valid())
			return self_type();
		self_type iter = *this;

		while(str != NULL && *str != '\0' && !iter.end())
		{
			if(*iter == *str)
				return iter.next();
			else
				++str;
		}
		return self_type();
	}

	self_type zero_or_more(const match_func &func, str_ptr str) const
	{
		if(!valid())
			return self_type();
		self_type iter = *this;
		self_type new_iter;

		while((new_iter = func(iter, str)).valid())
		{
			iter = new_iter;
		}
		return iter;
	}

	self_type one_or_more(const match_func &func, str_ptr str) const
	{
		if(!valid())
			return self_type();
		self_type iter = *this;
		iter = func(iter, str);
		if(!iter.valid())
			return self_type();
		else
			return zero_or_more(func, str);
	}

	self_type optional(const match_func &func, str_ptr str)
	{
		if(!valid())
			return self_type();
		self_type iter = *this;
		self_type new_iter = func(iter, str);
		return new_iter == NULL ? iter : new_iter;
	}



};

typedef string_iterator<char> str_iter;
typedef str_iter::str_ptr str_ptr;
typedef str_iter::match_func match_func;

str_iter match(str_iter iter, str_ptr str);

str_iter any(str_iter iter, str_ptr str);

str_iter zero_or_more(str_iter iter, const match_func &func, str_ptr str);

str_iter zero_or_more(str_iter iter, str_ptr str);

str_iter one_or_more(str_iter iter, const match_func &func, str_ptr str);

str_iter one_or_more(str_iter iter, str_ptr str);

str_iter optional(str_iter iter, const match_func &func, str_ptr str);

str_iter optional(str_iter iter, str_ptr str);

str_iter alpha(str_iter iter, str_ptr str = NULL);

str_iter digit(str_iter iter, str_ptr str = NULL);

str_iter hex_digit(str_iter iter, str_ptr str = NULL);

str_iter ws(str_iter iter, str_ptr str = NULL);

}