#include "nodes.hpp"

std::string ProgramNode::to_string()
{
	std::string program_str = "";
	if (type_definition_section != NULL)
	{
		program_str += type_definition_section->to_string();
	}
	if (variable_section != NULL)
	{
		program_str += variable_section->to_string();
	}
	if (subprogram_decl != NULL)
	{
		program_str += subprogram_decl->to_string();
	}
	program_str += "inicio\n";
	if (statement_list != NULL)
	{
		program_str += statement_list->to_string();
	}
	program_str += "fin";
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
	type_definition_str += "\n";
	return type_definition_str;
}

std::string Type::to_string()
{
	std::string type_str = "";
	switch (type)
	{
	case TypeEnum::Entero:
		type_str += "entero";
		break;
	case TypeEnum::Booleano:
		type_str += "booleano";
		break;
	case TypeEnum::Caracter:
		type_str += "caracter";
		break;
	case TypeEnum::Arreglo:
		type_str += "arreglo [";
		type_str += array_size->to_string();
		type_str += "] de ";
		type_str += array_type->to_string();
		break;
	case TypeEnum::Tipo:
		type_str += user_type->to_string();
		break;
	}
	return type_str;
}

std::string VariableSection::to_string()
{
	std::string variable_section_str = "";
	variable_section_str += type->to_string();
	variable_section_str += " ";
	variable_section_str += ids->to_string();
	variable_section_str += "\n";
	return variable_section_str;
}

std::string VariableSectionList::to_string()
{
	std::string variable_section_list_str = "";
	for (auto variable_section : variable_sections)
	{
		variable_section_list_str += variable_section->to_string();
	}
	return variable_section_list_str;
}

std::string IdList::to_string()
{
	std::string id_list_str = "";
	if (ids.size() > 1)
	{
		for (auto id : ids)
		{
			if (id != ids.back())
			{
				id_list_str = id->to_string() + ",";
			}
			else
			{
				id_list_str = id->to_string();
			}
		}
	}
	else
	{
		id_list_str = ids.front()->to_string();
	}
	return id_list_str;
}
