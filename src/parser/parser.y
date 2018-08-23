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

program						:	eols.opt
								type-definition-section.opt
								variable-section.opt
								subprogram-decl.opt
																							{}
;
eols.opt 					: 	%empty														{}
							| 	eols
;
eols						:	eols '\n'													{}
							|	'\n'														{}
;
type-definition-section.opt	:	%empty														{}
							|	type-definition-section eols								{}
;
type-definition-section		:	type-definition-section eols "tipo" TK_ID "es" type			{}
							|	"tipo" TK_ID "es" type										{}
;
type						:	"entero"													{}
							|	"booleano"													{}
							|	"caracter"													{}
							|	TK_ID														{}
							|	array-type													{}
;
array-type					:	"arreglo" '[' TK_NUMBER ',' TK_NUMBER ']' "de" type			{}
;
variable-section.opt		:	%empty														{}
							|	variable-section eols										{}
;
variable-section			:	variable-section eols type id-list							{}
							|	type id-list												{}
;
id-list						:	id-list	',' TK_ID											{}
							|	TK_ID														{}
;
subprogram-decl.opt			:	%empty														{}
							|	subprogram-decl-list eols									{}
;
subprogram-decl-list		:	subprogram-decl-list eols subprogram-decl					{}
							|	subprogram-decl
;
subprogram-decl				:	subprogram-header
								eols
								variable-section.opt
								"inicio"
								eols.opt
								"fin"														{ /* In progress */ }
;
subprogram-header			:	function-header												{}
							|	procedure-header											{}
;
function-header				:	"funcion" TK_ID arguments.opt ':' type						{}
;
arguments.opt				:	%empty														{}
							|	'(' argument-decl-list ')'									{}
;
argument-decl-list			:	argument-decl-list ',' type TK_ID							{}
							|	type TK_ID
;
procedure-header			:	"procedimiento" TK_ID arguments.opt 						{}
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
