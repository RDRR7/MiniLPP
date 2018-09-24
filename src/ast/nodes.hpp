#ifndef NODES_H
#define NODES_H

#include <iostream>
#include <string>
#include <list>
#include "code_handler.hpp"

#define GLOBAL_CONTEXT "MAIN"
#define RETURN_TYPE "RETURN_TYPE"
#define DEFINE_BINARY_EXPR(name, prec, oper)                     \
	class name##Expr : public BinaryExpr                         \
	{                                                            \
	  public:                                                    \
		name##Expr(int line,                                     \
				   ASTNode *expr1,                               \
				   ASTNode *expr2)                               \
			: BinaryExpr(line,                                   \
						 expr1,                                  \
						 expr2) {}                               \
		int get_precedence() const                               \
		{                                                        \
			return prec;                                         \
		}                                                        \
		std::string get_oper() const                             \
		{                                                        \
			return oper;                                         \
		}                                                        \
		TypeEnum infer_type(std::string context) const override; \
	};

enum class TypeEnum : unsigned int
{
	Entero = 1,
	Booleano = 2,
	Caracter = 3,
	Arreglo = 4,
	Tipo = 5,
	ArregloDeEntero = 6,
	ArregloDeCaracter = 7,
	ArregloDeBooleano = 8,
};

enum class NodeEnum : unsigned int
{
	ASTNodeList = 1,
	StringNode = 2,
	SubprogramDeclHeader = 3,
	Expr = 4,
	Type = 5,
	AssignStatement = 6,
	StringLiteral = 7,
	Other = 255,
};

class ASTNode
{
  public:
	ASTNode(int line)
		: line(line)
	{
		code = "";
		place = "";
	}
	virtual ~ASTNode() {}
	virtual std::string to_string() const = 0;
	virtual NodeEnum get_type() const
	{
		return NodeEnum::Other;
	}
	int get_line() const
	{
		return line;
	}
	virtual void pre_syntax_analysis(std::string context = GLOBAL_CONTEXT) {}
	virtual void syntax_analysis(std::string context = GLOBAL_CONTEXT) {}
	virtual void load_functions(CodeHandler &code_handler) {}
	std::string get_code()
	{
		return code;
	}
	std::string get_place()
	{
		return place;
	}
	void set_code(std::string code)
	{
		this->code = code;
	}
	void set_place(std::string place)
	{
		this->place = place;
	}
	virtual void generate_code(CodeHandler &code_handler)
	{
		std::cerr << "Not implemented" << std::endl;
	}

  private:
	int line;
	std::string code;
	std::string place;
};

class ProgramNode : public ASTNode
{
  public:
	ProgramNode(int line,
				ASTNode *type_definition_section,
				ASTNode *variable_section,
				ASTNode *subprogram_decl,
				ASTNode *statement_list)
		: ASTNode(line),
		  type_definition_section(type_definition_section),
		  variable_section(variable_section),
		  subprogram_decl(subprogram_decl),
		  statement_list(statement_list) {}
	~ProgramNode() override
	{
		delete type_definition_section;
		delete variable_section;
		delete subprogram_decl;
		delete statement_list;
	}
	std::string to_string() const override;
	void pre_syntax_analysis(std::string context) override;
	void syntax_analysis(std::string context) override;
	void load_functions(CodeHandler &code_handler) override;
	void generate_code(CodeHandler &code_handler) override;

  private:
	ASTNode *type_definition_section;
	ASTNode *variable_section;
	ASTNode *subprogram_decl;
	ASTNode *statement_list;
};

class ASTNodeList : public ASTNode
{
  public:
	ASTNodeList(int line)
		: ASTNode(line) {}
	~ASTNodeList() override
	{
		while (!ast_nodes.empty())
		{
			delete ast_nodes.front();
			ast_nodes.pop_front();
		}
	}
	std::list<ASTNode *> get_ast_nodes() const
	{
		return ast_nodes;
	}
	std::list<ASTNode *> *get_pointer_to_ast_nodes()
	{
		return &ast_nodes;
	}
	NodeEnum get_type() const override
	{
		return NodeEnum::ASTNodeList;
	}
	static void add_to_list(ASTNode *list,
							ASTNode *element);

