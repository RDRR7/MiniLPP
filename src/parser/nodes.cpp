#include "nodes.hpp"
#include <cassert>
#include <unordered_map>

std::unordered_map<std::string, ASTNode *> type_definitions;
std::unordered_map<std::string, std::unordered_map<std::string, ASTNode *>> functions_variables;

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
	assert(list->get_type() == NodeEnum::ASTNodeList);
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
				  << "] '"
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
	assert(ids->get_type() == NodeEnum::ASTNodeList);
	ASTNodeList *ast_node_list = static_cast<ASTNodeList *>(ids);
	auto ids_list = ast_node_list->get_ast_nodes();
	for (auto id : ids_list)
	{
		assert(id->get_type() == NodeEnum::StringNode);
		StringNode *string_node = static_cast<StringNode *>(id);
		const std::string id_str = string_node->get_id();
		if (functions_variables[context].find(id_str) != functions_variables[context].end())
		{
			std::cerr << "[line "
					  << string_node->get_line()
					  << "]<"
					  << context
					  << "> '"
					  << id_str
					  << "' was already defined"
					  << std::endl;
			exit(1);
		}
		else
		{
			functions_variables[context][id_str] = type;
		}
	}
}

void SubprogramDeclList::pre_syntax_analysis(std::string context)
{
	for (auto subprogram_decl : get_ast_nodes())
	{
		subprogram_decl->pre_syntax_analysis(context);
	}
}

void SubprogramDecl::pre_syntax_analysis(std::string context)
{
	assert(header->get_type() == NodeEnum::SubprogramDeclHeader);
	SubprogramDeclHeader *subprogram_decl_header = static_cast<SubprogramDeclHeader *>(header);
	ASTNode *id = subprogram_decl_header->get_id();

	assert(id->get_type() == NodeEnum::StringNode);
	StringNode *string_node = static_cast<StringNode *>(id);
	const std::string id_str = string_node->get_id();
	if (functions_variables.find(id_str) != functions_variables.end())
	{
		std::cerr << "[line "
				  << string_node->get_line()
				  << "] '"
				  << id_str
				  << "' was already defined"
				  << std::endl;
		exit(1);
	}
	else
	{
		functions_variables[id_str];
		subprogram_decl_header->pre_syntax_analysis(id_str);
		if (variable_section != NULL)
		{
			variable_section->pre_syntax_analysis(id_str);
		}
	}
}

void SubprogramDeclHeader::pre_syntax_analysis(std::string context)
{
	if (type != NULL)
	{
		functions_variables[context][RETURN_TYPE] = type;
	}
	if (arguments != NULL)
	{
		arguments->pre_syntax_analysis(context);
	}
}

void ArgumentDeclList::pre_syntax_analysis(std::string context)
{
	for (auto argument_decl : get_ast_nodes())
	{
		argument_decl->pre_syntax_analysis(context);
	}
}

void ArgumentDecl::pre_syntax_analysis(std::string context)
{
	assert(id->get_type() == NodeEnum::StringNode);
	StringNode *string_node = static_cast<StringNode *>(id);
	const std::string id_str = string_node->get_id();
	if (functions_variables[context].find(id_str) != functions_variables[context].end())
	{
		std::cerr << "[line "
				  << string_node->get_line()
				  << "]<"
				  << context
				  << "> '"
				  << id_str
				  << "' was already defined"
				  << std::endl;
		exit(1);
	}
	else
	{
		functions_variables[context][id_str] = type;
	}
}

