#pragma once
#include "string_iterator.hpp"
#include <string>

namespace squid
{

template<typename T>
class base_string
{
	string_iterator<T> start, end;
public:
	typedef string_iterator<T> str_iter;
	typedef base_string<T> self_type;

	base_string()
		:start(),
		end()
		{}

	base_string(const str_iter &start, const str_iter &end)
		:start(start),
		 end(end)
		{}

	base_string(const self_type &rhs)
		:start(rhs.start),
		end(rhs.end)
		{}

	// //DANGEROUS
	// base_string(typename str_iter::str_ptr str)
	// 	:start(str),
	// 	 end(str)
	// {
	// 	while(!end.end())
	// 	{
	// 		++end;
	// 	}
	// }

	base_string &operator=(const str_iter &rhs)
	{
		start = rhs.start;
		end = rhs.end;
		return *this;
	}

	bool operator==(const self_type &rhs)
	{
		return start == rhs.start && end == rhs.end;
	}

	bool operator==(const std::string &rhs)
	{
		return str() == rhs;
	}

	bool operator!=(const self_type &rhs)
	{
		return !(*this == rhs);
	}

	// //DANGEROUS
	// base_string &operator=(typename str_iter::str_ptr str)
	// {
	// 	*this = self_type(str);
	// 	return *this;
	// }

	bool valid()
	{
		return start.valid() && end.valid();
	}

	std::string str() const
	{
		return std::string(start.get_ptr(), str_iter::dist(start, end));
	}

	str_iter get_start_iter() const
	{
		return start;
	}

	str_iter get_end_iter() const
	{
		return end;
	}

};

typedef base_string<char> string;

}