  private:
	std::list<ASTNode *> ast_nodes;
};

class TypeDefinitionList : public ASTNodeList
{
  public:
	TypeDefinitionList(int line)
		: ASTNodeList(line) {}
	std::string to_string() const override;
	void pre_syntax_analysis(std::string context) override;
};

class TypeDefinition : public ASTNode
{
  public:
	TypeDefinition(int line,
				   ASTNode *id,
				   ASTNode *type)
		: ASTNode(line),
		  id(id),
		  type(type) {}
	~TypeDefinition() override
	{
		delete id;
		delete type;
	}
	std::string to_string() const override;
	void pre_syntax_analysis(std::string context) override;

  private:
	ASTNode *id;
	ASTNode *type;
};

class Type : public ASTNode
{
  public:
	Type(int line,
		 TypeEnum type,
		 ASTNode *array_size,
		 ASTNode *user_type,
		 ASTNode *array_type)
		: ASTNode(line),
		  type(type),
		  array_size(array_size),
		  user_type(user_type),
		  array_type(array_type) {}
	~Type() override
	{
		delete array_size;
		delete user_type;
		delete array_type;
	}
	std::string to_string() const override;
	NodeEnum get_type() const override
	{
		return NodeEnum::Type;
	}
	TypeEnum infer_type(std::string context) const;

  private:
	TypeEnum type;
	ASTNode *array_size;
	ASTNode *user_type;
	ASTNode *array_type;
};

class StringNode : public ASTNode
{
  public:
	StringNode(int line,
			   std::string id)
		: ASTNode(line),
		  id(id) {}
	std::string to_string() const override
	{
		return id;
	}
	NodeEnum get_type() const override
	{
		return NodeEnum::StringNode;
	}
	std::string get_id() const
	{
		return id;
	}
	void generate_code(CodeHandler &code_handler) override;

  private:
	std::string id;
};

class Expr : public ASTNode
{
  public:
	Expr(int line)
		: ASTNode(line) {}
	virtual std::string to_string() const = 0;
	NodeEnum get_type() const override
	{
		return NodeEnum::Expr;
	}
	std::string get_oper() const
	{
		return "";
	}
	virtual int get_precedence() const
	{
		return 255;
	}
	virtual TypeEnum infer_type(std::string context) const = 0;
};

class NumberNode : public Expr
{
  public:
	NumberNode(int line,
			   int number)
		: Expr(line),
		  number(number) {}
	std::string to_string() const override
	{
		return std::to_string(number);
	}
	TypeEnum infer_type(std::string context) const override
	{
		return TypeEnum::Entero;
	}
	void generate_code(CodeHandler &code_handler) override;

  private:
	int number;
};

class VariableSectionList : public ASTNodeList
{
  public:
	VariableSectionList(int line)
		: ASTNodeList(line) {}
	std::string to_string() const;
	void pre_syntax_analysis(std::string context) override;
	void load_functions(CodeHandler &code_handler) override;
};

class VariableSection : public ASTNode
{
  public:
	VariableSection(int line,
					ASTNode *ids,
					ASTNode *type)
		: ASTNode(line),
		  ids(ids),
		  type(type) {}
	~VariableSection() override
	{
		delete ids;
		delete type;
	}
	std::string to_string() const override;
	void pre_syntax_analysis(std::string context) override;
	void load_functions(CodeHandler &code_handler) override;

  private:
	ASTNode *ids;
	ASTNode *type;
};

class IdList : public ASTNodeList
{
  public:
	IdList(int line)
		: ASTNodeList(line) {}
	std::string to_string() const override;
	void load_functions(CodeHandler &code_handler) override;
};

class SubprogramDeclList : public ASTNodeList
{
  public:
	SubprogramDeclList(int line)
		: ASTNodeList(line) {}
	std::string to_string() const override;
	void pre_syntax_analysis(std::string context) override;
	void syntax_analysis(std::string context) override;
	void load_functions(CodeHandler &code_handler) override;
};

