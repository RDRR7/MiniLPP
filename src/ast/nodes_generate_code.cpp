#include "nodes.hpp"
#include <cassert>
#include <sstream>

/*--------------- Load functions ---------------*/

void ProgramNode::load_functions(CodeHandler &code_handler)
{
	code_handler.change_context(GLOBAL_CONTEXT);
	if (variable_section != NULL)
	{
		variable_section->load_functions(code_handler);
	}
	if (subprogram_decl != NULL)
	{
		subprogram_decl->load_functions(code_handler);
	}
	code_handler.change_context(GLOBAL_CONTEXT);
}

void VariableSectionList::load_functions(CodeHandler &code_handler)
{
	auto variables = get_ast_nodes();
	for (auto variable : variables)
	{
		variable->load_functions(code_handler);
	}
}

void VariableSection::load_functions(CodeHandler &code_handler)
{
	if (ids != NULL)
	{
		ids->load_functions(code_handler);
	}
}

void IdList::load_functions(CodeHandler &code_handler)
{
	auto ids = get_ast_nodes();
	for (auto id : ids)
	{
		assert(id->get_type() == NodeEnum::StringNode);
		StringNode *string_node = static_cast<StringNode *>(id);
		const std::string id_str = string_node->get_id();
		code_handler.register_variable(id_str);
	}
}

void SubprogramDeclList::load_functions(CodeHandler &code_handler)
{
	auto functions = get_ast_nodes();
	for (auto function : functions)
	{
		function->load_functions(code_handler);
	}
}

void SubprogramDecl::load_functions(CodeHandler &code_handler)
{
	if (header != NULL)
	{
		header->load_functions(code_handler);
	}
	if (variable_section != NULL)
	{
		variable_section->load_functions(code_handler);
	}
}

void SubprogramDeclHeader::load_functions(CodeHandler &code_handler)
{
	assert(id->get_type() == NodeEnum::StringNode);
	StringNode *string_node = static_cast<StringNode *>(id);
	const std::string id_str = string_node->get_id();

	code_handler.new_function(id_str);
	code_handler.change_context(id_str);

	if (arguments != NULL)
	{
		arguments->load_functions(code_handler);
	}
}

void ArgumentDeclList::load_functions(CodeHandler &code_handler)
{
	auto parameters = get_ast_nodes();
	for (auto parameter : parameters)
	{
		parameter->load_functions(code_handler);
	}
}

void ArgumentDecl::load_functions(CodeHandler &code_handler)
{
	assert(id->get_type() == NodeEnum::StringNode);
	StringNode *string_node = static_cast<StringNode *>(id);
	const std::string id_str = string_node->get_id();
	code_handler.register_function_parameter(id_str);
}

/*--------------- Generate code ---------------*/

void ProgramNode::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	ss << "extern init_screen\n"
	   << "extern end_screen\n"
	   << "extern limpiar_pantalla\n"
	   << "extern posicionar_cursor\n"
	   << "extern color_fondo\n"
	   << "extern color_texto\n"
	   << "extern caracter_ascii\n"
	   << "extern valor_ascii\n"
	   << "extern obtener_caracter\n"
	   << "extern obtener_tecla\n"
	   << "extern nueva_linea\n"
	   << "extern aleatorio\n"
	   << "extern inicializar_aleatorio\n"
	   << "extern tecla_presionada\n"
	   << "extern pausa\n"
	   << "extern read_int\n"
	   << "extern read_char\n"
	   << "extern read_bool\n"
	   << "extern print\n"
	   << "global main\n";

	std::ostringstream ss_subprogram_decl;
	if (subprogram_decl != NULL)
	{
		subprogram_decl->generate_code(code_handler);
		ss_subprogram_decl << subprogram_decl->get_code();
	}

	std::ostringstream ss_statement_list;
	if (statement_list != NULL)
	{
		statement_list->generate_code(code_handler);
		ss_statement_list << statement_list->get_code();
	}

	ss << "\nsection .data\n"
	   << code_handler.get_code();

	ss << "\nsection .text\n"
	   << ss_subprogram_decl.str();

	ss << "\nmain:\n"
	   << "call init_screen\n"
	   << ss_statement_list.str()
	   << "call obtener_tecla\n"
	   << "call end_screen\n"
	   << "mov eax, 0\n"
	   << "ret";

	set_code(ss.str());
}

