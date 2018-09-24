#include "nodes.hpp"
#include <cassert>
#include <unordered_map>
#include <vector>

std::unordered_map<std::string, ASTNode *> type_definitions;
std::unordered_map<std::string, std::unordered_map<std::string, ASTNode *>> functions_variables;
std::unordered_map<std::string, std::vector<ASTNode *>> functions_parameters;

void ASTNodeList::add_to_list(ASTNode *list, ASTNode *element)
{
	assert(list->get_type() == NodeEnum::ASTNodeList);
	ASTNodeList *ast_node_list = static_cast<ASTNodeList *>(list);
	ast_node_list->get_pointer_to_ast_nodes()->push_back(element);
}

void load_lpp_functions()
{
	functions_variables["limpiar_pantalla"];

	functions_variables["posicionar_cursor"];
	functions_parameters["posicionar_cursor"].push_back(
		new Type(0, TypeEnum::Entero, NULL, NULL, NULL));
	functions_parameters["posicionar_cursor"].push_back(
		new Type(0, TypeEnum::Entero, NULL, NULL, NULL));

	functions_variables["color_fondo"];
	functions_parameters["color_fondo"].push_back(
		new Type(0, TypeEnum::Entero, NULL, NULL, NULL));

	functions_variables["color_texto"];
	functions_parameters["color_texto"].push_back(
		new Type(0, TypeEnum::Entero, NULL, NULL, NULL));

	functions_variables["caracter_ascii"];
	functions_parameters["caracter_ascii"].push_back(
		new Type(0, TypeEnum::Entero, NULL, NULL, NULL));
	functions_variables["caracter_ascii"][RETURN_TYPE] =
		new Type(0, TypeEnum::Caracter, NULL, NULL, NULL);

	functions_variables["valor_ascii"];
	functions_parameters["valor_ascii"].push_back(
		new Type(0, TypeEnum::Caracter, NULL, NULL, NULL));
	functions_variables["valor_ascii"][RETURN_TYPE] =
		new Type(0, TypeEnum::Entero, NULL, NULL, NULL);

	functions_variables["obtener_caracter"];
	functions_variables["obtener_caracter"][RETURN_TYPE] =
		new Type(0, TypeEnum::Caracter, NULL, NULL, NULL);

	functions_variables["obtener_tecla"];
	functions_variables["obtener_tecla"][RETURN_TYPE] =
		new Type(0, TypeEnum::Entero, NULL, NULL, NULL);

	functions_variables["nueva_linea"];

	functions_variables["aleatorio"];
	functions_variables["aleatorio"][RETURN_TYPE] =
		new Type(0, TypeEnum::Entero, NULL, NULL, NULL);

	functions_variables["inicializar_aleatorio"];

	functions_variables["tecla_presionada"];
	functions_variables["tecla_presionada"][RETURN_TYPE] =
		new Type(0, TypeEnum::Booleano, NULL, NULL, NULL);

	functions_variables["pausa"];
	functions_parameters["pausa"].push_back(
		new Type(0, TypeEnum::Entero, NULL, NULL, NULL));
}