class SubprogramDecl : public ASTNode
{
  public:
	SubprogramDecl(int line,
				   ASTNode *header,
				   ASTNode *variable_section,
				   ASTNode *statements)
		: ASTNode(line),
		  header(header),
		  variable_section(variable_section),
		  statements(statements) {}
	~SubprogramDecl() override
	{
		delete header;
		delete variable_section;
		delete statements;
	}
	std::string to_string() const override;
	void pre_syntax_analysis(std::string context) override;
	void syntax_analysis(std::string context) override;
	void load_functions(CodeHandler &code_handler) override;

  private:
	ASTNode *header;
	ASTNode *variable_section;
	ASTNode *statements;
};

class SubprogramDeclHeader : public ASTNode
{
  public:
	SubprogramDeclHeader(int line,
						 ASTNode *id,
						 ASTNode *arguments,
						 ASTNode *type)
		: ASTNode(line),
		  id(id),
		  arguments(arguments),
		  type(type) {}
	~SubprogramDeclHeader() override
	{
		delete id;
		delete arguments;
		delete type;
	}
	std::string to_string() const override;
	NodeEnum get_type() const override
	{
		return NodeEnum::SubprogramDeclHeader;
	}
	ASTNode *get_id()
	{
		return id;
	}
	void pre_syntax_analysis(std::string context) override;
	void load_functions(CodeHandler &code_handler) override;

  private:
	ASTNode *id;
	ASTNode *arguments;
	ASTNode *type;
};

class ArgumentDeclList : public ASTNodeList
{
  public:
	ArgumentDeclList(int line)
		: ASTNodeList(line) {}
	std::string to_string() const override;
	void pre_syntax_analysis(std::string context) override;
	void load_functions(CodeHandler &code_handler) override;
};

class ArgumentDecl : public ASTNode
{
  public:
	ArgumentDecl(int line,
				 ASTNode *type,
				 ASTNode *id,
				 bool var)
		: ASTNode(line),
		  type(type),
		  id(id),
		  var(var) {}
	~ArgumentDecl() override
	{
		delete type;
		delete id;
	}
	std::string to_string() const override;
	void pre_syntax_analysis(std::string context) override;
	void load_functions(CodeHandler &code_handler) override;

  private:
	ASTNode *type;
	ASTNode *id;
	bool var;
};

class StatementList : public ASTNodeList
{
  public:
	StatementList(int line)
		: ASTNodeList(line) {}
	std::string to_string() const override;
	void syntax_analysis(std::string context) override;
	void generate_code(CodeHandler &code_handler) override;
};

class AssignStatement : public ASTNode
{
  public:
	AssignStatement(int line,
					ASTNode *lvalue,
					ASTNode *expr)
		: ASTNode(line),
		  lvalue(lvalue),
		  expr(expr) {}
	~AssignStatement() override
	{
		delete lvalue;
		delete expr;
	}
	std::string to_string() const override;
	NodeEnum get_type() const override
	{
		return NodeEnum::AssignStatement;
	}
	void syntax_analysis(std::string context) override;
	TypeEnum infer_type(std::string context) const;
	void generate_code(CodeHandler &code_handler) override;

  private:
	ASTNode *lvalue;
	ASTNode *expr;
};

class LeftValue : public Expr
{
  public:
	LeftValue(int line,
			  ASTNode *id,
			  ASTNode *index)
		: Expr(line),
		  id(id),
		  index(index) {}
	~LeftValue() override
	{
		delete id;
		delete index;
	}
	std::string to_string() const override;
	TypeEnum infer_type(std::string context) const override;
	void generate_code(CodeHandler &code_handler) override;

  private:
	ASTNode *id;
	ASTNode *index;
};