TypeEnum LeftValue::infer_type(std::string context) const
{
	assert(id->get_type() == NodeEnum::StringNode);
	StringNode *string_node = static_cast<StringNode *>(id);
	const std::string id_str = string_node->get_id();
	bool in_global_context = functions_variables[GLOBAL_CONTEXT].find(id_str) !=
							 functions_variables[GLOBAL_CONTEXT].end();
	bool in_context_context = functions_variables[context].find(id_str) !=
							  functions_variables[context].end();
	if (!in_global_context && !in_context_context)
	{
		std::cerr << "[line "
				  << string_node->get_line()
				  << "]<"
				  << context
				  << "> '"
				  << id_str
				  << "' is undefined"
				  << std::endl;
		exit(1);
	}
	std::string lookup_context = GLOBAL_CONTEXT;
	if (in_context_context)
	{
		lookup_context = context;
	}
	assert(functions_variables[lookup_context][id_str]->get_type() == NodeEnum::Type);
	Type *type = static_cast<Type *>(functions_variables[lookup_context][id_str]);
	TypeEnum possible_type = type->infer_type(context);
	if (index != NULL)
	{
		assert(index->get_type() == NodeEnum::Expr);
		Expr *expr = static_cast<Expr *>(index);
		if (expr->infer_type(context) != TypeEnum::Entero)
		{
			std::cerr << "[line "
					  << string_node->get_line()
					  << "]<"
					  << context
					  << "> '"
					  << id_str
					  << "' expression must be 'entero'"
					  << std::endl;
			exit(1);
		}
		switch (possible_type)
		{
		case TypeEnum::ArregloDeEntero:
			return TypeEnum::Entero;
		case TypeEnum::ArregloDeCaracter:
			return TypeEnum::Caracter;
		case TypeEnum::ArregloDeBooleano:
			return TypeEnum::Booleano;
		default:
			std::cerr << "[line "
					  << string_node->get_line()
					  << "]<"
					  << context
					  << "> '"
					  << id_str
					  << "' is not an array"
					  << std::endl;
			exit(1);
		}
	}
	return possible_type;
}

TypeEnum EqualExpr::infer_type(std::string context) const
{
	assert(get_expr1()->get_type() == NodeEnum::Expr);
	Expr *expr1_as_expr = static_cast<Expr *>(get_expr1());
	assert(get_expr2()->get_type() == NodeEnum::Expr);
	Expr *expr2_as_expr = static_cast<Expr *>(get_expr2());
	TypeEnum expr1_type = expr1_as_expr->infer_type(context);
	TypeEnum expr2_type = expr2_as_expr->infer_type(context);

	if (expr1_type == expr2_type &&
		expr1_type != TypeEnum::ArregloDeEntero &&
		expr1_type != TypeEnum::ArregloDeCaracter &&
		expr1_type != TypeEnum::ArregloDeBooleano)
	{
		return expr1_type;
	}
	else
	{
		std::cerr << "[line "
				  << get_line()
				  << "]<"
				  << context
				  << "> expression with incompatible types"
				  << std::endl;
		exit(1);
	}
}

TypeEnum NotEqualExpr::infer_type(std::string context) const
{
	assert(get_expr1()->get_type() == NodeEnum::Expr);
	Expr *expr1_as_expr = static_cast<Expr *>(get_expr1());
	assert(get_expr2()->get_type() == NodeEnum::Expr);
	Expr *expr2_as_expr = static_cast<Expr *>(get_expr2());
	TypeEnum expr1_type = expr1_as_expr->infer_type(context);
	TypeEnum expr2_type = expr2_as_expr->infer_type(context);

	if (expr1_type == expr2_type &&
		expr1_type != TypeEnum::ArregloDeEntero &&
		expr1_type != TypeEnum::ArregloDeCaracter &&
		expr1_type != TypeEnum::ArregloDeBooleano)
	{
		return expr1_type;
	}
	else
	{
		std::cerr << "[line "
				  << get_line()
				  << "]<"
				  << context
				  << "> expression with incompatible types"
				  << std::endl;
		exit(1);
	}
}

TypeEnum LessThanExpr::infer_type(std::string context) const
{
	assert(get_expr1()->get_type() == NodeEnum::Expr);
	Expr *expr1_as_expr = static_cast<Expr *>(get_expr1());
	assert(get_expr2()->get_type() == NodeEnum::Expr);
	Expr *expr2_as_expr = static_cast<Expr *>(get_expr2());
	TypeEnum expr1_type = expr1_as_expr->infer_type(context);
	TypeEnum expr2_type = expr2_as_expr->infer_type(context);

	if (expr1_type == expr2_type &&
		expr1_type != TypeEnum::ArregloDeEntero &&
		expr1_type != TypeEnum::ArregloDeCaracter &&
		expr1_type != TypeEnum::ArregloDeBooleano)
	{
		return expr1_type;
	}
	else
	{
		std::cerr << "[line "
				  << get_line()
				  << "]<"
				  << context
				  << "> expression with incompatible types"
				  << std::endl;
		exit(1);
	}
}

