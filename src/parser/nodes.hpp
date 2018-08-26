#ifndef NODES_H
#define NODES_H

#include <iostream>
#include <string>
#include <list>

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
	std::string to_string() override { return id; }

	std::string id;
};

class NumberNode : public ASTNode
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

#endif