void SubprogramCall::check_arguments(std::string context) const
{
	assert(id->get_type() == NodeEnum::StringNode);
	StringNode *string_node = static_cast<StringNode *>(id);
	const std::string id_str = string_node->get_id();

	if (argument_list != NULL)
	{
		assert(argument_list->get_type() == NodeEnum::ASTNodeList);
		ASTNodeList *ast_node_list = static_cast<ASTNodeList *>(argument_list);
		auto argument_list_as_list = ast_node_list->get_ast_nodes();

		int i = 0;
		for (auto argument : argument_list_as_list)
		{
			assert(argument->get_type() == NodeEnum::Expr);
			Expr *argument_as_expr = static_cast<Expr *>(argument);

			if (i >= functions_parameters[id_str].size())
			{
				break;
			}
			assert(functions_parameters[id_str].at(i)->get_type() == NodeEnum::Type);
			Type *parameter_as_type = static_cast<Type *>(functions_parameters[id_str].at(i++));

			if (argument_as_expr->infer_type(context) != parameter_as_type->infer_type(id_str))
			{
				std::cerr << "[line "
						  << string_node->get_line()
						  << "]<"
						  << context
						  << "> '"
						  << id_str
						  << "' argument "
						  << i
						  << ", incompatible type"
						  << std::endl;
				exit(1);
			}
		}

		if (argument_list_as_list.size() != functions_parameters[id_str].size())
		{
			std::cerr << "[line "
					  << string_node->get_line()
					  << "]<"
					  << context
					  << "> '"
					  << id_str
					  << "' expected "
					  << functions_parameters[id_str].size()
					  << " arguments, but got "
					  << argument_list_as_list.size()
					  << std::endl;
			exit(1);
		}
	}
	else
	{
		if (!functions_parameters[id_str].empty())
		{
			std::cerr << "[line "
					  << string_node->get_line()
					  << "]<"
					  << context
					  << "> '"
					  << id_str
					  << "' expected "
					  << functions_parameters[id_str].size()
					  << " arguments, but got 0"
					  << std::endl;
			exit(1);
		}
	}
}

/*--------------- Pre-syntax analysis ---------------*/

void ProgramNode::pre_syntax_analysis(std::string context)
{
	load_lpp_functions();
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
		functions_parameters[context].push_back(type);
	}
}

/*--------------- Infer type ---------------*/

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
		return TypeEnum::Booleano;
	}
	else
	{
		std::cerr << "[line "
				  << get_expr1()->get_line()
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
		return TypeEnum::Booleano;
	}
	else
	{
		std::cerr << "[line "
				  << get_expr1()->get_line()
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
		return TypeEnum::Booleano;
	}
	else
	{
		std::cerr << "[line "
				  << get_expr1()->get_line()
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
		return TypeEnum::Booleano;
	}
	else
	{
		std::cerr << "[line "
				  << get_expr1()->get_line()
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
		return TypeEnum::Booleano;
	}
	else
	{
		std::cerr << "[line "
				  << get_expr1()->get_line()
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
		return TypeEnum::Booleano;
	}
	else
	{
		std::cerr << "[line "
				  << get_expr1()->get_line()
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
		return TypeEnum::Entero;
	}
	else
	{
		std::cerr << "[line "
				  << get_expr1()->get_line()
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
		return TypeEnum::Entero;
	}
	else
	{
		std::cerr << "[line "
				  << get_expr1()->get_line()
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
				  << get_expr1()->get_line()
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
		return TypeEnum::Entero;
	}
	else
	{
		std::cerr << "[line "
				  << get_expr1()->get_line()
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
		return TypeEnum::Entero;
	}
	else
	{
		std::cerr << "[line "
				  << get_expr1()->get_line()
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
		return TypeEnum::Entero;
	}
	else
	{
		std::cerr << "[line "
				  << get_expr1()->get_line()
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
				  << get_expr1()->get_line()
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
		return TypeEnum::Entero;
	}
	else
	{
		std::cerr << "[line "
				  << get_expr1()->get_line()
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
		return TypeEnum::Entero;
	}
	else
	{
		std::cerr << "[line "
				  << expr->get_line()
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
			check_arguments(context);

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
	TypeEnum expr_type = expr_as_expr->infer_type(context);

	if (expr_type == TypeEnum::Booleano)
	{
		return TypeEnum::Booleano;
	}
	else
	{
		std::cerr << "[line "
				  << expr->get_line()
				  << "]<"
				  << context
				  << "> expression with incompatible type"
				  << std::endl;
		exit(1);
	}
}