TypeEnum GreaterThanExpr::infer_type(std::string context) const
{
	assert(get_expr1()->get_type() == NodeEnum::Expr);
	Expr *expr1_as_expr = static_cast<Expr *>(get_expr1());
	assert(get_expr2()->get_type() == NodeEnum::Expr);
	Expr *expr2_as_expr = static_cast<Expr *>(get_expr2());
	TypeEnum expr1_type = expr1_as_expr->infer_type(context);
	TypeEnum expr2_type = expr2_as_expr->infer_type(context);

	if (expr1_type == expr2_type &&
		expr1_type != TypeEnum::ArregloDeEntero &&
		expr1_type != TypeEnum::ArregloDeCaracter &&
		expr1_type != TypeEnum::ArregloDeBooleano)
	{
		return expr1_type;
	}
	else
	{
		std::cerr << "[line "
				  << get_line()
				  << "]<"
				  << context
				  << "> expression with incompatible types"
				  << std::endl;
		exit(1);
	}
}

TypeEnum LessThanOrEqualExpr::infer_type(std::string context) const
{
	assert(get_expr1()->get_type() == NodeEnum::Expr);
	Expr *expr1_as_expr = static_cast<Expr *>(get_expr1());
	assert(get_expr2()->get_type() == NodeEnum::Expr);
	Expr *expr2_as_expr = static_cast<Expr *>(get_expr2());
	TypeEnum expr1_type = expr1_as_expr->infer_type(context);
	TypeEnum expr2_type = expr2_as_expr->infer_type(context);

	if (expr1_type == expr2_type &&
		expr1_type != TypeEnum::ArregloDeEntero &&
		expr1_type != TypeEnum::ArregloDeCaracter &&
		expr1_type != TypeEnum::ArregloDeBooleano)
	{
		return expr1_type;
	}
	else
	{
		std::cerr << "[line "
				  << get_line()
				  << "]<"
				  << context
				  << "> expression with incompatible types"
				  << std::endl;
		exit(1);
	}
}

TypeEnum GreaterThanOrEqualExpr::infer_type(std::string context) const
{
	assert(get_expr1()->get_type() == NodeEnum::Expr);
	Expr *expr1_as_expr = static_cast<Expr *>(get_expr1());
	assert(get_expr2()->get_type() == NodeEnum::Expr);
	Expr *expr2_as_expr = static_cast<Expr *>(get_expr2());
	TypeEnum expr1_type = expr1_as_expr->infer_type(context);
	TypeEnum expr2_type = expr2_as_expr->infer_type(context);

	if (expr1_type == expr2_type &&
		expr1_type != TypeEnum::ArregloDeEntero &&
		expr1_type != TypeEnum::ArregloDeCaracter &&
		expr1_type != TypeEnum::ArregloDeBooleano)
	{
		return expr1_type;
	}
	else
	{
		std::cerr << "[line "
				  << get_line()
				  << "]<"
				  << context
				  << "> expression with incompatible types"
				  << std::endl;
		exit(1);
	}
}

TypeEnum AdditionExpr::infer_type(std::string context) const
{
	assert(get_expr1()->get_type() == NodeEnum::Expr);
	Expr *expr1_as_expr = static_cast<Expr *>(get_expr1());
	assert(get_expr2()->get_type() == NodeEnum::Expr);
	Expr *expr2_as_expr = static_cast<Expr *>(get_expr2());
	TypeEnum expr1_type = expr1_as_expr->infer_type(context);
	TypeEnum expr2_type = expr2_as_expr->infer_type(context);

	if (expr1_type == expr2_type &&
		expr1_type != TypeEnum::Caracter &&
		expr1_type != TypeEnum::Booleano &&
		expr1_type != TypeEnum::ArregloDeEntero &&
		expr1_type != TypeEnum::ArregloDeCaracter &&
		expr1_type != TypeEnum::ArregloDeBooleano)
	{
		return expr1_type;
	}
	else
	{
		std::cerr << "[line "
				  << get_line()
				  << "]<"
				  << context
				  << "> expression with incompatible types"
				  << std::endl;
		exit(1);
	}
}

