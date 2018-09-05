#include "nodes.hpp"
#include <cassert>
#include <unordered_map>

std::unordered_map<std::string, ASTNode *> type_definitions;
std::unordered_map<std::string, std::unordered_map<std::string, ASTNode *>> variables;

std::string ProgramNode::to_string() const
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

std::string TypeDefinitionList::to_string() const
{
	std::string type_definition_list_str = "";
	for (auto type_definition : get_ast_nodes())
	{
		type_definition_list_str += type_definition->to_string();
	}
	return type_definition_list_str;
}

std::string TypeDefinition::to_string() const
{
	std::string type_definition_str = "tipo ";
	type_definition_str += id->to_string();
	type_definition_str += " es ";
	type_definition_str += type->to_string();
	type_definition_str += "\n";
	return type_definition_str;
}

std::string Type::to_string() const
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

std::string VariableSection::to_string() const
{
	std::string variable_section_str = "";
	variable_section_str += type->to_string();
	variable_section_str += " ";
	variable_section_str += ids->to_string();
	variable_section_str += "\n";
	return variable_section_str;
}

std::string VariableSectionList::to_string() const
{
	std::string variable_section_list_str = "";
	for (auto variable_section : get_ast_nodes())
	{
		variable_section_list_str += variable_section->to_string();
	}
	return variable_section_list_str;
}

std::string IdList::to_string() const
{
	std::string id_list_str = "";
	if (get_ast_nodes().size() > 1)
	{
		for (auto id : get_ast_nodes())
		{
			if (id != get_ast_nodes().back())
			{
				id_list_str += id->to_string() + ", ";
			}
			else
			{
				id_list_str += id->to_string();
			}
		}
	}
	else if (!get_ast_nodes().empty())
	{
		id_list_str = get_ast_nodes().front()->to_string();
	}
	return id_list_str;
}

std::string SubprogramDeclList::to_string() const
{
	std::string subprogram_decl_list_str = "";
	for (auto subprogram_decl : get_ast_nodes())
	{
		subprogram_decl_list_str += subprogram_decl->to_string();
	}
	return subprogram_decl_list_str;
}

std::string SubprogramDecl::to_string() const
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

std::string SubprogramDeclHeader::to_string() const
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
	if (arguments != NULL)
	{
		subprogram_decl_header_str += "(";
		subprogram_decl_header_str += arguments->to_string();
		subprogram_decl_header_str += ")";
	}
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

std::string ArgumentDeclList::to_string() const
{
	std::string argument_decl_list_str = "";
	if (get_ast_nodes().size() > 1)
	{
		for (auto argument_decl : get_ast_nodes())
		{
			if (argument_decl != get_ast_nodes().back())
			{
				argument_decl_list_str += argument_decl->to_string() + ", ";
			}
			else
			{
				argument_decl_list_str += argument_decl->to_string();
			}
		}
	}
	else if (!get_ast_nodes().empty())
	{
		argument_decl_list_str = get_ast_nodes().front()->to_string();
	}
	return argument_decl_list_str;
}

std::string ArgumentDecl::to_string() const
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

std::string StatementList::to_string() const
{
	std::string statement_list_str = "";
	for (auto statement : get_ast_nodes())
	{
		statement_list_str += statement->to_string();
	}
	return statement_list_str;
}

std::string AssignStatement::to_string() const
{
	std::string assign_statement_str = "";
	assign_statement_str += lvalue->to_string();
	assign_statement_str += " <- ";
	assign_statement_str += expr->to_string();
	assign_statement_str += "\n";
	return assign_statement_str;
}
std::string LeftValue::to_string() const
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

std::string BinaryExpr::to_string() const
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

	binary_expr_str += string1 + " " + get_oper() + " " + string2;
	return binary_expr_str;
}

std::string NegativeExpr::to_string() const
{
	std::string negative_expr_str = "-";
	negative_expr_str += expr->to_string();
	return negative_expr_str;
}

std::string BooleanNode::to_string() const
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

std::string SubprogramCall::to_string() const
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

std::string ArgumentList::to_string() const
{
	std::string argument_list_str = "";
	if (get_ast_nodes().size() > 1)
	{
		for (auto argument : get_ast_nodes())
		{
			if (argument != get_ast_nodes().back())
			{
				argument_list_str += argument->to_string() + ", ";
			}
			else
			{
				argument_list_str += argument->to_string();
			}
		}
	}
	else if (!get_ast_nodes().empty())
	{
		argument_list_str = get_ast_nodes().front()->to_string();
	}
	return argument_list_str;
}

