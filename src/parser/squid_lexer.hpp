#pragma once
#include "lexer_base.hpp"
#include "tokens.hpp"

namespace squid
{

class squid_lexer : public lexer_base
{
public:
	squid_lexer(const str_iter &iter);
};

}