TypeEnum SubtractionExpr::infer_type(std::string context) const
{
	assert(get_expr1()->get_type() == NodeEnum::Expr);
	Expr *expr1_as_expr = static_cast<Expr *>(get_expr1());
	assert(get_expr2()->get_type() == NodeEnum::Expr);
	Expr *expr2_as_expr = static_cast<Expr *>(get_expr2());
	TypeEnum expr1_type = expr1_as_expr->infer_type(context);
	TypeEnum expr2_type = expr2_as_expr->infer_type(context);

	if (expr1_type == expr2_type &&
		expr1_type != TypeEnum::Caracter &&
		expr1_type != TypeEnum::Booleano &&
		expr1_type != TypeEnum::ArregloDeEntero &&
		expr1_type != TypeEnum::ArregloDeCaracter &&
		expr1_type != TypeEnum::ArregloDeBooleano)
	{
		return expr1_type;
	}
	else
	{
		std::cerr << "[line "
				  << get_line()
				  << "]<"
				  << context
				  << "> expression with incompatible types"
				  << std::endl;
		exit(1);
	}
}

TypeEnum OrExpr::infer_type(std::string context) const
{
	assert(get_expr1()->get_type() == NodeEnum::Expr);
	Expr *expr1_as_expr = static_cast<Expr *>(get_expr1());
	assert(get_expr2()->get_type() == NodeEnum::Expr);
	Expr *expr2_as_expr = static_cast<Expr *>(get_expr2());
	TypeEnum expr1_type = expr1_as_expr->infer_type(context);
	TypeEnum expr2_type = expr2_as_expr->infer_type(context);

	if (expr1_type == expr2_type &&
		expr1_type != TypeEnum::Caracter &&
		expr1_type != TypeEnum::ArregloDeEntero &&
		expr1_type != TypeEnum::ArregloDeCaracter &&
		expr1_type != TypeEnum::ArregloDeBooleano)
	{
		return expr1_type;
	}
	else
	{
		std::cerr << "[line "
				  << get_line()
				  << "]<"
				  << context
				  << "> expression with incompatible types"
				  << std::endl;
		exit(1);
	}
}

TypeEnum MultiplicationExpr::infer_type(std::string context) const
{
	assert(get_expr1()->get_type() == NodeEnum::Expr);
	Expr *expr1_as_expr = static_cast<Expr *>(get_expr1());
	assert(get_expr2()->get_type() == NodeEnum::Expr);
	Expr *expr2_as_expr = static_cast<Expr *>(get_expr2());
	TypeEnum expr1_type = expr1_as_expr->infer_type(context);
	TypeEnum expr2_type = expr2_as_expr->infer_type(context);

	if (expr1_type == expr2_type &&
		expr1_type != TypeEnum::Caracter &&
		expr1_type != TypeEnum::Booleano &&
		expr1_type != TypeEnum::ArregloDeEntero &&
		expr1_type != TypeEnum::ArregloDeCaracter &&
		expr1_type != TypeEnum::ArregloDeBooleano)
	{
		return expr1_type;
	}
	else
	{
		std::cerr << "[line "
				  << get_line()
				  << "]<"
				  << context
				  << "> expression with incompatible types"
				  << std::endl;
		exit(1);
	}
}

