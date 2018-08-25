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
	std::string to_string() override { return id; }

	std::string id;
};

class NumberNode : public ASTNode
{
  public:
	NumberNode(int number)
		: number(number) {}
	std::string to_string() override { return std::to_string(number); }

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

#endif
