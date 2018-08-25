#include "nodes.hpp"

std::string ProgramNode::to_string()
{
	std::string program = "";
	if (type_definition_section != NULL)
	{
		program += type_definition_section->to_string();
	}
	return program;
}

std::string TypeDefinitionSection::to_string()
{
	return "typedefinitionsection";
}

std::string TypeDefinition::to_string()
{
	return "typedefinition";
}

std::string Type::to_string()
{
	return "type";
}