void StatementList::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	auto statements = get_ast_nodes();
	for (auto statement : statements)
	{
		statement->generate_code(code_handler);
		ss << statement->get_code();
	}
	set_code(ss.str());
}

void WriteNode::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	std::ostringstream fmt;
	size_t argsize = 0;

	std::list<std::string> argscode;
	fmt << "\"";

	assert(argument_list->get_type() == NodeEnum::ASTNodeList);
	ASTNodeList *argument_list_as_ast_node_list = static_cast<ASTNodeList *>(argument_list);
	auto argument_list_as_list = argument_list_as_ast_node_list->get_ast_nodes();
	for (auto argument : argument_list_as_list)
	{
		argument->generate_code(code_handler);
		if (argument->get_type() == NodeEnum::StringLiteral)
		{
			fmt << "%s";
			argscode.push_front("push " + argument->get_place());
		}
		else if (argument->get_type() == NodeEnum::Expr)
		{
			Expr *argument_as_expr = static_cast<Expr *>(argument);
			if (argument_as_expr->infer_type(code_handler.get_context_name()) == TypeEnum::Entero)
			{
				fmt << "%d";
				argscode.push_front("push dword [" + argument->get_place() + "]");
			}
			else if (argument_as_expr->infer_type(code_handler.get_context_name()) == TypeEnum::Caracter)
			{
				fmt << "%c";
				argscode.push_front("push dword [" + argument->get_place() + "]");
			}
			else if (argument_as_expr->infer_type(code_handler.get_context_name()) == TypeEnum::Booleano)
			{
				fmt << "%b";
				argscode.push_front("push dword [" + argument->get_place() + "]");
			}
		}
		ss << argument->get_code();
		argsize += 4;
	}
	fmt << "\"";

	for (const auto &s : argscode)
	{
		ss << s << "\n";
	}

	std::string fmt_place = code_handler.register_string_literal(fmt.str());
	ss << "push " << fmt_place << "\n"
	   << "call print\n"
	   << "add esp, " << (argsize + 4) << "\n";

	set_code(ss.str());
}

void StringLiteralNode::generate_code(CodeHandler &code_handler)
{
	set_place(code_handler.register_string_literal(string_literal));
}

void CharacterLiteralNode::generate_code(CodeHandler &code_handler)
{
	set_place(code_handler.register_character_literal(character_literal));
}

void NumberNode::generate_code(CodeHandler &code_handler)
{
	set_place(code_handler.register_constant(number));
}

void StringNode::generate_code(CodeHandler &code_handler)
{
	set_place(code_handler.register_variable(id));
}

void LeftValue::generate_code(CodeHandler &code_handler) // work in progress
{
	id->generate_code(code_handler);
	set_place(id->get_place());
}

void AssignStatement::generate_code(CodeHandler &code_handler)
{
	lvalue->generate_code(code_handler);
	expr->generate_code(code_handler);

	std::ostringstream ss;
	ss << expr->get_code()
	   << "mov eax, dword [" << expr->get_place() << "]\n"
	   << "mov dword [" << lvalue->get_place() << "], eax\n";

	set_code(ss.str());
	set_place(lvalue->get_place());
}

void BooleanNode::generate_code(CodeHandler &code_handler)
{
	if (boolean)
	{
		set_place(code_handler.register_constant(1));
	}
	else
	{
		set_place(code_handler.register_constant(0));
	}
}

void ReadNode::generate_code(CodeHandler &code_handler)
{
	assert(expr->get_type() == NodeEnum::Expr);
	std::ostringstream ss;
	expr->generate_code(code_handler);

	Expr *expr_as_expr = static_cast<Expr *>(expr);

	if (expr_as_expr->infer_type(code_handler.get_context_name()) == TypeEnum::Entero)
	{
		ss << "call read_int\n";
	}
	else if (expr_as_expr->infer_type(code_handler.get_context_name()) == TypeEnum::Caracter)
	{
		ss << "call read_char\n";
	}
	else if (expr_as_expr->infer_type(code_handler.get_context_name()) == TypeEnum::Booleano)
	{
		ss << "call read_bool\n";
	}
	ss << "mov dword [" << expr->get_place() << "], eax\n"
	   << "call nueva_linea\n";

	set_code(ss.str());
}