TypeEnum DivisionExpr::infer_type(std::string context) const
{
	assert(get_expr1()->get_type() == NodeEnum::Expr);
	Expr *expr1_as_expr = static_cast<Expr *>(get_expr1());
	assert(get_expr2()->get_type() == NodeEnum::Expr);
	Expr *expr2_as_expr = static_cast<Expr *>(get_expr2());
	TypeEnum expr1_type = expr1_as_expr->infer_type(context);
	TypeEnum expr2_type = expr2_as_expr->infer_type(context);

	if (expr1_type == expr2_type &&
		expr1_type != TypeEnum::Caracter &&
		expr1_type != TypeEnum::ArregloDeEntero &&
		expr1_type != TypeEnum::ArregloDeCaracter &&
		expr1_type != TypeEnum::ArregloDeBooleano)
	{
		return expr1_type;
	}
	else
	{
		std::cerr << "[line "
				  << get_line()
				  << "]<"
				  << context
				  << "> expression with incompatible types"
				  << std::endl;
		exit(1);
	}
}

TypeEnum ModExpr::infer_type(std::string context) const
{
	assert(get_expr1()->get_type() == NodeEnum::Expr);
	Expr *expr1_as_expr = static_cast<Expr *>(get_expr1());
	assert(get_expr2()->get_type() == NodeEnum::Expr);
	Expr *expr2_as_expr = static_cast<Expr *>(get_expr2());
	TypeEnum expr1_type = expr1_as_expr->infer_type(context);
	TypeEnum expr2_type = expr2_as_expr->infer_type(context);

	if (expr1_type == expr2_type &&
		expr1_type != TypeEnum::Caracter &&
		expr1_type != TypeEnum::ArregloDeEntero &&
		expr1_type != TypeEnum::ArregloDeCaracter &&
		expr1_type != TypeEnum::ArregloDeBooleano)
	{
		return expr1_type;
	}
	else
	{
		std::cerr << "[line "
				  << get_line()
				  << "]<"
				  << context
				  << "> expression with incompatible types"
				  << std::endl;
		exit(1);
	}
}

TypeEnum AndExpr::infer_type(std::string context) const
{
	assert(get_expr1()->get_type() == NodeEnum::Expr);
	Expr *expr1_as_expr = static_cast<Expr *>(get_expr1());
	assert(get_expr2()->get_type() == NodeEnum::Expr);
	Expr *expr2_as_expr = static_cast<Expr *>(get_expr2());
	TypeEnum expr1_type = expr1_as_expr->infer_type(context);
	TypeEnum expr2_type = expr2_as_expr->infer_type(context);

	if (expr1_type == expr2_type &&
		expr1_type != TypeEnum::Caracter &&
		expr1_type != TypeEnum::ArregloDeEntero &&
		expr1_type != TypeEnum::ArregloDeCaracter &&
		expr1_type != TypeEnum::ArregloDeBooleano)
	{
		return expr1_type;
	}
	else
	{
		std::cerr << "[line "
				  << get_line()
				  << "]<"
				  << context
				  << "> expression with incompatible types"
				  << std::endl;
		exit(1);
	}
}

TypeEnum PowerExpr::infer_type(std::string context) const
{
	assert(get_expr1()->get_type() == NodeEnum::Expr);
	Expr *expr1_as_expr = static_cast<Expr *>(get_expr1());
	assert(get_expr2()->get_type() == NodeEnum::Expr);
	Expr *expr2_as_expr = static_cast<Expr *>(get_expr2());
	TypeEnum expr1_type = expr1_as_expr->infer_type(context);
	TypeEnum expr2_type = expr2_as_expr->infer_type(context);

	if (expr1_type == expr2_type &&
		expr1_type != TypeEnum::Caracter &&
		expr1_type != TypeEnum::Booleano &&
		expr1_type != TypeEnum::ArregloDeEntero &&
		expr1_type != TypeEnum::ArregloDeCaracter &&
		expr1_type != TypeEnum::ArregloDeBooleano)
	{
		return expr1_type;
	}
	else
	{
		std::cerr << "[line "
				  << get_line()
				  << "]<"
				  << context
				  << "> expression with incompatible types"
				  << std::endl;
		exit(1);
	}
}

/*---- Change != to == ⬆ ----*/

TypeEnum NegativeExpr::infer_type(std::string context) const
{
	assert(expr->get_type() == NodeEnum::Expr);
	Expr *expr_as_expr = static_cast<Expr *>(expr);
	TypeEnum expr_type = expr_as_expr->infer_type(context);

	if (expr_type == TypeEnum::Entero)
	{
		return expr_type;
	}
	else
	{
		std::cerr << "[line "
				  << get_line()
				  << "]<"
				  << context
				  << "> expression with incompatible type"
				  << std::endl;
		exit(1);
	}
}

