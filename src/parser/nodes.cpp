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
				id_list_str += id->to_string() + ", ";
			}
			else
			{
				id_list_str += id->to_string();
			}
		}
	}
	else if (!ids.empty())
	{
		id_list_str = ids.front()->to_string();
	}
	return id_list_str;
}

std::string SubprogramDeclList::to_string()
{
	std::string subprogram_decl_list_str = "";
	for (auto subprogram_decl : subprogram_decls)
	{
		subprogram_decl_list_str += subprogram_decl->to_string();
	}
	return subprogram_decl_list_str;
}

std::string SubprogramDecl::to_string()
{
	std::string subprogram_decl_str = "";
	subprogram_decl_str += header->to_string();
	if (variable_section != NULL)
	{
		subprogram_decl_str += variable_section->to_string();
	}
	subprogram_decl_str += "inicio\n";
	if (statements != NULL)
	{
		subprogram_decl_str += statements->to_string();
	}
	subprogram_decl_str += "fin\n";
	return subprogram_decl_str;
}

std::string SubprogramDeclHeader::to_string()
{
	std::string subprogram_decl_header_str = "";
	if (type != NULL)
	{
		subprogram_decl_header_str += "funcion ";
	}
	else
	{
		subprogram_decl_header_str += "procedimiento ";
	}
	subprogram_decl_header_str += id->to_string();
	subprogram_decl_header_str += "(";
	if (arguments != NULL)
	{
		subprogram_decl_header_str += arguments->to_string();
	}
	subprogram_decl_header_str += ")";
	if (type != NULL)
	{
		subprogram_decl_header_str += " : ";
		subprogram_decl_header_str += type->to_string();
		subprogram_decl_header_str += "\n";
	}
	else
	{
		subprogram_decl_header_str += "\n";
	}
	return subprogram_decl_header_str;
}

std::string ArgumentDeclList::to_string()
{
	std::string argument_decl_list_str = "";
	if (arguments_decls.size() > 1)
	{
		for (auto argument_decl : arguments_decls)
		{
			if (argument_decl != arguments_decls.back())
			{
				argument_decl_list_str += argument_decl->to_string() + ", ";
			}
			else
			{
				argument_decl_list_str += argument_decl->to_string();
			}
		}
	}
	else if (!arguments_decls.empty())
	{
		argument_decl_list_str = arguments_decls.front()->to_string();
	}
	return argument_decl_list_str;
}

std::string ArgumentDecl::to_string()
{
	std::string argument_decl_str = "";
	if (var)
	{
		argument_decl_str += "var ";
	}
	argument_decl_str += type->to_string();
	argument_decl_str += " ";
	argument_decl_str += id->to_string();
	return argument_decl_str;
}

std::string StatementList::to_string()
{
	std::string statement_list_str = "";
	for (auto statement : statements)
	{
		if (statement != NULL) // remove
			statement_list_str += statement->to_string();
	}
	return statement_list_str;
}

std::string AssignStatement::to_string()
{
	std::string assign_statement_str = "";
	assign_statement_str += lvalue->to_string();
	assign_statement_str += " <- ";
	assign_statement_str += expr->to_string();
	assign_statement_str += "\n";
	return assign_statement_str;
}
std::string LeftValue::to_string()
{
	std::string left_value_str = "";
	left_value_str += id->to_string();
	if (index != NULL)
	{
		left_value_str += "[";
		left_value_str += index->to_string();
		left_value_str += "]";
	}
	return left_value_str;
}

std::string BinaryExpr::to_string()
{
	std::string binary_expr_str = "";
	Expr *expr_expr1 = static_cast<Expr *>(expr1);
	Expr *expr_expr2 = static_cast<Expr *>(expr2);
	std::string string1 = expr1->to_string();
	std::string string2 = expr2->to_string();

	if (get_precedence() > expr_expr1->get_precedence())
	{
		string1 = "( " + string1 + " )";
	}
	if (get_precedence() > expr_expr2->get_precedence())
	{
		string2 = "( " + string2 + " )";
	}

	binary_expr_str += string1 + " " + this->get_oper() + " " + string2;
	return binary_expr_str;
}

