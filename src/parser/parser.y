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

void yyerror(ASTNode **program_node,
			 const char *msg);
int yylex();

%}

%defines
%define parse.lac full
%define parse.error verbose
%expect 54
%parse-param {ASTNode **program_node}

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

start						:	program												{ *program_node = $1; }
;
program						:	eols.opt
								type-definition-section.opt
								variable-section.opt
								subprogram-decl.opt
								"inicio"
								eols.opt
								statement-list.opt
								"fin"
								eols.opt											{ $$ = new ProgramNode(lexer.get_line(), $2, $3, $4, $7); }
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
																						$$ = $1;
																						ASTNodeList::add_to_list($$, new TypeDefinition(lexer.get_line(), $4, $6));
																					}
							|	"tipo" TK_ID "es" type								{
																						$$ = new TypeDefinitionList(lexer.get_line());
																						ASTNodeList::add_to_list($$, new TypeDefinition(lexer.get_line(), $2, $4));
																					}
;
type						:	"entero"											{ $$ = new Type(lexer.get_line(), TypeEnum::Entero, NULL, NULL, NULL); }
							|	"booleano"											{ $$ = new Type(lexer.get_line(), TypeEnum::Booleano, NULL, NULL, NULL); }
							|	"caracter"											{ $$ = new Type(lexer.get_line(), TypeEnum::Caracter, NULL, NULL, NULL); }
							|	TK_ID												{ $$ = new Type(lexer.get_line(), TypeEnum::Tipo, NULL, $1, NULL); }
							|	array-type											{ $$ = $1; }
;
array-type					:	"arreglo" '[' TK_NUMBER ']' "de" type				{ $$ = new Type(lexer.get_line(), TypeEnum::Arreglo, $3, NULL, $6); }
;
variable-section.opt		:	%empty												{ $$ = NULL; }
							|	variable-section eols								{ $$ = $1; }
;
variable-section			:	variable-section eols type id-list					{
																						$$ = $1;
																						ASTNodeList::add_to_list($$, new VariableSection(lexer.get_line(), $4, $3));
																					}
							|	type id-list										{
																						$$ = new VariableSectionList(lexer.get_line());
																						ASTNodeList::add_to_list($$, new VariableSection(lexer.get_line(), $2, $1));
																					}
;
id-list						:	id-list	',' TK_ID									{
																						$$ = $1;
																						ASTNodeList::add_to_list($$, $3);
																					}
							|	TK_ID												{
																						$$ = new IdList(lexer.get_line());
																						ASTNodeList::add_to_list($$, $1);
																					}
;
subprogram-decl.opt			:	%empty												{ $$ = NULL; }
							|	subprogram-decl-list eols							{ $$ = $1; }
;
subprogram-decl-list		:	subprogram-decl-list eols subprogram-decl			{
																						$$ = $1;
																						ASTNodeList::add_to_list($$, $3);
																					}
							|	subprogram-decl										{
																						$$ = new SubprogramDeclList(lexer.get_line());
																						ASTNodeList::add_to_list($$, $1);
																					}
;
subprogram-decl				:	subprogram-header
								eols
								variable-section.opt
								"inicio"
								eols.opt
								statement-list.opt
								"fin"												{ $$ = new SubprogramDecl(lexer.get_line(), $1, $3, $6); }
;
subprogram-header			:	function-header										{ $$ = $1; }
							|	procedure-header									{ $$ = $1; }
;
function-header				:	"funcion" TK_ID arguments.opt ':' type				{ $$ = new SubprogramDeclHeader(lexer.get_line(), $2, $3, $5); }
;
arguments.opt				:	%empty												{ $$ = NULL; }
							|	'(' argument-decl-list ')'							{ $$ = $2; }
;
argument-decl-list			:	argument-decl-list ',' type TK_ID					{
																						$$ = $1;
																						ASTNodeList::add_to_list($$, new ArgumentDecl(lexer.get_line(), $3, $4, false));
																					}
							|	argument-decl-list ',' "var" type TK_ID				{
																						$$ = $1;
																						ASTNodeList::add_to_list($$, new ArgumentDecl(lexer.get_line(), $4, $5, true));
							 														}
							|	type TK_ID											{
																						$$ = new ArgumentDeclList(lexer.get_line());
																						ASTNodeList::add_to_list($$, new ArgumentDecl(lexer.get_line(), $1, $2, false));
																					}
							|	"var" type TK_ID									{
																						$$ = new ArgumentDeclList(lexer.get_line());
																						ASTNodeList::add_to_list($$, new ArgumentDecl(lexer.get_line(), $2, $3, true));
																					}
;
procedure-header			:	"procedimiento" TK_ID arguments.opt 				{ $$ = new SubprogramDeclHeader(lexer.get_line(), $2, $3, NULL); }
;
statement-list.opt			:	%empty												{ $$ = NULL; }
							|	statement-list eols 								{ $$ = $1; }
;
statement-list				:	statement-list eols statement						{
																						$$ = $1;
																						ASTNodeList::add_to_list($$, $3);
																					}
							|	statement											{
																						$$ = new StatementList(lexer.get_line());
																						ASTNodeList::add_to_list($$, $1);
																					}
