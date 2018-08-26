#ifndef NODES_H
#define NODES_H

#include <iostream>
#include <string>
#include <list>

#define DEFINE_BINARY_EXPR(name, prec, oper)       \
	class name##Expr : public BinaryExpr           \
	{                                              \
	  public:                                      \
		name##Expr(ASTNode *expr1, ASTNode *expr2) \
			: BinaryExpr(expr1, expr2) {}          \
		int get_precedence()                       \
		{                                          \
			return prec;                           \
		}                                          \
		std::string get_oper()                     \
		{                                          \
			return oper;                           \
		}                                          \
	};

enum class TypeEnum : unsigned int
{
	Entero = 1,
	Booleano = 2,
	Caracter = 3,
	Arreglo = 4,
	Tipo = 5
};

class ASTNode
{
  public:
	ASTNode() {}
	virtual std::string to_string() = 0;
};

class ProgramNode : public ASTNode
{
  public:
	ProgramNode(ASTNode *type_definition_section,
				ASTNode *variable_section,
				ASTNode *subprogram_decl,
				ASTNode *statement_list)
		: type_definition_section(type_definition_section),
		  variable_section(variable_section),
		  subprogram_decl(subprogram_decl),
		  statement_list(statement_list) {}
	std::string to_string() override;

	ASTNode *type_definition_section;
	ASTNode *variable_section;
	ASTNode *subprogram_decl;
	ASTNode *statement_list;
};

class TypeDefinitionSection : public ASTNode
{
  public:
	TypeDefinitionSection(std::list<ASTNode *> type_definitions)
		: type_definitions(type_definitions) {}
	std::string to_string() override;

	std::list<ASTNode *> type_definitions;
};

class TypeDefinition : public ASTNode
{
  public:
	TypeDefinition(ASTNode *id, ASTNode *type)
		: id(id), type(type) {}
	std::string to_string();

	ASTNode *id;
	ASTNode *type;
};

class Type : public ASTNode
{
  public:
	Type(TypeEnum type,
		 ASTNode *array_size,
		 ASTNode *user_type,
		 ASTNode *array_type)
		: type(type),
		  array_size(array_size),
		  user_type(user_type),
		  array_type(array_type) {}
	std::string to_string() override;

	TypeEnum type;
	ASTNode *array_size;
	ASTNode *user_type;
	ASTNode *array_type;
};

class StringNode : public ASTNode
{
  public:
	StringNode(std::string id)
		: id(id) {}
	std::string to_string() override
	{
		return id;
	}

	std::string id;
};

class Expr : public ASTNode
{
  public:
	Expr() {}
	virtual std::string to_string() = 0;
	std::string get_oper()
	{
		return "";
	}
	int get_precedence()
	{
		return 255;
	}
};

class NumberNode : public Expr
{
  public:
	NumberNode(int number)
		: number(number) {}
	std::string to_string() override
	{
		return std::to_string(number);
	}

	int number;
};

class VariableSectionList : public ASTNode
{
  public:
	VariableSectionList(std::list<ASTNode *> variable_sections)
		: variable_sections(variable_sections) {}
	std::string to_string();

	std::list<ASTNode *> variable_sections;
};

class VariableSection : public ASTNode
{
  public:
	VariableSection(ASTNode *ids, ASTNode *type)
		: ids(ids), type(type) {}
	std::string to_string() override;

	ASTNode *ids;
	ASTNode *type;
};

class IdList : public ASTNode
{
  public:
	IdList(std::list<ASTNode *> ids)
		: ids(ids) {}
	std::string to_string() override;

	std::list<ASTNode *> ids;
};

class SubprogramDeclList : public ASTNode
{
  public:
	SubprogramDeclList(std::list<ASTNode *> subprogram_decls)
		: subprogram_decls(subprogram_decls) {}
	std::string to_string() override;

	std::list<ASTNode *> subprogram_decls;
};

class SubprogramDecl : public ASTNode
{
  public:
	SubprogramDecl(ASTNode *header,
				   ASTNode *variable_section,
				   ASTNode *statements)
		: header(header),
		  variable_section(variable_section),
		  statements(statements) {}
	std::string to_string() override;

	ASTNode *header;
	ASTNode *variable_section;
	ASTNode *statements;
};