class BinaryExpr : public Expr
{
  public:
	BinaryExpr(int line,
			   ASTNode *expr1,
			   ASTNode *expr2)
		: Expr(line),
		  expr1(expr1),
		  expr2(expr2) {}
	~BinaryExpr() override
	{
		delete expr1;
		delete expr2;
	}
	std::string to_string() const override;
	virtual std::string get_oper() const = 0;
	virtual int get_precedence() const = 0;
	virtual TypeEnum infer_type(std::string context) const override = 0;
	ASTNode *get_expr1() const
	{
		return expr1;
	}
	ASTNode *get_expr2() const
	{
		return expr2;
	}

  private:
	ASTNode *expr1;
	ASTNode *expr2;
};

DEFINE_BINARY_EXPR(Equal, 0, "=");
DEFINE_BINARY_EXPR(NotEqual, 0, "<>");
DEFINE_BINARY_EXPR(LessThan, 0, "<");
DEFINE_BINARY_EXPR(GreaterThan, 0, ">");
DEFINE_BINARY_EXPR(LessThanOrEqual, 0, "<=");
DEFINE_BINARY_EXPR(GreaterThanOrEqual, 0, ">=");
DEFINE_BINARY_EXPR(Addition, 1, "+");
DEFINE_BINARY_EXPR(Subtraction, 1, "-");
DEFINE_BINARY_EXPR(Or, 1, "o");
DEFINE_BINARY_EXPR(Multiplication, 2, "*");
DEFINE_BINARY_EXPR(Division, 2, "div");
DEFINE_BINARY_EXPR(Mod, 2, "mod");
DEFINE_BINARY_EXPR(And, 2, "y");
DEFINE_BINARY_EXPR(Power, 3, "^");

class NegativeExpr : public Expr
{
  public:
	NegativeExpr(int line,
				 ASTNode *expr)
		: Expr(line),
		  expr(expr) {}
	~NegativeExpr() override
	{
		delete expr;
	}
	std::string to_string() const override;
	TypeEnum infer_type(std::string context) const override;

  private:
	ASTNode *expr;
};

class StringLiteralNode : public ASTNode
{
  public:
	StringLiteralNode(int line,
					  std::string string_literal)
		: ASTNode(line),
		  string_literal(string_literal) {}
	std::string to_string() const override
	{
		return string_literal;
	}
	NodeEnum get_type() const override
	{
		return NodeEnum::StringLiteral;
	}
	void generate_code(CodeHandler &code_handler) override;

  private:
	std::string string_literal;
};

class CharacterLiteralNode : public Expr
{
  public:
	CharacterLiteralNode(int line,
						 std::string character_literal)
		: Expr(line),
		  character_literal(character_literal) {}
	std::string to_string() const override
	{
		return character_literal;
	}
	TypeEnum infer_type(std::string context) const override
	{
		return TypeEnum::Caracter;
	}
	void generate_code(CodeHandler &code_handler) override;

  private:
	std::string character_literal;
};

class BooleanNode : public Expr
{
  public:
	BooleanNode(int line,
				bool boolean)
		: Expr(line),
		  boolean(boolean) {}
	std::string to_string() const override;
	TypeEnum infer_type(std::string context) const override
	{
		return TypeEnum::Booleano;
	}
	void generate_code(CodeHandler &code_handler) override;

  private:
	bool boolean;
};

class SubprogramCall : public Expr
{
  public:
	SubprogramCall(int line,
				   ASTNode *id,
				   ASTNode *argument_list)
		: Expr(line),
		  id(id),
		  argument_list(argument_list) {}
	~SubprogramCall() override
	{
		delete id;
		delete argument_list;
	}
	std::string to_string() const override;
	ASTNode *get_id()
	{
		return id;
	}
	TypeEnum infer_type(std::string context) const override;
	void check_arguments(std::string context) const;

  private:
	ASTNode *id;
	ASTNode *argument_list;
};

class ArgumentList : public ASTNodeList
{
  public:
	ArgumentList(int line)
		: ASTNodeList(line) {}
	std::string to_string() const override;
};

class CallStatement : public ASTNode
{
  public:
	CallStatement(int line,
				  ASTNode *call)
		: ASTNode(line),
		  call(call) {}
	~CallStatement() override
	{
		delete call;
	}
	std::string to_string() const override;
	void syntax_analysis(std::string context) override;

