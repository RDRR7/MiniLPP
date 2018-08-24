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
%expect 48

%token TK_NUMBER			"numero"
%token TK_ID				"id"
%token TK_STRING_LITERAL	"cadena"
%token TK_CHARACTER_LITERAL "character"

%token TK_ASSIGN			"<-"
%token TK_GToE				">="
%token TK_LToE				"<="
%token TK_NEQU				"<>"

%token KW_INICIO			"inicio"
%token KW_ESCRIBA			"escriba"
%token KW_LLAMAR			"llamar"
%token KW_FIN				"fin"
%token KW_ENTERO			"entero"
%token KW_CARACTER			"caracter"
%token KW_BOOLEANO			"booleano"
%token KW_MOD				"mod"
%token KW_DIV				"div"
%token KW_LEA				"lea"
%token KW_SI				"si"
%token KW_SINO				"sino"
%token KW_ENTONCES			"entonces"
%token KW_MIENTRAS			"mientras"
%token KW_PARA				"para"
%token KW_HASTA				"hasta"
%token KW_HAGA				"haga"
%token KW_REPITA			"repita"
%token KW_PROCEDIMIENTO		"procedimiento"
%token KW_VAR				"var"
%token KW_FUNCION			"funcion"
%token KW_ARREGLO			"arreglo"
%token KW_DE				"de"
%token KW_RETORNE			"retorne"
%token KW_TIPO				"tipo"
%token KW_ES				"es"
%token KW_VERDADERO			"verdadero"
%token KW_FALSO				"falso"
%token KW_NO				"no"

%%

program						:	eols.opt
								type-definition-section.opt
								variable-section.opt
								subprogram-decl.opt
								"inicio"
								eols.opt
								statement-list.opt
								"fin"
								eols.opt													{}
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
array-type					:	"arreglo" '[' TK_NUMBER ']' "de" type						{}
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
							|	subprogram-decl												{}
;
subprogram-decl				:	subprogram-header
								eols
								variable-section.opt
								"inicio"
								eols.opt
								statement-list.opt
								"fin"														{}
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
							|	argument-decl-list ',' "var" type TK_ID						{}
							|	type TK_ID													{}
							|	"var" type TK_ID											{}
;
procedure-header			:	"procedimiento" TK_ID arguments.opt 						{}
;
statement-list.opt			:	%empty														{}
							|	statement-list eols 										{}
;
statement-list				:	statement-list eols statement								{}
							|	statement													{}
;
statement					:	assign														{}
							|	"llamar" subprogram-call									{}
							|	"llamar" TK_ID												{}
							|	"si" expr "entonces"
								eols.opt
								statement-list.opt
								else.opt
								"fin" "si"													{}
							|	"mientras" expr "haga"
								eols.opt
								statement-list.opt
								"fin" "mientras"											{}
							|	"mientras" "no" expr "haga"
								eols.opt
								statement-list.opt
								"fin" "mientras"											{}
							|	"para" assign "hasta" expr "haga"
								eols.opt
								statement-list.opt
								"fin" "para"												{}
							|	"repita"
								eols.opt
								statement-list.opt
								"hasta" expr												{}
							|	"retorne" expr												{}
							|	"escriba" argument-list-with-string							{}
							|	"lea" lvalue												{}
;
argument-list-with-string	:	argument-list-with-string ',' expr							{}
							|	argument-list-with-string ',' TK_STRING_LITERAL				{}
							|	expr														{}
							|	TK_STRING_LITERAL											{}
;
else.opt					:	%empty														{}
							|	"sino"
								eols.opt
								statement-list.opt											{}
;
assign						:	lvalue "<-" expr											{}
;
lvalue						:	TK_ID														{}
							|	TK_ID '[' expr ']'											{}
;
expr						:	expr '=' term												{}
							|	expr "<>" term												{}
							|	expr '<' term												{}
							|	expr '>' term												{}
							|	expr ">=" term												{}
							|	expr "<=" term												{}
							|	term														{}
;
term						:	term '+' factor												{}
							|	term '-' factor												{}
							|	term 'o' factor												{}
							|	factor														{}
;
factor						:	factor '*' exponent											{}
							|	factor "div" exponent										{}
							|	factor "mod" exponent										{}
							|	factor 'y' exponent											{}
							|	exponent
;
exponent					:	exponent '^' rvalue											{}
							|	rvalue														{}
;
rvalue						:	'(' expr ')'												{}
							|	constant													{}
							|	'-' expr													{}
							|	lvalue														{}
							|	subprogram-call												{}
;
constant					:	TK_NUMBER													{}
							|	TK_CHARACTER_LITERAL										{}
							|	"verdadero"													{}
							|	"falso"														{}
;
subprogram-call				:	TK_ID '(' argument-list.opt ')'								{}
;
argument-list.opt			:	%empty														{}
							|	argument-list												{}
;
argument-list				:	argument-list ',' expr										{}
							|	expr														{}
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