class SubprogramDeclHeader : public ASTNode
{
  public:
	SubprogramDeclHeader(ASTNode *id,
						 ASTNode *arguments,
						 ASTNode *type)
		: id(id),
		  arguments(arguments),
		  type(type) {}
	std::string to_string() override;

	ASTNode *id;
	ASTNode *arguments;
	ASTNode *type;
};

class ArgumentDeclList : public ASTNode
{
  public:
	ArgumentDeclList(std::list<ASTNode *> arguments_decls)
		: arguments_decls(arguments_decls) {}
	std::string to_string() override;

	std::list<ASTNode *> arguments_decls;
};

class ArgumentDecl : public ASTNode
{
  public:
	ArgumentDecl(ASTNode *type,
				 ASTNode *id,
				 bool var)
		: type(type),
		  id(id),
		  var(var) {}
	std::string to_string() override;

	ASTNode *type;
	ASTNode *id;
	bool var;
};

class StatementList : public ASTNode
{
  public:
	StatementList(std::list<ASTNode *> statements)
		: statements(statements) {}
	std::string to_string() override;

	std::list<ASTNode *> statements;
};

class AssignStatement : public ASTNode
{
  public:
	AssignStatement(ASTNode *lvalue,
					ASTNode *expr)
		: lvalue(lvalue),
		  expr(expr) {}
	std::string to_string() override;

	ASTNode *lvalue;
	ASTNode *expr;
};

class LeftValue : public Expr
{
  public:
	LeftValue(ASTNode *id,
			  ASTNode *index)
		: id(id),
		  index(index) {}
	std::string to_string() override;

	ASTNode *id;
	ASTNode *index;
};

class BinaryExpr : public Expr
{
  public:
	BinaryExpr(ASTNode *expr1,
			   ASTNode *expr2)
		: expr1(expr1),
		  expr2(expr2) {}
	std::string to_string() override;
	virtual std::string get_oper() = 0;
	virtual int get_precedence() = 0;

	ASTNode *expr1;
	ASTNode *expr2;
};

DEFINE_BINARY_EXPR(Equal, 0, "=");
DEFINE_BINARY_EXPR(NotEqual, 0, "<>");
DEFINE_BINARY_EXPR(LessThan, 0, "<");
DEFINE_BINARY_EXPR(GreaterThan, 0, ">");
DEFINE_BINARY_EXPR(LessThanOrEqual, 0, "<=");
DEFINE_BINARY_EXPR(GreaterThanOrEqual, 0, ">=");
DEFINE_BINARY_EXPR(Addition, 1, "+");
DEFINE_BINARY_EXPR(Subtraction, 1, "-");
DEFINE_BINARY_EXPR(Or, 1, "o");
DEFINE_BINARY_EXPR(Multiplication, 2, "*");
DEFINE_BINARY_EXPR(Division, 2, "div");
DEFINE_BINARY_EXPR(Mod, 2, "mod");
DEFINE_BINARY_EXPR(And, 2, "y");
DEFINE_BINARY_EXPR(Power, 3, "^");

class NegativeExpr : public Expr
{
  public:
	NegativeExpr(ASTNode *expr)
		: expr(expr) {}
	std::string to_string() override;

	ASTNode *expr;
};

class StringLiteralNode : public ASTNode
{
  public:
	StringLiteralNode(std::string string_literal)
		: string_literal(string_literal) {}
	std::string to_string() override
	{
		return string_literal;
	}

	std::string string_literal;
};

class CharacterLiteralNode : public Expr
{
  public:
	CharacterLiteralNode(std::string character_literal)
		: character_literal(character_literal) {}
	std::string to_string() override
	{
		return character_literal;
	}

	std::string character_literal;
};

class BooleanNode : public Expr
{
  public:
	BooleanNode(bool boolean)
		: boolean(boolean) {}
	std::string to_string() override;

	bool boolean;
};

class SubprogramCall : public Expr
{
  public:
	SubprogramCall(ASTNode *id,
				   ASTNode *argument_list)
		: id(id),
		  argument_list(argument_list) {}
	std::string to_string() override;

	ASTNode *id;
	ASTNode *argument_list;
};

class ArgumentList : public ASTNode
{
  public:
	ArgumentList(std::list<ASTNode *> arguments)
		: arguments(arguments) {}
	std::string to_string() override;

	std::list<ASTNode *> arguments;
};

#endif
