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

	int token = lexer.get_next_token();
	while (token != 0)
	{
		if (token == static_cast<int>(Token::TK_UNKNOWN))
		{
			std::cout << "Unkown token " << lexer.get_lexeme() << std::endl;
			return 1;
		}
		std::cout << lexer.get_lexeme() << " " << token << std::endl;
		token = lexer.get_next_token();
	}

	return 0;
}