void IfStatement::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	expr->generate_code(code_handler);
	if (statement_list != NULL)
	{
		statement_list->generate_code(code_handler);
	}
	if (else_statement != NULL)
	{
		else_statement->generate_code(code_handler);
	}

	std::string else_label = code_handler.new_label();
	std::string endif_label = code_handler.new_label();

	ss << expr->get_code()
	   << "cmp dword [" << expr->get_place() << "], 0\n"
	   << "je " << else_label << "\n";

	if (statement_list != NULL)
	{
		ss << statement_list->get_code();
	}

	ss << "jmp " << endif_label << "\n"
	   << else_label << ":\n";

	if (else_statement != NULL)
	{
		ss << else_statement->get_code();
	}
	ss << endif_label << ":\n";

	set_code(ss.str());
}

void ElseStatement::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	if (statement_list != NULL)
	{
		statement_list->generate_code(code_handler);
		ss << statement_list->get_code();
	}
	set_code(ss.str());
}

void WhileStatement::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	expr->generate_code(code_handler);
	if (statement_list != NULL)
	{
		statement_list->generate_code(code_handler);
	}

	std::string loop_label = code_handler.new_label();
	std::string end_loop_label = code_handler.new_label();

	ss << loop_label << ":\n"
	   << expr->get_code()
	   << "cmp dword [" << expr->get_place() << "], 0\n"
	   << "je " << end_loop_label << "\n";

	if (statement_list != NULL)
	{
		ss << statement_list->get_code();
	}

	ss << "jmp " << loop_label << "\n"
	   << end_loop_label << ":\n";

	set_code(ss.str());
}

void ForStatement::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;

	assign->generate_code(code_handler);
	expr->generate_code(code_handler);
	if (statement_list != NULL)
	{
		statement_list->generate_code(code_handler);
	}

	std::string for_label = code_handler.new_label();
	std::string end_for_label = code_handler.new_label();

	ss << for_label << ":\n"
	   << expr->get_code()
	   << "mov eax, dword [" << expr->get_place() << "]\n"
	   << "cmp dword [" << assign->get_place() << "], eax\n"
	   << "jg " << end_for_label << "\n";

	if (statement_list != NULL)
	{
		ss << statement_list->get_code();
	}

	ss << "inc dword [" << assign->get_place() << "]\n"
	   << "jmp " << for_label << "\n"
	   << end_for_label << ":\n";

	set_code(ss.str());
}

void NotDoWhileStatement::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	expr->generate_code(code_handler);
	if (statement_list != NULL)
	{
		statement_list->generate_code(code_handler);
	}

	std::string loop_label = code_handler.new_label();
	std::string end_loop_label = code_handler.new_label();

	ss << loop_label << ":\n";

	if (statement_list != NULL)
	{
		ss << statement_list->get_code();
	}

	ss << expr->get_code()
	   << "cmp dword [" << expr->get_place() << "], 1\n"
	   << "je " << end_loop_label << "\n"
	   << "jmp " << loop_label << "\n"
	   << end_loop_label << ":\n";

	set_code(ss.str());
}

void ReturnNode::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	expr->generate_code(code_handler);

	ss << expr->get_code()
	   << "mov eax, dword [" << expr->get_place() << "]\n";

	set_code(ss.str());
}

void EqualExpr::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	ASTNode *expr1 = get_expr1();
	ASTNode *expr2 = get_expr2();

	expr1->generate_code(code_handler);
	expr2->generate_code(code_handler);

	set_place(code_handler.new_temp());

	ss << expr1->get_code()
	   << expr2->get_code()
	   << "mov eax, dword [" << expr1->get_place() << "]\n"
	   << "cmp eax, dword [" << expr2->get_place() << "]\n"
	   << "sete al\n"
	   << "cbw\n"
	   << "cwd\n"
	   << "mov dword [" << get_place() << "], eax\n";

	set_code(ss.str());
}

void NotEqualExpr::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	ASTNode *expr1 = get_expr1();
	ASTNode *expr2 = get_expr2();

	expr1->generate_code(code_handler);
	expr2->generate_code(code_handler);

	set_place(code_handler.new_temp());

	ss << expr1->get_code()
	   << expr2->get_code()
	   << "mov eax, dword [" << expr1->get_place() << "]\n"
	   << "cmp eax, dword [" << expr2->get_place() << "]\n"
	   << "setne al\n"
	   << "cbw\n"
	   << "cwd\n"
	   << "mov dword [" << get_place() << "], eax\n";

	set_code(ss.str());
}

