#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <string>

using yyscan_t = void *;

enum class Token
{
	OP_ADD = '+',
	OP_SUB = '-',
	OP_MUL = '*',
	OP_DIV = '/',
	TK_OPENPAR = '(',
	TK_CLOSEPAR = ')',
	TK_ASSIGN = '=',
	TK_SEMICOLON = ';',
	TK_UNKNOWN = 998,
};

class Lexer
{
  public:
	Lexer(std::ifstream &in);
	~Lexer();

	int get_next_token() { return _get_next_token(scanner); }
	std::string get_lexeme();
	int get_line();

  private:
	std::ifstream &in;
	yyscan_t scanner;

	int _get_next_token(yyscan_t yyscanner);
};

#endif
