#ifndef FUNCTION_HANDLER_H
#define FUNCTION_HANDLER_H

#include <unordered_map>
#include <string>
#include <sstream>

class FunctionHandler
{
  public:
	FunctionHandler()
		: parameter_offset(8),
		  variable_offset(-4) {}
	~FunctionHandler() {}
	void register_parameter(std::string name);
	void register_variable(std::string name);
	std::string get_variable_place(std::string name);
	void print_all();

  private:
	int parameter_offset;
	int variable_offset;
	std::unordered_map<std::string, std::string> parameters;
	std::unordered_map<std::string, std::string> variables;

	std::string new_parameter();
	std::string new_variable();
};

#endif
