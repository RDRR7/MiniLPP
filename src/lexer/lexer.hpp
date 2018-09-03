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
	OP_EXP = '^',
	OP_AND = 'y',
	OP_OR = 'o',
	TK_OPENBRA = '[',
	TK_CLOSEBRA = ']',
	TK_OPENPAR = '(',
	TK_CLOSEPAR = ')',
	TK_LT = '<',
	TK_GT = '>',
	TK_COLON = ':',
	TK_EQUAL = '=',
	TK_COMMA = ',',
	TK_SEMICOLON = ';',
	TK_EOL = '\n',
	TK_UNKNOWN = 998,
};

class Lexer
{
  public:
	Lexer(std::ifstream &in);
	~Lexer();
	int get_next_token() const
	{
		return _get_next_token(scanner);
	}
	std::string get_lexeme() const;
	int get_line() const;

  private:
	std::ifstream &in;
	yyscan_t scanner;

	int _get_next_token(yyscan_t yyscanner) const;
};

#endif