TypeEnum SubprogramCall::infer_type(std::string context) const
{
	assert(id->get_type() == NodeEnum::StringNode);
	StringNode *string_node = static_cast<StringNode *>(id);
	const std::string id_str = string_node->get_id();
	if (functions_variables.find(id_str) == functions_variables.end())
	{
		std::cerr << "[line "
				  << string_node->get_line()
				  << "]<"
				  << context
				  << "> '"
				  << id_str
				  << "' is not defined"
				  << std::endl;
		exit(1);
	}
	else
	{
		if (functions_variables[id_str].find(RETURN_TYPE) == functions_variables[id_str].end())
		{
			std::cerr << "[line "
					  << string_node->get_line()
					  << "]<"
					  << context
					  << "> '"
					  << id_str
					  << "' is a procedure"
					  << std::endl;
			exit(1);
		}
		else
		{
			assert(functions_variables[id_str][RETURN_TYPE]->get_type() == NodeEnum::Type);
			Type *type = static_cast<Type *>(functions_variables[id_str][RETURN_TYPE]);
			return type->infer_type(context);
		}
	}
}

TypeEnum NegateExpr::infer_type(std::string context) const
{
	assert(expr->get_type() == NodeEnum::Expr);
	Expr *expr_as_expr = static_cast<Expr *>(expr);
	return expr_as_expr->infer_type(context);
}

TypeEnum Type::infer_type(std::string context) const
{
	if (type == TypeEnum::Entero)
	{
		return type;
	}
	if (type == TypeEnum::Caracter)
	{
		return type;
	}
	if (type == TypeEnum::Booleano)
	{
		return type;
	}
	if (type == TypeEnum::Tipo)
	{
		assert(user_type->get_type() == NodeEnum::StringNode);
		StringNode *string_node = static_cast<StringNode *>(user_type);
		const std::string id_str = string_node->get_id();
		if (type_definitions.find(id_str) == type_definitions.end())
		{
			std::cerr << "[line "
					  << string_node->get_line()
					  << "]<"
					  << context
					  << "> '"
					  << id_str
					  << "' is undefined"
					  << std::endl;
			exit(1);
		}
		else
		{
			assert(type_definitions[id_str]->get_type() == NodeEnum::Type);
			Type *type = static_cast<Type *>(type_definitions[id_str]);
			return type->infer_type(context);
		}
	}
	if (type == TypeEnum::Arreglo)
	{
		assert(array_type->get_type() == NodeEnum::Type);
		Type *type = static_cast<Type *>(array_type);
		switch (type->infer_type(context))
		{
		case TypeEnum::Entero:
			return TypeEnum::ArregloDeEntero;
		case TypeEnum::Caracter:
			return TypeEnum::ArregloDeCaracter;
		case TypeEnum::Booleano:
			return TypeEnum::ArregloDeBooleano;
		case TypeEnum::ArregloDeEntero:
		case TypeEnum::ArregloDeCaracter:
		case TypeEnum::ArregloDeBooleano:
			std::cerr << "[line "
					  << get_line()
					  << "]<"
					  << context
					  << "> array of array is not supported"
					  << std::endl;
			exit(1);
		case TypeEnum::Tipo:
			assert(user_type->get_type() == NodeEnum::StringNode);
			StringNode *string_node = static_cast<StringNode *>(user_type);
			const std::string id_str = string_node->get_id();
			if (type_definitions.find(id_str) == type_definitions.end())
			{
				std::cerr << "[line "
						  << string_node->get_line()
						  << "]<"
						  << context
						  << "> '"
						  << id_str
						  << "' is undefined"
						  << std::endl;
				exit(1);
			}
			else
			{
				assert(type_definitions[id_str]->get_type() == NodeEnum::Type);
				Type *type = static_cast<Type *>(type_definitions[id_str]);
				return type->infer_type(context);
			}
		}
	}
}