void LessThanExpr::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	ASTNode *expr1 = get_expr1();
	ASTNode *expr2 = get_expr2();

	expr1->generate_code(code_handler);
	expr2->generate_code(code_handler);

	set_place(code_handler.new_temp());

	ss << expr1->get_code()
	   << expr2->get_code()
	   << "mov eax, dword [" << expr1->get_place() << "]\n"
	   << "cmp eax, dword [" << expr2->get_place() << "]\n"
	   << "setl al\n"
	   << "cbw\n"
	   << "cwd\n"
	   << "mov dword [" << get_place() << "], eax\n";

	set_code(ss.str());
}

void GreaterThanExpr::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	ASTNode *expr1 = get_expr1();
	ASTNode *expr2 = get_expr2();

	expr1->generate_code(code_handler);
	expr2->generate_code(code_handler);

	set_place(code_handler.new_temp());

	ss << expr1->get_code()
	   << expr2->get_code()
	   << "mov eax, dword [" << expr1->get_place() << "]\n"
	   << "cmp eax, dword [" << expr2->get_place() << "]\n"
	   << "setg al\n"
	   << "cbw\n"
	   << "cwd\n"
	   << "mov dword [" << get_place() << "], eax\n";

	set_code(ss.str());
}

void LessThanOrEqualExpr::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	ASTNode *expr1 = get_expr1();
	ASTNode *expr2 = get_expr2();

	expr1->generate_code(code_handler);
	expr2->generate_code(code_handler);

	set_place(code_handler.new_temp());

	ss << expr1->get_code()
	   << expr2->get_code()
	   << "mov eax, dword [" << expr1->get_place() << "]\n"
	   << "cmp eax, dword [" << expr2->get_place() << "]\n"
	   << "setle al\n"
	   << "cbw\n"
	   << "cwd\n"
	   << "mov dword [" << get_place() << "], eax\n";

	set_code(ss.str());
}

void GreaterThanOrEqualExpr::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	ASTNode *expr1 = get_expr1();
	ASTNode *expr2 = get_expr2();

	expr1->generate_code(code_handler);
	expr2->generate_code(code_handler);

	set_place(code_handler.new_temp());

	ss << expr1->get_code()
	   << expr2->get_code()
	   << "mov eax, dword [" << expr1->get_place() << "]\n"
	   << "cmp eax, dword [" << expr2->get_place() << "]\n"
	   << "setge al\n"
	   << "cbw\n"
	   << "cwd\n"
	   << "mov dword [" << get_place() << "], eax\n";

	set_code(ss.str());
}

void AdditionExpr::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	ASTNode *expr1 = get_expr1();
	ASTNode *expr2 = get_expr2();

	expr1->generate_code(code_handler);
	expr2->generate_code(code_handler);

	set_place(code_handler.new_temp());

	ss << expr1->get_code()
	   << expr2->get_code()
	   << "mov eax, dword [" << expr1->get_place() << "]\n"
	   << "add eax, dword [" << expr2->get_place() << "]\n"
	   << "mov dword [" << get_place() << "], eax\n";

	set_code(ss.str());
}

void SubtractionExpr::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	ASTNode *expr1 = get_expr1();
	ASTNode *expr2 = get_expr2();

	expr1->generate_code(code_handler);
	expr2->generate_code(code_handler);

	set_place(code_handler.new_temp());

	ss << expr1->get_code()
	   << expr2->get_code()
	   << "mov eax, dword [" << expr1->get_place() << "]\n"
	   << "sub eax, dword [" << expr2->get_place() << "]\n"
	   << "mov dword [" << get_place() << "], eax\n";

	set_code(ss.str());
}

void OrExpr::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	ASTNode *expr1 = get_expr1();
	ASTNode *expr2 = get_expr2();

	expr1->generate_code(code_handler);
	expr2->generate_code(code_handler);

	set_place(code_handler.new_temp());

	ss << expr1->get_code()
	   << expr2->get_code()
	   << "mov eax, dword [" << expr1->get_place() << "]\n"
	   << "or eax, dword [" << expr2->get_place() << "]\n"
	   << "mov dword [" << get_place() << "], eax\n";

	set_code(ss.str());
}

