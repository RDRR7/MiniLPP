#include "nodes.hpp"
#include <cassert>

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
