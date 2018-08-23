%{

#include <iostream>
#include "lexer.hpp"

extern std::ifstream in;
Lexer lexer(in);

void yyerror(const char *msg);
int yylex();

%}

%defines
%define parse.lac full
%define parse.error verbose

%token TK_NUMBER "numero"
%token TK_ID "id"
%token TK_STRING_LITERAL "cadena"
%token TK_CHARACTER_LITERAL "character"

%token TK_ASSIGN
%token TK_GToE
%token TK_LToE

%token EOL

%token KW_INICIO "inicio"
%token KW_ESCRIBA "escriba"
%token KW_LLAMAR "llamar"
%token KW_FIN "fin"
%token KW_ENTERO "entero"
%token KW_CARACTER "caracter"
%token KW_BOOLEANO "booleano"
%token KW_MOD "mod"
%token KW_DIV "div"
%token KW_LEA "lea"
%token KW_SI "si"
%token KW_SINO "sino"
%token KW_ENTONCES "entonces"
%token KW_MIENTRAS "mientras"
%token KW_PARA "para"
%token KW_HASTA "hasta"
%token KW_HAGA "haga"
%token KW_REPITA "repita"
%token KW_PROCEDIMIENTO "procedimiento"
%token KW_VAR "var"
%token KW_FUNCION "funcion"
%token KW_ARREGLO "arreglo"
%token KW_DE "de"
%token KW_RETORNE "retorne"
%token KW_TIPO "tipo"
%token KW_ES "es"

%%

statements				:	statements statement		{}
						|	statement					{}
;
statement				:	assignment_statement		{}
;
assignment_statement	:	TK_ID '=' expression ';'	{}
;
expression				:	expression '+' term			{}
						|	expression '-' term			{}
						| 	term						{}
;
term 					: 	term '*' factor 			{}
						| 	term '/' factor 			{}
						|	factor 						{}
;
factor					:	'(' expression ')'			{}
						|	TK_NUMBER					{}
						|	TK_ID						{}
;

%%

void yyerror(const char *msg)
{
	std::cerr << "[line " << lexer.get_line() << "]" << msg <<std::endl;
}

int yylex()
{
	return lexer.get_next_token();
}