;
statement					:	assign												{ $$ = $1; }
							|	"llamar" subprogram-call							{ $$ = new CallStatement(lexer.get_line(), $2); }
							|	"llamar" TK_ID										{ $$ = new CallStatement(lexer.get_line(), $2); }
							|	"si" expr eols.opt "entonces"
								eols.opt
								statement-list.opt
								else.opt
								"fin" "si"											{ $$ = new IfStatement(lexer.get_line(), $2, $6, $7); }
							|	"mientras" expr "haga"
								eols.opt
								statement-list.opt
								"fin" "mientras"									{ $$ = new WhileStatement(lexer.get_line(), $2, $5); }
							|	"para" assign "hasta" expr "haga"
								eols.opt
								statement-list.opt
								"fin" "para"										{ $$ = new ForStatement(lexer.get_line(), $2, $4, $7); }
							|	"repita"
								eols.opt
								statement-list.opt
								"hasta" expr										{ $$ = new NotDoWhileStatement(lexer.get_line(), $5, $3); }
							|	"retorne" expr										{ $$ = new ReturnNode(lexer.get_line(), $2); }
							|	"escriba" argument-list-with-string					{ $$ = new WriteNode(lexer.get_line(), $2); }
							|	"lea" lvalue										{ $$ = new ReadNode(lexer.get_line(), $2); }
;
argument-list-with-string	:	argument-list-with-string ',' expr					{
																						$$ = $1;
																						ASTNodeList::add_to_list($$, $3);
																					}
							|	argument-list-with-string ',' TK_STRING_LITERAL		{
																						$$ = $1;
																						ASTNodeList::add_to_list($$, $3);
							 														}
							|	expr												{
																						$$ = new ArgumentList(lexer.get_line());
																						ASTNodeList::add_to_list($$, $1);
																					}
							|	TK_STRING_LITERAL									{
																						$$ = new ArgumentList(lexer.get_line());
																						ASTNodeList::add_to_list($$, $1);
																					}
;
else.opt					:	%empty												{ $$ = NULL; }
							|	"sino"
								eols.opt
								statement-list.opt									{ $$ = new ElseStatement(lexer.get_line(), $3); }
;
assign						:	lvalue "<-" expr									{ $$ = new AssignStatement(lexer.get_line(), $1, $3); }
;
lvalue						:	TK_ID												{ $$ = new LeftValue(lexer.get_line(), $1, NULL); }
							|	TK_ID '[' expr ']'									{ $$ = new LeftValue(lexer.get_line(), $1, $3); }
;
expr						:	expr '=' term										{ $$ = new EqualExpr(lexer.get_line(), $1, $3); }
							|	expr "<>" term										{ $$ = new NotEqualExpr(lexer.get_line(), $1, $3); }
							|	expr '<' term										{ $$ = new LessThanExpr(lexer.get_line(), $1, $3); }
							|	expr '>' term										{ $$ = new GreaterThanExpr(lexer.get_line(), $1, $3); }
							|	expr "<=" term										{ $$ = new LessThanOrEqualExpr(lexer.get_line(), $1, $3); }
							|	expr ">=" term										{ $$ = new GreaterThanOrEqualExpr(lexer.get_line(), $1, $3); }
							|	term												{ $$ = $1; }
;
term						:	term '+' factor										{ $$ = new AdditionExpr(lexer.get_line(), $1, $3); }
							|	term '-' factor										{ $$ = new SubtractionExpr(lexer.get_line(), $1, $3); }
							|	term 'o' factor										{ $$ = new OrExpr(lexer.get_line(), $1, $3); }
							|	factor												{ $$ = $1; }
;
factor						:	factor '*' exponent									{ $$ = new MultiplicationExpr(lexer.get_line(), $1, $3); }
							|	factor "div" exponent								{ $$ = new DivisionExpr(lexer.get_line(), $1, $3); }
							|	factor "mod" exponent								{ $$ = new ModExpr(lexer.get_line(), $1, $3); }
							|	factor 'y' exponent									{ $$ = new AndExpr(lexer.get_line(), $1, $3); }
							|	exponent											{ $$ = $1; }
;
exponent					:	exponent '^' rvalue									{ $$ = new PowerExpr(lexer.get_line(), $1, $3); }
							|	rvalue												{ $$ = $1; }
;
rvalue						:	'(' expr ')'										{ $$ = $2; }
							|	constant											{ $$ = $1; }
							|	'-' expr											{ $$ = new NegativeExpr(lexer.get_line(), $2); }
							|	lvalue												{ $$ = $1; }
							|	subprogram-call										{ $$ = $1; }
							|	"no" expr											{ $$ = new NegateExpr(lexer.get_line(), $2); }
;
constant					:	TK_NUMBER											{ $$ = $1; }
							|	TK_CHARACTER_LITERAL								{ $$ = $1; }
							|	"verdadero"											{ $$ = new BooleanNode(lexer.get_line(), true); }
							|	"falso"												{ $$ = new BooleanNode(lexer.get_line(), false); }
;
subprogram-call				:	TK_ID '(' argument-list.opt ')'						{ $$ = new SubprogramCall(lexer.get_line(), $1, $3); }
;
argument-list.opt			:	%empty												{ $$ = NULL; }
							|	argument-list										{ $$ = $1; }
;
argument-list				:	argument-list ',' expr								{
																						$$ = $1;
																						ASTNodeList::add_to_list($$, $3);
																					}
							|	expr												{
																						$$ = new ArgumentList(lexer.get_line());
																						ASTNodeList::add_to_list($$, $1);
																					}
;

%%

void yyerror(ASTNode **program_node,
			 const char *msg)
{
	std::cerr << "[line " << lexer.get_line() << "]" << msg << std::endl;
}

int yylex()
{
	return lexer.get_next_token();
}
