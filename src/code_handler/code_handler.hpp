#ifndef CODE_HANDLER_H
#define CODE_HANDLER_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include "function_handler.hpp"

#define GLOBAL_CONTEXT "MAIN"

class CodeHandler
{
  public:
	CodeHandler()
		: label_count(0),
		  string_literal_count(0),
		  character_literal_count(0),
		  constants_count(0),
		  temp_count(0)
	{
		current_context = NULL;
	}
	~CodeHandler()
	{
		auto it = functions.begin();
		if (it != functions.end())
		{
			delete it->second;
			functions.erase(it);
		}
	}
	std::string new_label();
	void new_function(std::string name);
	void change_context(std::string name);
	void register_variable(std::string name);
	std::string register_string_literal(std::string value);
	void register_character_literal(std::string value);
	void register_constant(int value);
	void register_function_parameter(std::string name);
	std::string get_variable_place(std::string name);
	std::string get_string_literal_place(std::string value);
	std::string get_character_literal_place(std::string value);
	std::string get_constant_place(int value);
	std::string new_temp();
	void print_all();
	std::string get_code();

  private:
	int label_count;
	int string_literal_count;
	int character_literal_count;
	int constants_count;
	int temp_count;
	std::unordered_set<std::string> variables;
	std::unordered_map<std::string, std::string> literals;
	std::unordered_map<int, std::string> constants;
	std::unordered_map<std::string, FunctionHandler *> functions;
	FunctionHandler *current_context;

	std::string new_string_literal();
	std::string new_character_literal();
	std::string new_constant();
};

#endif
