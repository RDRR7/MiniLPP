#include <iostream>
#include <fstream>
#include "parser.hpp"
#include "code_handler.hpp"

std::ifstream in;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage " << argv[0] << " <input file>" << std::endl;
		return 1;
	}

	in = std::ifstream(argv[1], std::ios::in);

	if (!in.is_open())
	{
		std::cerr << "Cannot open input file '" << argv[1] << "'" << std::endl;
		return 1;
	}

	ASTNode *program_node;
	if (yyparse(&program_node))
	{
		return 1;
	}

	// std::cout << program_node->to_string() << std::endl;
	program_node->pre_syntax_analysis();
	program_node->syntax_analysis();

	CodeHandler code_handler;
	program_node->load_functions(code_handler);
	// code_handler.print_all();

	program_node->generate_code(code_handler);
	// code_handler.print_all();
	std::cout << program_node->get_code() << std::endl;

	delete program_node;
	return 0;
}
