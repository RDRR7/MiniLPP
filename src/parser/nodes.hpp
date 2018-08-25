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
				ASTNode *statement_list) : type_definition_section(type_definition_section),
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
		 ASTNode *array_type) : type(type),
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
	std::string to_string() { return id; }

	std::string id;
};

class NumberNode : public ASTNode
{
  public:
	NumberNode(int number)
		: number(number) {}
	std::string to_string() { return std::to_string(number); }

	int number;
};

#endif
