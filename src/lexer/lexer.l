%option noyywrap
%option yylineno
%option reentrant

%{

#include <fstream>
#include "./lexer/lexer.hpp"

#define YY_DECL int Lexer::_get_next_token(yyscan_t yyscanner)
#define YY_INPUT(buf, result, max_size)                                             \
	std::ifstream *_in = reinterpret_cast<std::ifstream *>(yyget_extra(yyscanner)); \
	_in->read(buf, max_size);                                                       \
	result = _in->gcount();

%}

%%

"=" 					return static_cast<int>(Token::TK_ASSIGN);
"+" 					return static_cast<int>(Token::OP_ADD);
"-" 					return static_cast<int>(Token::OP_SUB);
"*" 					return static_cast<int>(Token::OP_MUL);
"/" 					return static_cast<int>(Token::OP_DIV);
"(" 					return static_cast<int>(Token::TK_OPENPAR);
")" 					return static_cast<int>(Token::TK_CLOSEPAR);
";" 					return static_cast<int>(Token::TK_SEMICOLON);
[\n ]+ 					;
<<EOF>> 				yyterminate();
.						return static_cast<int>(Token::TK_UNKNOWN);

%%

Lexer::Lexer(std::ifstream &in) : in(in)
{
	yylex_init_extra(&this->in, &this->scanner);
}

Lexer::~Lexer()
{
	yylex_destroy(this->scanner);
}

std::string Lexer::get_lexeme()
{
	return yyget_text(this->scanner);
}

int Lexer::get_line()
{
	return yyget_lineno(this->scanner);
}
