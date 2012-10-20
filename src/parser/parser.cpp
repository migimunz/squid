#include "parser.hpp"
#include "squid_lexer.hpp"
#include "parslets.hpp"
#include <iostream>
namespace squid
{

parser_exception::parser_exception(const std::string &msg, int position)
	:std::runtime_error(msg),
	 position(position)
{
}

unexpected_token_exception::unexpected_token_exception(const token &tok)
	:std::runtime_error("Unexpected token encountered."),
	 tok(tok)
 { 	
 }

parser::parser(squid_lexer &lexer)
	:lexer(lexer),
	 prefix_map(),
	 infix_map(),
	 infix_precedence_map(TOKEN_TYPE_COUNT),
	 prefix_precedence_map(TOKEN_TYPE_COUNT)
	{
		init_precedences();
		init_parslets();
	}

void parser::set_infix_precedence(token_type t, int precedence)
{
	if(lexer.valid_token_type(t))
	{
		infix_precedence_map[t] = precedence;
	}
}

void parser::set_prefix_precedence(token_type t, int precedence)
{
	if(lexer.valid_token_type(t))
	{
		prefix_precedence_map[t] = precedence;
	}
}

int parser::get_infix_precedence(token_type t) const
{
	if(lexer.valid_token_type(t))
	{
		return infix_precedence_map[t];
	}
	else
	{
		return 0;
	}
}

int parser::get_prefix_precedence(token_type t) const
{
	if(lexer.valid_token_type(t))
	{
		return prefix_precedence_map[t];
	}
	else
	{
		return 0;
	}
}

void parser::init_precedences()
{
	// FCALL
	// ARRAY ACCESS
	set_prefix_precedence(PAREN_OPEN, 		2000);	// nested exprs or tuples
	set_infix_precedence (DOT, 				1000);	// a.b
	set_prefix_precedence(MINUS, 			900);	// -a
	set_prefix_precedence(PLUS, 			900);	// +b
	set_prefix_precedence(NOT, 				900);	// not b
	set_infix_precedence (DIVIDE, 			250);	// a / b
	set_infix_precedence (MULTIPLY, 		250);	// a * b
	set_infix_precedence (PLUS, 			200);	// a + b
	set_infix_precedence (MINUS, 			200);	// a - b
	set_infix_precedence (AND,				100);
	set_infix_precedence (OR,				100);
	/** INFIX PRECEDENCE **/
	set_infix_precedence (MATCH, 			10);	// a = b


}

void parser::init_parslets()
{
	// Prefix parslets
	prefix_map[PAREN_OPEN]	=		prefix_paren_open; //FIXME:
	prefix_map[IDENTIFIER]	= 		prefix_identifier;
	prefix_map[MINUS] 		= 		prefix_unary_minus;
	prefix_map[PLUS] 		= 		prefix_unary_plus;
	prefix_map[NOT]			=		prefix_unary_not;
	// Infix parslets
	infix_map[PLUS]			=		infix_add;
	infix_map[MINUS]		=		infix_sub;
	infix_map[MULTIPLY]		=		infix_mult;
	infix_map[DIVIDE]		=		infix_div;
	infix_map[AND]			=		infix_and;
	infix_map[OR]			=		infix_or;
	infix_map[DOT]			=		infix_member_access;
}

ast_node *parser::parse()
{
	return NULL;
}

ast_node *parser::parse_expression(int precedence)
{
	token tok = lexer.consume();
	prefix_parslet prefix = prefix_map[tok.type];
	if(!prefix)
	{
		throw unexpected_token_exception(tok);
	}
	ast_node *left = prefix(*this, tok);
	
	tok = lexer.look_ahead(0);
	while(precedence < get_infix_precedence(tok.type))
	{
		infix_parslet infix = infix_map[tok.type];
		if(!infix)
		{
			return left;
		}
		lexer.consume();
		left = infix(*this, left, tok);
		tok = lexer.look_ahead(0);	
	}
	return left;
}

}