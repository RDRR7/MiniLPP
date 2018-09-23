#include "function_handler.hpp"

std::string FunctionHandler::new_parameter()
{
	std::ostringstream ss;
	ss << "ebp + " << parameter_offset;
	parameter_offset += 4;
	return ss.str();
}

std::string FunctionHandler::new_variable()
{
	std::ostringstream ss;
	ss << "ebp - " << (-1 * variable_offset);
	variable_offset -= 4;
	return ss.str();
}

void FunctionHandler::register_parameter(std::string name)
{
	parameters[name] = new_parameter();
}

void FunctionHandler::register_variable(std::string name)
{
	variables[name] = new_variable();
}

std::string FunctionHandler::get_variable_place(std::string name)
{
	if (variables.find(name) != variables.end())
	{
		return variables[name];
	}
	else if (parameters.find(name) != parameters.end())
	{
		return parameters[name];
	}
	else
	{
		"";
	}
}