void MultiplicationExpr::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	ASTNode *expr1 = get_expr1();
	ASTNode *expr2 = get_expr2();

	expr1->generate_code(code_handler);
	expr2->generate_code(code_handler);

	set_place(code_handler.new_temp());

	ss << expr1->get_code()
	   << expr2->get_code()
	   << "mov eax, dword [" << expr1->get_place() << "]\n"
	   << "imul eax, dword [" << expr2->get_place() << "]\n"
	   << "mov dword [" << get_place() << "], eax\n";

	set_code(ss.str());
}

void DivisionExpr::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	ASTNode *expr1 = get_expr1();
	ASTNode *expr2 = get_expr2();

	expr1->generate_code(code_handler);
	expr2->generate_code(code_handler);

	set_place(code_handler.new_temp());

	ss << expr1->get_code()
	   << expr2->get_code()
	   << "mov eax, dword [" << expr1->get_place() << "]\n"
	   << "mov ebx, dword [" << expr2->get_place() << "]\n"
	   << "cdq\n"
	   << "idiv ebx\n"
	   << "mov dword [" << get_place() << "], eax\n";

	set_code(ss.str());
}

void ModExpr::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	ASTNode *expr1 = get_expr1();
	ASTNode *expr2 = get_expr2();

	expr1->generate_code(code_handler);
	expr2->generate_code(code_handler);

	set_place(code_handler.new_temp());

	ss << expr1->get_code()
	   << expr2->get_code()
	   << "mov eax, dword [" << expr1->get_place() << "]\n"
	   << "mov ebx, dword [" << expr2->get_place() << "]\n"
	   << "cdq\n"
	   << "idiv ebx\n"
	   << "mov dword [" << get_place() << "], edx\n";

	set_code(ss.str());
}

void AndExpr::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	ASTNode *expr1 = get_expr1();
	ASTNode *expr2 = get_expr2();

	expr1->generate_code(code_handler);
	expr2->generate_code(code_handler);

	set_place(code_handler.new_temp());

	ss << expr1->get_code()
	   << expr2->get_code()
	   << "mov eax, dword [" << expr1->get_place() << "]\n"
	   << "and eax, dword [" << expr2->get_place() << "]\n"
	   << "mov dword [" << get_place() << "], eax\n";

	set_code(ss.str());
}

void PowerExpr::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;
	ASTNode *expr1 = get_expr1();
	ASTNode *expr2 = get_expr2();

	expr1->generate_code(code_handler);
	expr2->generate_code(code_handler);

	set_place(code_handler.new_temp());

	std::string loop_label = code_handler.new_label();
	std::string end_loop_label = code_handler.new_label();

	ss << expr1->get_code()
	   << expr2->get_code()
	   << "mov eax, dword [" << expr1->get_place() << "]\n"
	   << "mov ebx, dword [" << expr1->get_place() << "]\n"
	   << "mov ecx, 1\n"
	   << loop_label << ":\n"
	   << "cmp ecx, dword [" << expr2->get_place() << "]\n"
	   << "jge " << end_loop_label << "\n"
	   << "imul eax, ebx\n"
	   << "inc ecx\n"
	   << "jmp " << loop_label << "\n"
	   << end_loop_label << ":\n"
	   << "mov dword [" << get_place() << "], eax\n";

	set_code(ss.str());
}

void NegativeExpr::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;

	expr->generate_code(code_handler);

	set_place(code_handler.new_temp());

	ss << expr->get_code()
	   << "mov eax, dword [" << expr->get_place() << "]\n"
	   << "imul dword eax, -1\n"
	   << "mov dword [" << get_place() << "], eax\n";

	set_code(ss.str());
}

void NegateExpr::generate_code(CodeHandler &code_handler)
{
	std::ostringstream ss;

	expr->generate_code(code_handler);

	set_place(code_handler.new_temp());

	ss << expr->get_code()
	   << "mov eax, dword [" << expr->get_place() << "]\n"
	   << "cmp dword eax, 0\n"
	   << "sete al\n"
	   << "cbw\n"
	   << "cwd\n"
	   << "mov dword [" << get_place() << "], eax\n";

	set_code(ss.str());
}

void SubprogramCall::generate_code(CodeHandler &code_handler)
{
	std::cerr << "Not implemented" << std::endl;
}
