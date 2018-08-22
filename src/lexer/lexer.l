%option noyywrap
%option yylineno
%option reentrant
%option case-insensitive
%option extra-type = "std::ifstream *"

%{

#include <fstream>
#include "./lexer/lexer.hpp"

#define YY_DECL int Lexer::_get_next_token(yyscan_t yyscanner)
#define YY_INPUT(buf, result, max_size)          \
	std::ifstream *_in = yyget_extra(yyscanner); \
	_in->read(buf, max_size);                    \
	result = _in->gcount();

%}

%x LINE_COMMENT
%x BLOCK_COMMENT

%%

"inicio"						return 1;
"escriba"						return 1;
"llamar"						return 1;
"fin"							return 1;
"entero"						return 1;
"caracter"						return 1;
"booleano"						return 1;
"mod"							return 1;
"div"							return 1;
"lea"							return 1;
"si"							return 1;
"sino"							return 1;
"entonces"						return 1;
"mientras"						return 1;
"para"							return 1;
"hasta"							return 1;
"haga"							return 1;
"repita"						return 1;
"procedimiento"					return 1;
"var"							return 1;
"funcion"						return 1;
"arreglo"						return 1;
"de"							return 1;
"retorne"						return 1;
"tipo"							return 1;
"es"							return 1;

\'.\' 							return 1;
\".*\" 							return 1;

"+"								return static_cast<int>(Token::OP_ADD);
"-"								return static_cast<int>(Token::OP_SUB);
"*"								return static_cast<int>(Token::OP_MUL);
"/"								return static_cast<int>(Token::OP_DIV);
"^"								return static_cast<int>(Token::OP_EXP);
"y"								return static_cast<int>(Token::OP_AND);
"o"								return static_cast<int>(Token::OP_OR);
"["								return static_cast<int>(Token::TK_OPENBRA);
"]"								return static_cast<int>(Token::TK_CLOSEBRA);
"("								return static_cast<int>(Token::TK_OPENPAR);
")"								return static_cast<int>(Token::TK_CLOSEPAR);
">" 							return static_cast<int>(Token::TK_GT);
"<" 							return static_cast<int>(Token::TK_LT);
":" 				 			return static_cast<int>(Token::TK_COLON);
"=" 							return static_cast<int>(Token::TK_EQUAL);
","								return static_cast<int>(Token::TK_COMMA);
";" 				 			return static_cast<int>(Token::TK_SEMICOLON);

"//" 							BEGIN(LINE_COMMENT);
"/*" 							BEGIN(BLOCK_COMMENT);

[0-9]+							return 1;
[_a-z][_a-z0-9]*				return 1;
"<-"							return 1;
">="							return 1;
"<="							return 1;

[ ] 							;
\n								return 1;
<INITIAL,LINE_COMMENT><<EOF>>	yyterminate();
.								return static_cast<int>(Token::TK_UNKNOWN);

<LINE_COMMENT>.					;
<LINE_COMMENT>\n 				BEGIN(INITIAL);

<BLOCK_COMMENT>(.|\n)			;
<BLOCK_COMMENT><<EOF>>			YY_FATAL_ERROR("Unclosed block comment");
<BLOCK_COMMENT>"*/"				BEGIN(INITIAL);

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
