#include <iostream>
#include <fstream>
#include "./lexer/lexer.hpp"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage " << argv[0] << " <input file>" << std::endl;
		return 1;
	}

	std::ifstream in = std::ifstream(argv[1], std::ios::in);

	if (!in.is_open())
	{
		std::cerr << "Cannot open input file '" << argv[1] << "'" << std::endl;
		return 1;
	}

	Lexer lexer(in);

	while (lexer.get_next_token() != 0)
	{
		std::cout << lexer.get_lexeme() << std::endl;
	}

	return 0;
}
