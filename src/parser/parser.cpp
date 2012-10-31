#include "parser.hpp"
#include "parslets.hpp"

namespace squid
{

auto_nested_expr::auto_nested_expr(parser &p)
	:p(p)
{
	p.start_nested_exp();
}

auto_nested_expr::~auto_nested_expr()
{
	p.end_nested_exp();
}

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
	 tok_iter(lexer),
	 prefix_map(),
	 infix_map(),
	 infix_precedence_map(TOKEN_TYPE_COUNT),
	 prefix_precedence_map(TOKEN_TYPE_COUNT),
	 nested_exp_depth(0)
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

void parser::start_nested_exp()
{
	nested_exp_depth++;
}

void parser::end_nested_exp()
{
	nested_exp_depth--;
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
	prefix_map[DEF]			=		prefix_function_def; //FIXME:
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

ast_node_ptr parser::parse()
{
	return NULL;
}

bool parser::is_expression_terminated()
{
	return nested_exp_depth == 0 && tok_iter.peek_newline();
}

expression_wrapper_node_ptr parser::parse_wrapped_expression()
{
	return new expression_wrapper_node(expression_wrapper_node::STATEMENT_EXPR, parse_expression(0));
}

ast_node_ptr parser::parse_indented_block_or_expr()
{
	token indent_tok;
	tok_iter.peek_indentation(indent_tok);
	if(indent_tok.type == INDENT)
		return parse_indented_block();
	else
		return parse_expression(0);
}

expression_list_node_ptr parser::parse_indented_block()
{
	expression_list_node_ptr expr_list = new expression_list_node();
	token indent_tok;

	tok_iter.consume_indentation(indent_tok);
	if(indent_tok.type != INDENT)
	{
		throw parser_exception("Expected an indented block here.", tok_iter.get_position());
	}
	else //first indented expr
	{
		expr_list->append_child(parse_wrapped_expression());
	}

	tok_iter.consume_indentation(indent_tok);
	while(indent_tok.type != DEDENT)
	{
		if(indent_tok.type == INDENT_UNCHANGED)
		{
			expr_list->append_child(parse_wrapped_expression());
		}
		else
		{
			throw parser_exception("Expected an indented block here.", tok_iter.get_position());
		}
		tok_iter.consume_indentation(indent_tok);
	} 
	expr_list->convert_last();

	return expr_list;
}

expression_list_node_ptr parser::parse_script()
{
	expression_list_node_ptr expr_list = new expression_list_node();
	token indent_tok;
	token eof_tok;

	while(true)
	{
		tok_iter.consume_indentation(indent_tok);
		if(indent_tok.type != INDENT_UNCHANGED)
		{
			throw parser_exception("Unexpected indentation.", tok_iter.get_position());
		}

		tok_iter.peek(eof_tok);
		if(eof_tok.type == END_OF_TEXT)
		{
			break;
		}

		expr_list->append_child(parse_wrapped_expression());
	}
	return expr_list;

}

void parser::parse_comma_separated_exprs(ast_node_list &list, token_type open_tok, token_type close_tok)
{
	auto_nested_expr auto_nested(*this);
	tok_iter.consume_expected(open_tok);

	token tok;
	tok_iter.peek(tok);
	if(tok.type == close_tok) // empty expr list
	{
		return;
	}

	list.push_back(parse_expression(0)); //first expr in list
	tok_iter.peek(tok);
	while(tok.type != close_tok) // consume comma separated exprs
	{
		tok_iter.consume_expected(COMMA);
		list.push_back(parse_expression(0));
		tok_iter.peek(tok);
	}
	tok_iter.consume_expected(close_tok);
}

ast_node_ptr parser::parse_expression(int precedence)
{
	token tok = tok_iter.next();
	auto prefix = prefix_map.find(tok.type);
	if(prefix == prefix_map.end())
	{
		throw unexpected_token_exception(tok);
	}
	ast_node_ptr left = prefix->second(*this, tok);

	if(is_expression_terminated()) return left;
	tok = tok_iter.peek();

	while(precedence < get_infix_precedence(tok.type))
	{
		auto infix = infix_map.find(tok.type);
		if(infix == infix_map.end())
		{
			return left;
		}
		tok_iter.next();
		left = infix->second(*this, left, tok);

		if(is_expression_terminated()) return left;
		tok = tok_iter.peek();
	}
	return left;
}

}