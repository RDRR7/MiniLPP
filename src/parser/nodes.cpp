#include "nodes.hpp"

std::string ProgramNode::to_string()
{
	std::string program_str = "";
	if (type_definition_section != NULL)
	{
		program_str += type_definition_section->to_string();
	}
	return program_str;
}

std::string TypeDefinitionSection::to_string()
{
	std::string type_definition_section_str = "";
	for (auto type_definition : type_definitions)
	{
		type_definition_section_str += type_definition->to_string();
	}
	return type_definition_section_str;
}

std::string TypeDefinition::to_string()
{
	std::string type_definition_str = "tipo ";
	type_definition_str += id->to_string();
	type_definition_str += " es ";
	type_definition_str += type->to_string();
	return type_definition_str;
}

std::string Type::to_string()
{
	std::string type_str = "";
	switch (type)
	{
	case TypeEnum::Entero:
		type_str += "entero\n";
		break;
	case TypeEnum::Booleano:
		type_str += "booleano\n";
		break;
	case TypeEnum::Caracter:
		type_str += "caracter\n";
		break;
	case TypeEnum::Arreglo:
		type_str += "arreglo [";
		type_str += array_size->to_string();
		type_str += "] de ";
		type_str += array_type->to_string();
		break;
	case TypeEnum::Tipo:
		type_str += user_type->to_string();
		type_str += "\n";
		break;
	}
	return type_str;
}
