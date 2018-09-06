%option noyywrap
%option yylineno
%option reentrant
%option case-insensitive
%option extra-type = "std::ifstream *"

%{

#include <fstream>
#include <algorithm>
#include "lexer.hpp"
#include "parser.hpp"

#define YY_DECL int Lexer::_get_next_token(yyscan_t yyscanner) const
#define YY_INPUT(buf, result, max_size)          \
	std::ifstream *_in = yyget_extra(yyscanner); \
	_in->read(buf, max_size);                    \
	result = _in->gcount();

%}

%x LINE_COMMENT
%x BLOCK_COMMENT

%%

"inicio"						return static_cast<int>(yytokentype::KW_INICIO);
"escriba"						return static_cast<int>(yytokentype::KW_ESCRIBA);
"llamar"						return static_cast<int>(yytokentype::KW_LLAMAR);
"fin"							return static_cast<int>(yytokentype::KW_FIN);
"entero"						return static_cast<int>(yytokentype::KW_ENTERO);
"caracter"						return static_cast<int>(yytokentype::KW_CARACTER);
"booleano"						return static_cast<int>(yytokentype::KW_BOOLEANO);
"mod"							return static_cast<int>(yytokentype::KW_MOD);
"div"							return static_cast<int>(yytokentype::KW_DIV);
"lea"							return static_cast<int>(yytokentype::KW_LEA);
"si"							return static_cast<int>(yytokentype::KW_SI);
"sino"							return static_cast<int>(yytokentype::KW_SINO);
"entonces"						return static_cast<int>(yytokentype::KW_ENTONCES);
"mientras"						return static_cast<int>(yytokentype::KW_MIENTRAS);
"para"							return static_cast<int>(yytokentype::KW_PARA);
"hasta"							return static_cast<int>(yytokentype::KW_HASTA);
"haga"							return static_cast<int>(yytokentype::KW_HAGA);
"repita"						return static_cast<int>(yytokentype::KW_REPITA);
"procedimiento"					return static_cast<int>(yytokentype::KW_PROCEDIMIENTO);
"var"							return static_cast<int>(yytokentype::KW_VAR);
"funcion"						return static_cast<int>(yytokentype::KW_FUNCION);
"arreglo"						return static_cast<int>(yytokentype::KW_ARREGLO);
"de"							return static_cast<int>(yytokentype::KW_DE);
"retorne"						return static_cast<int>(yytokentype::KW_RETORNE);
"tipo"							return static_cast<int>(yytokentype::KW_TIPO);
"es"							return static_cast<int>(yytokentype::KW_ES);
"verdadero"						return static_cast<int>(yytokentype::KW_VERDADERO);
"falso"							return static_cast<int>(yytokentype::KW_FALSO);
"no"							return static_cast<int>(yytokentype::KW_NO);

\'.\' 							yylval = new CharacterLiteralNode(get_line(), get_lexeme()); return static_cast<int>(yytokentype::TK_CHARACTER_LITERAL);
\".*\" 							yylval = new StringLiteralNode(get_line(), get_lexeme()); return static_cast<int>(yytokentype::TK_STRING_LITERAL);

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

[0-9]+							yylval = new NumberNode(get_line(), std::stoi(get_lexeme())); return static_cast<int>(yytokentype::TK_NUMBER);
[_a-z][_a-z0-9]*				yylval = new StringNode(get_line(), get_lexeme()); return static_cast<int>(yytokentype::TK_ID);
"<-"							return static_cast<int>(yytokentype::TK_ASSIGN);
">="							return static_cast<int>(yytokentype::TK_GToE);
"<="							return static_cast<int>(yytokentype::TK_LToE);
"<>"							return static_cast<int>(yytokentype::TK_NEQU);

[ \t] 							;
\r?\n							return static_cast<int>(Token::TK_EOL);
<INITIAL,LINE_COMMENT><<EOF>>	yyterminate();
.								return static_cast<int>(Token::TK_UNKNOWN);

<LINE_COMMENT>.					;
<LINE_COMMENT>\r?\n 			BEGIN(INITIAL); return static_cast<int>(Token::TK_EOL);

<BLOCK_COMMENT>(.|\r?\n)		;
<BLOCK_COMMENT><<EOF>>			YY_FATAL_ERROR("Unclosed block comment");
<BLOCK_COMMENT>"*/"				BEGIN(INITIAL);

%%

Lexer::Lexer(std::ifstream &in)
	: in(in)
{
	yylex_init_extra(&in, &scanner);
}

Lexer::~Lexer()
{
	yylex_destroy(scanner);
}

std::string Lexer::get_lexeme() const
{
	std::string lexeme = yyget_text(scanner);
	std::transform(lexeme.begin(), lexeme.end(), lexeme.begin(), ::tolower);
	return lexeme;
}

int Lexer::get_line() const
{
	return yyget_lineno(scanner);
}
