#include "code_handler.hpp"
#include <sstream>
#include <cassert>

std::string CodeHandler::new_string_literal()
{
	std::ostringstream ss;
	ss << "str" << string_literal_count++;
	return ss.str();
}

std::string CodeHandler::new_character_literal()
{
	std::ostringstream ss;
	ss << "char" << character_literal_count++;
	return ss.str();
}

std::string CodeHandler::new_constant()
{
	std::ostringstream ss;
	ss << "const" << constants_count++;
	return ss.str();
}

std::string CodeHandler::new_label()
{
	std::ostringstream ss;
	ss << "lbl" << label_count++;
	return ss.str();
}

void CodeHandler::new_function(std::string name)
{
	FunctionHandler *function = new FunctionHandler();
	functions[name] = function;
}

void CodeHandler::change_context(std::string name)
{
	if (name.compare(GLOBAL_CONTEXT) == 0)
	{
		current_context = NULL;
	}
	else
	{
		current_context = functions[name];
	}
}

void CodeHandler::register_variable(std::string name)
{
	if (current_context == NULL)
	{
		variables.insert(name);
	}
	else
	{
		current_context->register_variable(name);
	}
}

void CodeHandler::register_string_literal(std::string value)
{
	if (literals.find(value) == literals.end())
	{
		literals[value] = new_string_literal();
	}
}

void CodeHandler::register_character_literal(std::string value)
{
	if (literals.find(value) == literals.end())
	{
		literals[value] = new_character_literal();
	}
}

void CodeHandler::register_constant(int value)
{
	if (constants.find(value) == constants.end())
	{
		constants[value] = new_constant();
	}
}

void CodeHandler::register_function_parameter(std::string name)
{
	assert(current_context != NULL);
	current_context->register_parameter(name);
}

std::string CodeHandler::get_variable_place(std::string name)
{
	if (current_context == NULL)
	{
		return name;
	}
	else
	{
		std::string place = current_context->get_variable_place(name);
		if (place.empty())
		{
			return name;
		}
		else
		{
			return place;
		}
	}
}

std::string CodeHandler::get_string_literal_place(std::string value)
{
	return literals[value];
}

std::string CodeHandler::get_character_literal_place(std::string value)
{
	return literals[value];
}

std::string CodeHandler::get_constant_place(int value)
{
	return constants[value];
}

std::string CodeHandler::new_temp()
{
	std::ostringstream ss;
	ss << "temp" << temp_count++;
	return ss.str();
}
