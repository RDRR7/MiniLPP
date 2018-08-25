%code requires {

#include <iostream>
#include "lexer.hpp"
#include "nodes.hpp"

#define YYSTYPE_IS_DECLARED 1
using YYSTYPE = ASTNode *;

}

%{
#include "parser.hpp"

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

start						:	program												{ std::cout << $1->to_string() << std::endl; }
;
program						:	eols.opt
								type-definition-section.opt
								variable-section.opt
								subprogram-decl.opt
								"inicio"
								eols.opt
								statement-list.opt
								"fin"
								eols.opt											{
																						$$ = new ProgramNode($2,
																											 $3,
																											 $4,
																											 $7);
																					}
;
eols.opt 					: 	%empty												{ $$ = NULL; }
							| 	eols
;
eols						:	eols '\n'											{ $$ = NULL; }
							|	'\n'												{ $$ = NULL; }
;
type-definition-section.opt	:	%empty												{ $$ = NULL; }
							|	type-definition-section eols						{ $$ = $1; }
;
type-definition-section		:	type-definition-section eols "tipo" TK_ID "es" type	{
																						std::list<ASTNode *> type_definitions;
																						type_definitions.push_back($1);
																						type_definitions.push_back(new TypeDefinition($4, $6));
																						$$ = new TypeDefinitionSection(type_definitions);
																					}
							|	"tipo" TK_ID "es" type								{ $$ = new TypeDefinition($2, $4); }
;
type						:	"entero"											{ $$ = new Type(TypeEnum::Entero, NULL, NULL, NULL); }
							|	"booleano"											{ $$ = new Type(TypeEnum::Booleano, NULL, NULL, NULL); }
							|	"caracter"											{ $$ = new Type(TypeEnum::Caracter, NULL, NULL, NULL); }
							|	TK_ID												{ $$ = new Type(TypeEnum::Tipo, NULL, $1, NULL); }
							|	array-type											{ $$ = $1; }
;
array-type					:	"arreglo" '[' TK_NUMBER ']' "de" type				{ $$ = new Type(TypeEnum::Arreglo, $3, NULL, $6); }
;
variable-section.opt		:	%empty												{ $$ = NULL; }
							|	variable-section eols								{ $$ = $1; }
;
variable-section			:	variable-section eols type id-list					{
																						std::list<ASTNode *> variable_sections;
																						variable_sections.push_back($1);
																						variable_sections.push_back(new VariableSection($4, $3));
																						$$ = new VariableSectionList(variable_sections);
																					}
							|	type id-list										{ $$ = new VariableSection($2, $1); }
;
id-list						:	id-list	',' TK_ID									{
																						std::list<ASTNode *> ids;
																						ids.push_back($1);
																						ids.push_back($3);
																						$$ = new IdList(ids);
																					}
							|	TK_ID												{ $$ = $1; }
;
subprogram-decl.opt			:	%empty												{ $$ = NULL; }
							|	subprogram-decl-list eols							{ $$ = NULL; }
;
subprogram-decl-list		:	subprogram-decl-list eols subprogram-decl			{ $$ = NULL; }
							|	subprogram-decl										{ $$ = NULL; }
;
subprogram-decl				:	subprogram-header
								eols
								variable-section.opt
								"inicio"
								eols.opt
								statement-list.opt
								"fin"												{ $$ = NULL; }
;
subprogram-header			:	function-header										{ $$ = NULL; }
							|	procedure-header									{ $$ = NULL; }
;
function-header				:	"funcion" TK_ID arguments.opt ':' type				{ $$ = NULL; }
;
arguments.opt				:	%empty												{ $$ = NULL; }
							|	'(' argument-decl-list ')'							{ $$ = NULL; }
;
argument-decl-list			:	argument-decl-list ',' type TK_ID					{ $$ = NULL; }
							|	argument-decl-list ',' "var" type TK_ID				{ $$ = NULL; }
							|	type TK_ID											{ $$ = NULL; }
							|	"var" type TK_ID									{ $$ = NULL; }
;
procedure-header			:	"procedimiento" TK_ID arguments.opt 				{ $$ = NULL; }
;
statement-list.opt			:	%empty												{ $$ = NULL; }
							|	statement-list eols 								{ $$ = NULL; }
;
statement-list				:	statement-list eols statement						{ $$ = NULL; }
							|	statement											{ $$ = NULL; }
;
statement					:	assign												{ $$ = NULL; }
							|	"llamar" subprogram-call							{ $$ = NULL; }
							|	"llamar" TK_ID										{ $$ = NULL; }
							|	"si" expr "entonces"
								eols.opt
								statement-list.opt
								else.opt
								"fin" "si"											{ $$ = NULL; }
							|	"mientras" expr "haga"
								eols.opt
								statement-list.opt
								"fin" "mientras"									{ $$ = NULL; }
							|	"mientras" "no" expr "haga"
								eols.opt
								statement-list.opt
								"fin" "mientras"									{ $$ = NULL; }
							|	"para" assign "hasta" expr "haga"
								eols.opt
								statement-list.opt
								"fin" "para"										{ $$ = NULL; }
							|	"repita"
								eols.opt
								statement-list.opt
								"hasta" expr										{ $$ = NULL; }
							|	"retorne" expr										{ $$ = NULL; }
							|	"escriba" argument-list-with-string					{ $$ = NULL; }
							|	"lea" lvalue										{ $$ = NULL; }
;
argument-list-with-string	:	argument-list-with-string ',' expr					{ $$ = NULL; }
							|	argument-list-with-string ',' TK_STRING_LITERAL		{ $$ = NULL; }
							|	expr												{ $$ = NULL; }
							|	TK_STRING_LITERAL									{ $$ = NULL; }
;
else.opt					:	%empty												{ $$ = NULL; }
							|	"sino"
								eols.opt
								statement-list.opt									{ $$ = NULL; }
;
assign						:	lvalue "<-" expr									{ $$ = NULL; }
;
lvalue						:	TK_ID												{ $$ = NULL; }
							|	TK_ID '[' expr ']'									{ $$ = NULL; }
;
expr						:	expr '=' term										{ $$ = NULL; }
							|	expr "<>" term										{ $$ = NULL; }
							|	expr '<' term										{ $$ = NULL; }
							|	expr '>' term										{ $$ = NULL; }
							|	expr ">=" term										{ $$ = NULL; }
							|	expr "<=" term										{ $$ = NULL; }
							|	term												{ $$ = NULL; }
;
term						:	term '+' factor										{ $$ = NULL; }
							|	term '-' factor										{ $$ = NULL; }
							|	term 'o' factor										{ $$ = NULL; }
							|	factor												{ $$ = NULL; }
;
factor						:	factor '*' exponent									{ $$ = NULL; }
							|	factor "div" exponent								{ $$ = NULL; }
							|	factor "mod" exponent								{ $$ = NULL; }
							|	factor 'y' exponent									{ $$ = NULL; }
							|	exponent
;
exponent					:	exponent '^' rvalue									{ $$ = NULL; }
							|	rvalue												{ $$ = NULL; }
;
rvalue						:	'(' expr ')'										{ $$ = NULL; }
							|	constant											{ $$ = NULL; }
							|	'-' expr											{ $$ = NULL; }
							|	lvalue												{ $$ = NULL; }
							|	subprogram-call										{ $$ = NULL; }
;
constant					:	TK_NUMBER											{ $$ = NULL; }
							|	TK_CHARACTER_LITERAL								{ $$ = NULL; }
							|	"verdadero"											{ $$ = NULL; }
							|	"falso"												{ $$ = NULL; }
;
subprogram-call				:	TK_ID '(' argument-list.opt ')'						{ $$ = NULL; }
;
argument-list.opt			:	%empty												{ $$ = NULL; }
							|	argument-list										{ $$ = NULL; }
;
argument-list				:	argument-list ',' expr								{ $$ = NULL; }
							|	expr												{ $$ = NULL; }
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