std::string CallStatement::to_string() const
{
	std::string call_statement_str = "llamar ";
	call_statement_str += call->to_string();
	call_statement_str += "\n";
	return call_statement_str;
}

std::string IfStatement::to_string() const
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

std::string ElseStatement::to_string() const
{
	std::string else_statement_str = "sino\n";
	if (statement_list != NULL)
	{
		else_statement_str += statement_list->to_string();
	}
	return else_statement_str;
}

std::string WhileStatement::to_string() const
{
	std::string while_statement_str = "mientras ";
	while_statement_str += expr->to_string();
	while_statement_str += " haga\n";
	if (statement_list != NULL)
	{
		while_statement_str += statement_list->to_string();
	}
	while_statement_str += "fin mientras\n";
	return while_statement_str;
}

std::string ForStatement::to_string() const
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

std::string NotDoWhileStatement::to_string() const
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

std::string ReturnNode::to_string() const
{
	std::string return_node_str = "retorne ";
	return_node_str += expr->to_string();
	return_node_str += "\n";
	return return_node_str;
}

std::string WriteNode::to_string() const
{
	std::string write_node_str = "escriba ";
	write_node_str += argument_list->to_string();
	write_node_str += "\n";
	return write_node_str;
}

std::string ReadNode::to_string() const
{
	std::string read_node_str = "lea ";
	read_node_str += expr->to_string();
	read_node_str += "\n";
	return read_node_str;
}

std::string NegateExpr::to_string() const
{
	std::string negate_expr_str = "no ";
	negate_expr_str += expr->to_string();
	return negate_expr_str;
}

void ASTNodeList::add_to_list(ASTNode *list, ASTNode *element)
{
	assert(list->get_type() == NodeEnum::List);
	ASTNodeList *ast_node_list = static_cast<ASTNodeList *>(list);
	ast_node_list->get_pointer_to_ast_nodes()->push_back(element);
}

void ProgramNode::pre_syntax_analysis(std::string context)
{
	if (type_definition_section != NULL)
	{
		type_definition_section->pre_syntax_analysis(context);
	}
	if (variable_section != NULL)
	{
		variable_section->pre_syntax_analysis(context);
	}
	if (subprogram_decl != NULL)
	{
		subprogram_decl->pre_syntax_analysis(context);
	}
}

void TypeDefinitionList::pre_syntax_analysis(std::string context)
{
	for (auto type_definition : get_ast_nodes())
	{
		type_definition->pre_syntax_analysis(context);
	}
}

void TypeDefinition::pre_syntax_analysis(std::string context)
{
	assert(id->get_type() == NodeEnum::StringNode);
	StringNode *string_node = static_cast<StringNode *>(id);
	const std::string id_str = string_node->get_id();
	if (type_definitions.find(id_str) != type_definitions.end())
	{
		std::cerr << "[line "
				  << get_line()
				  << "] "
				  << "'"
				  << id_str
				  << "' was already defined"
				  << std::endl;
		exit(1);
	}
	else
	{
		type_definitions[id_str] = type;
	}
}

void VariableSectionList::pre_syntax_analysis(std::string context)
{
	for (auto variable_section : get_ast_nodes())
	{
		variable_section->pre_syntax_analysis(context);
	}
}

void VariableSection::pre_syntax_analysis(std::string context)
{
	assert(ids->get_type() == NodeEnum::List);
	ASTNodeList *ast_node_list = static_cast<ASTNodeList *>(ids);
	auto ids_list = ast_node_list->get_ast_nodes();
	for (auto id : ids_list)
	{
		assert(id->get_type() == NodeEnum::StringNode);
		StringNode *string_node = static_cast<StringNode *>(id);
		const std::string id_str = string_node->get_id();
		if (variables[context].find(id_str) != variables[context].end())
		{
			std::cerr << "[line "
					  << string_node->get_line()
					  << "] "
					  << "'"
					  << id_str
					  << "' was already defined"
					  << std::endl;
			exit(1);
		}
		else
		{
			variables[context][id_str] = type;
		}
	}
}