  private:
	ASTNode *call;
};

class IfStatement : public ASTNode
{
  public:
	IfStatement(int line,
				ASTNode *expr,
				ASTNode *statement_list,
				ASTNode *else_statement)
		: ASTNode(line),
		  expr(expr),
		  statement_list(statement_list),
		  else_statement(else_statement) {}
	~IfStatement() override
	{
		delete expr;
		delete statement_list;
		delete else_statement;
	}
	std::string to_string() const override;
	void syntax_analysis(std::string context) override;

  private:
	ASTNode *expr;
	ASTNode *statement_list;
	ASTNode *else_statement;
};

class ElseStatement : public ASTNode
{
  public:
	ElseStatement(int line,
				  ASTNode *statement_list)
		: ASTNode(line),
		  statement_list(statement_list) {}
	~ElseStatement() override
	{
		delete statement_list;
	}
	std::string to_string() const override;
	void syntax_analysis(std::string context) override;

  private:
	ASTNode *statement_list;
};

class WhileStatement : public ASTNode
{
  public:
	WhileStatement(int line,
				   ASTNode *expr,
				   ASTNode *statement_list)
		: ASTNode(line),
		  expr(expr),
		  statement_list(statement_list) {}
	~WhileStatement() override
	{
		delete expr;
		delete statement_list;
	}
	std::string to_string() const override;
	void syntax_analysis(std::string context) override;

  private:
	ASTNode *expr;
	ASTNode *statement_list;
};

class ForStatement : public ASTNode
{
  public:
	ForStatement(int line,
				 ASTNode *assign,
				 ASTNode *expr,
				 ASTNode *statement_list)
		: ASTNode(line),
		  assign(assign),
		  expr(expr),
		  statement_list(statement_list) {}
	~ForStatement() override
	{
		delete assign;
		delete expr;
		delete statement_list;
	}
	std::string to_string() const override;
	void syntax_analysis(std::string context) override;

  private:
	ASTNode *assign;
	ASTNode *expr;
	ASTNode *statement_list;
};

class NotDoWhileStatement : public ASTNode
{
  public:
	NotDoWhileStatement(int line,
						ASTNode *expr,
						ASTNode *statement_list)
		: ASTNode(line),
		  expr(expr),
		  statement_list(statement_list) {}
	~NotDoWhileStatement() override
	{
		delete expr;
		delete statement_list;
	}
	std::string to_string() const override;
	void syntax_analysis(std::string context) override;

  private:
	ASTNode *expr;
	ASTNode *statement_list;
};

class ReturnNode : public ASTNode
{
  public:
	ReturnNode(int line,
			   ASTNode *expr)
		: ASTNode(line),
		  expr(expr) {}
	~ReturnNode() override
	{
		delete expr;
	}
	std::string to_string() const override;
	void syntax_analysis(std::string context) override;

  private:
	ASTNode *expr;
};

class WriteNode : public ASTNode
{
  public:
	WriteNode(int line,
			  ASTNode *argument_list)
		: ASTNode(line),
		  argument_list(argument_list) {}
	~WriteNode() override
	{
		delete argument_list;
	}
	std::string to_string() const override;
	void syntax_analysis(std::string context) override;
	void generate_code(CodeHandler &code_handler) override;

  private:
	ASTNode *argument_list;
};

class ReadNode : public ASTNode
{
  public:
	ReadNode(int line,
			 ASTNode *expr)
		: ASTNode(line),
		  expr(expr) {}
	~ReadNode() override
	{
		delete expr;
	}
	std::string to_string() const override;
	void syntax_analysis(std::string context) override;
	void generate_code(CodeHandler &code_handler) override;

  private:
	ASTNode *expr;
};

class NegateExpr : public Expr
{
  public:
	NegateExpr(int line,
			   ASTNode *expr)
		: Expr(line),
		  expr(expr) {}
	~NegateExpr() override
	{
		delete expr;
	}
	std::string to_string() const override;
	TypeEnum infer_type(std::string context) const override;

  private:
	ASTNode *expr;
};

#endif