std::string NegativeExpr::to_string()
{
	std::string negative_expr_str = "-";
	negative_expr_str += expr->to_string();
	return negative_expr_str;
}

std::string BooleanNode::to_string()
{
	std::string boolean_node_str = "";
	if (boolean)
	{
		boolean_node_str += "verdadero";
	}
	else
	{
		boolean_node_str += "falso";
	}
	return boolean_node_str;
}

std::string SubprogramCall::to_string()
{
	std::string subprogram_call_str = "";
	subprogram_call_str += id->to_string();
	subprogram_call_str += "(";
	if (argument_list != NULL)
	{
		subprogram_call_str += argument_list->to_string();
	}
	subprogram_call_str += ")";
	return subprogram_call_str;
}

std::string ArgumentList::to_string()
{
	std::string argument_list_str = "";
	if (arguments.size() > 1)
	{
		for (auto argument : arguments)
		{
			if (argument != arguments.back())
			{
				argument_list_str += argument->to_string() + ", ";
			}
			else
			{
				argument_list_str += argument->to_string();
			}
		}
	}
	else if (!arguments.empty())
	{
		argument_list_str = arguments.front()->to_string();
	}
	return argument_list_str;
}

std::string CallStatement::to_string()
{
	std::string call_statement_str = "llamar ";
	call_statement_str += call->to_string();
	call_statement_str += "\n";
	return call_statement_str;
}

std::string IfStatement::to_string()
{
	std::string if_statement_str = "si ";
	if_statement_str += expr->to_string();
	if_statement_str += " entonces\n";
	if (statement_list != NULL)
	{
		if_statement_str += statement_list->to_string();
	}
	if (else_statement != NULL)
	{
		if_statement_str += else_statement->to_string();
	}
	if_statement_str += "fin si\n";
	return if_statement_str;
}

std::string ElseStatement::to_string()
{
	std::string else_statement_str = "sino\n";
	if (statement_list != NULL)
	{
		else_statement_str += statement_list->to_string();
	}
	return else_statement_str;
}

std::string WhileStatement::to_string()
{
	std::string while_statement_str = "mientras ";
	if (no)
	{
		while_statement_str += "no ";
	}
	while_statement_str += expr->to_string();
	while_statement_str += " haga\n";
	if (statement_list != NULL)
	{
		while_statement_str += statement_list->to_string();
	}
	while_statement_str += "fin mientras\n";
	return while_statement_str;
}

std::string ForStatement::to_string()
{
	std::string for_statement_str = "para ";
	for_statement_str += assign->to_string();
	for_statement_str.erase(for_statement_str.end() - 1);
	for_statement_str += " hasta ";
	for_statement_str += expr->to_string();
	for_statement_str += " haga\n";
	if (statement_list != NULL)
	{
		for_statement_str += statement_list->to_string();
	}
	for_statement_str += "fin para\n";
	return for_statement_str;
}

std::string NotDoWhileStatement::to_string()
{
	std::string not_do_while_statement_str = "repita\n";
	if (statement_list != NULL)
	{
		not_do_while_statement_str += statement_list->to_string();
	}
	not_do_while_statement_str += "hasta ";
	not_do_while_statement_str += expr->to_string();
	not_do_while_statement_str += "\n";
	return not_do_while_statement_str;
}

std::string ReturnNode::to_string()
{
	std::string return_node_str = "retorne ";
	return_node_str += expr->to_string();
	return_node_str += "\n";
	return return_node_str;
}

std::string WriteNode::to_string()
{
	std::string write_node_str = "escriba ";
	write_node_str += argument_list->to_string();
	write_node_str += "\n";
	return write_node_str;
}

std::string ReadNode::to_string()
{
	std::string read_node_str = "lea ";
	read_node_str += expr->to_string();
	read_node_str += "\n";
	return read_node_str;
}