TypeEnum Type::infer_type(std::string context) const
{
	if (type == TypeEnum::Entero)
	{
		return TypeEnum::Entero;
	}
	if (type == TypeEnum::Caracter)
	{
		return TypeEnum::Caracter;
	}
	if (type == TypeEnum::Booleano)
	{
		return TypeEnum::Booleano;
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

TypeEnum AssignStatement::infer_type(std::string context) const
{
	assert(lvalue->get_type() == NodeEnum::Expr);
	Expr *lvalue_as_expr = static_cast<Expr *>(lvalue);
	return lvalue_as_expr->infer_type(context);
}

/*--------------- Syntax analysis ---------------*/

void ProgramNode::syntax_analysis(std::string context)
{
	if (subprogram_decl != NULL)
	{
		subprogram_decl->syntax_analysis(context);
	}
	if (statement_list != NULL)
	{
		statement_list->syntax_analysis(context);
	}
}

void SubprogramDeclList::syntax_analysis(std::string context)
{
	for (auto subprogram_decl : get_ast_nodes())
	{
		subprogram_decl->syntax_analysis(context);
	}
}

void SubprogramDecl::syntax_analysis(std::string context)
{
	assert(header->get_type() == NodeEnum::SubprogramDeclHeader);
	SubprogramDeclHeader *subprogram_decl_header = static_cast<SubprogramDeclHeader *>(header);
	ASTNode *id = subprogram_decl_header->get_id();
	assert(id->get_type() == NodeEnum::StringNode);
	StringNode *string_node = static_cast<StringNode *>(id);
	const std::string id_str = string_node->get_id();
	if (statements != NULL)
	{
		statements->syntax_analysis(id_str);
	}
}

void StatementList::syntax_analysis(std::string context)
{
	for (auto statement : get_ast_nodes())
	{
		statement->syntax_analysis(context);
	}
}

void AssignStatement::syntax_analysis(std::string context)
{
	assert(lvalue->get_type() == NodeEnum::Expr);
	Expr *lvalue_as_expr = static_cast<Expr *>(lvalue);
	TypeEnum lvalue_type = lvalue_as_expr->infer_type(context);
	assert(expr->get_type() == NodeEnum::Expr);
	Expr *expr_as_expr = static_cast<Expr *>(expr);
	TypeEnum expr_type = expr_as_expr->infer_type(context);
	if (lvalue_type != expr_type)
	{
		std::cerr << "[line "
				  << lvalue->get_line()
				  << "]<"
				  << context
				  << "> incompatible types in assignment"
				  << std::endl;
		exit(1);
	}
}

void CallStatement::syntax_analysis(std::string context)
{
	assert(call->get_type() == NodeEnum::Expr || call->get_type() == NodeEnum::StringNode);
	if (call->get_type() == NodeEnum::Expr)
	{
		SubprogramCall *call_as_subprogram_call = static_cast<SubprogramCall *>(call);
		assert(call_as_subprogram_call->get_id()->get_type() == NodeEnum::StringNode);
		StringNode *string_node = static_cast<StringNode *>(call_as_subprogram_call->get_id());
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

		call_as_subprogram_call->check_arguments(context);
	}
	if (call->get_type() == NodeEnum::StringNode)
	{
		assert(call->get_type() == NodeEnum::StringNode);
		StringNode *string_node = static_cast<StringNode *>(call);
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

		if (!functions_parameters[id_str].empty())
		{
			std::cerr << "[line "
					  << string_node->get_line()
					  << "]<"
					  << context
					  << "> '"
					  << id_str
					  << "' expected "
					  << functions_parameters[id_str].size()
					  << " arguments, but got 0"
					  << std::endl;
			exit(1);
		}
	}
}

void IfStatement::syntax_analysis(std::string context)
{
	assert(expr->get_type() == NodeEnum::Expr);
	Expr *expr_as_expr = static_cast<Expr *>(expr);
	if (expr_as_expr->infer_type(context) != TypeEnum::Booleano)
	{
		std::cerr << "[line "
				  << expr->get_line()
				  << "]<"
				  << context
				  << "><si> expression must be 'booleano'"
				  << std::endl;
		exit(1);
	}
	if (statement_list != NULL)
	{
		statement_list->syntax_analysis(context);
	}
	if (else_statement != NULL)
	{
		else_statement->syntax_analysis(context);
	}
}

void ElseStatement::syntax_analysis(std::string context)
{
	if (statement_list != NULL)
	{
		statement_list->syntax_analysis(context);
	}
}

void WhileStatement::syntax_analysis(std::string context)
{
	assert(expr->get_type() == NodeEnum::Expr);
	Expr *expr_as_expr = static_cast<Expr *>(expr);
	if (expr_as_expr->infer_type(context) != TypeEnum::Booleano)
	{
		std::cerr << "[line "
				  << expr->get_line()
				  << "]<"
				  << context
				  << "><mientras> expression must be 'booleano'"
				  << std::endl;
		exit(1);
	}
	if (statement_list != NULL)
	{
		statement_list->syntax_analysis(context);
	}
}

void ForStatement::syntax_analysis(std::string context)
{
	assign->syntax_analysis(context);
	assert(assign->get_type() == NodeEnum::AssignStatement);
	AssignStatement *assign_as_assign_statement = static_cast<AssignStatement *>(assign);
	if (assign_as_assign_statement->infer_type(context) != TypeEnum::Entero)
	{
		std::cerr << "[line "
				  << assign->get_line()
				  << "]<"
				  << context
				  << "><para> variable must be 'entero'"
				  << std::endl;
		exit(1);
	}
	assert(expr->get_type() == NodeEnum::Expr);
	Expr *expr_as_expr = static_cast<Expr *>(expr);
	if (expr_as_expr->infer_type(context) != TypeEnum::Entero)
	{
		std::cerr << "[line "
				  << expr->get_line()
				  << "]<"
				  << context
				  << "><para> expr must be 'entero'"
				  << std::endl;
		exit(1);
	}
	if (statement_list != NULL)
	{
		statement_list->syntax_analysis(context);
	}
}

void NotDoWhileStatement::syntax_analysis(std::string context)
{
	assert(expr->get_type() == NodeEnum::Expr);
	Expr *expr_as_expr = static_cast<Expr *>(expr);
	if (expr_as_expr->infer_type(context) != TypeEnum::Booleano)
	{
		std::cerr << "[line "
				  << expr->get_line()
				  << "]<"
				  << context
				  << "><repita> expression must be 'booleano'"
				  << std::endl;
		exit(1);
	}
	if (statement_list != NULL)
	{
		statement_list->syntax_analysis(context);
	}
}

void ReturnNode::syntax_analysis(std::string context)
{
	if (functions_variables.find(context) == functions_variables.end())
	{
		std::cerr << "[line "
				  << expr->get_line()
				  << "]<"
				  << context
				  << "> '"
				  << context
				  << "' is not defined"
				  << std::endl;
		exit(1);
	}
	if (functions_variables[context].find(RETURN_TYPE) == functions_variables[context].end())
	{
		std::cerr << "[line "
				  << expr->get_line()
				  << "]<"
				  << context
				  << "><retorne> '"
				  << context
				  << "' is not a function"
				  << std::endl;
		exit(1);
	}
	assert(functions_variables[context][RETURN_TYPE]->get_type() == NodeEnum::Type);
	Type *type = static_cast<Type *>(functions_variables[context][RETURN_TYPE]);
	assert(expr->get_type() == NodeEnum::Expr);
	Expr *expr_as_expr = static_cast<Expr *>(expr);
	if (type->infer_type(context) != expr_as_expr->infer_type(context))
	{
		std::cerr << "[line "
				  << expr->get_line()
				  << "]<"
				  << context
				  << "> '"
				  << context
				  << "' return value does not match the function type"
				  << std::endl;
		exit(1);
	}
}

void WriteNode::syntax_analysis(std::string context)
{
	assert(argument_list->get_type() == NodeEnum::ASTNodeList);
	ASTNodeList *ast_node_list = static_cast<ASTNodeList *>(argument_list);
	auto argument_list_as_list = ast_node_list->get_ast_nodes();

	for (auto argument : argument_list_as_list)
	{
		if (argument->get_type() == NodeEnum::Expr)
		{
			Expr *argument_as_expr = static_cast<Expr *>(argument);
			argument_as_expr->infer_type(context);
		}
	}
}

void ReadNode::syntax_analysis(std::string context)
{
	assert(expr->get_type() == NodeEnum::Expr);
	Expr *expr_as_expr = static_cast<Expr *>(expr);
	expr_as_expr->infer_type(context);
}
