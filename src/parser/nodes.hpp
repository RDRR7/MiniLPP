#ifndef NODES_H
#define NODES_H

#include <iostream>
#include <string>
#include <list>

#define DEFINE_BINARY_EXPR(name, prec, oper)       \
	class name##Expr : public BinaryExpr           \
	{                                              \
	  public:                                      \
		name##Expr(ASTNode *expr1, ASTNode *expr2) \
			: BinaryExpr(expr1, expr2) {}          \
		int get_precedence() const                 \
		{                                          \
			return prec;                           \
		}                                          \
		std::string get_oper() const               \
		{                                          \
			return oper;                           \
		}                                          \
	};

enum class TypeEnum : unsigned int
{
	Entero = 1,
	Booleano = 2,
	Caracter = 3,
	Arreglo = 4,
	Tipo = 5,
};

enum class NodeEnum : unsigned int
{
	List = 1,
	Other = 255,
};

class ASTNode
{
  public:
	ASTNode() {}
	virtual ~ASTNode() {}
	virtual std::string to_string() const = 0;
	virtual NodeEnum get_type() const
	{
		return NodeEnum::Other;
	}
};

class ProgramNode : public ASTNode
{
  public:
	ProgramNode(ASTNode *type_definition_section,
				ASTNode *variable_section,
				ASTNode *subprogram_decl,
				ASTNode *statement_list)
		: type_definition_section(type_definition_section),
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

  private:
	ASTNode *type_definition_section;
	ASTNode *variable_section;
	ASTNode *subprogram_decl;
	ASTNode *statement_list;
};

class ASTNodeList : public ASTNode
{
  public:
	ASTNodeList() {}
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
		return NodeEnum::List;
	}
	static void add_to_list(ASTNode *list, ASTNode *element);

  private:
	std::list<ASTNode *> ast_nodes;
};

class TypeDefinitionList : public ASTNodeList
{
  public:
	TypeDefinitionList() {}
	std::string to_string() const override;
};

class TypeDefinition : public ASTNode
{
  public:
	TypeDefinition(ASTNode *id,
				   ASTNode *type)
		: id(id),
		  type(type) {}
	~TypeDefinition() override
	{
		delete id;
		delete type;
	}
	std::string to_string() const override;

  private:
	ASTNode *id;
	ASTNode *type;
};

class Type : public ASTNode
{
  public:
	Type(TypeEnum type,
		 ASTNode *array_size,
		 ASTNode *user_type,
		 ASTNode *array_type)
		: type(type),
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

  private:
	TypeEnum type;
	ASTNode *array_size;
	ASTNode *user_type;
	ASTNode *array_type;
};

class StringNode : public ASTNode
{
  public:
	StringNode(std::string id)
		: id(id) {}
	std::string to_string() const override
	{
		return id;
	}

  private:
	std::string id;
};

class Expr : public ASTNode
{
  public:
	Expr() {}
	virtual std::string to_string() const = 0;
	std::string get_oper() const
	{
		return "";
	}
	virtual int get_precedence() const
	{
		return 255;
	}
};

class NumberNode : public Expr
{
  public:
	NumberNode(int number)
		: number(number) {}
	std::string to_string() const override
	{
		return std::to_string(number);
	}

  private:
	int number;
};

class VariableSectionList : public ASTNodeList
{
  public:
	VariableSectionList() {}
	std::string to_string() const;
};

class VariableSection : public ASTNode
{
  public:
	VariableSection(ASTNode *ids,
					ASTNode *type)
		: ids(ids),
		  type(type) {}
	~VariableSection() override
	{
		delete ids;
		delete type;
	}
	std::string to_string() const override;

  private:
	ASTNode *ids;
	ASTNode *type;
};

class IdList : public ASTNodeList
{
  public:
	IdList() {}
	std::string to_string() const override;
};

class SubprogramDeclList : public ASTNodeList
{
  public:
	SubprogramDeclList() {}
	std::string to_string() const override;
};

class SubprogramDecl : public ASTNode
{
  public:
	SubprogramDecl(ASTNode *header,
				   ASTNode *variable_section,
				   ASTNode *statements)
		: header(header),
		  variable_section(variable_section),
		  statements(statements) {}
	~SubprogramDecl() override
	{
		delete header;
		delete variable_section;
		delete statements;
	}
	std::string to_string() const override;

  private:
	ASTNode *header;
	ASTNode *variable_section;
	ASTNode *statements;
};

class SubprogramDeclHeader : public ASTNode
{
  public:
	SubprogramDeclHeader(ASTNode *id,
						 ASTNode *arguments,
						 ASTNode *type)
		: id(id),
		  arguments(arguments),
		  type(type) {}
	~SubprogramDeclHeader() override
	{
		delete id;
		delete arguments;
		delete type;
	}
	std::string to_string() const override;

  private:
	ASTNode *id;
	ASTNode *arguments;
	ASTNode *type;
};

class ArgumentDeclList : public ASTNodeList
{
  public:
	ArgumentDeclList() {}
	std::string to_string() const override;
};

class ArgumentDecl : public ASTNode
{
  public:
	ArgumentDecl(ASTNode *type,
				 ASTNode *id,
				 bool var)
		: type(type),
		  id(id),
		  var(var) {}
	~ArgumentDecl() override
	{
		delete type;
		delete id;
	}
	std::string to_string() const override;

  private:
	ASTNode *type;
	ASTNode *id;
	bool var;
};

class StatementList : public ASTNodeList
{
  public:
	StatementList() {}
	std::string to_string() const override;
};

class AssignStatement : public ASTNode
{
  public:
	AssignStatement(ASTNode *lvalue,
					ASTNode *expr)
		: lvalue(lvalue),
		  expr(expr) {}
	~AssignStatement() override
	{
		delete lvalue;
		delete expr;
	}
	std::string to_string() const override;

  private:
	ASTNode *lvalue;
	ASTNode *expr;
};

class LeftValue : public Expr
{
  public:
	LeftValue(ASTNode *id,
			  ASTNode *index)
		: id(id),
		  index(index) {}
	~LeftValue() override
	{
		delete id;
		delete index;
	}
	std::string to_string() const override;

  private:
	ASTNode *id;
	ASTNode *index;
};

class BinaryExpr : public Expr
{
  public:
	BinaryExpr(ASTNode *expr1,
			   ASTNode *expr2)
		: expr1(expr1),
		  expr2(expr2) {}
	~BinaryExpr() override
	{
		delete expr1;
		delete expr2;
	}
	std::string to_string() const override;
	virtual std::string get_oper() const = 0;
	virtual int get_precedence() const = 0;

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
	NegativeExpr(ASTNode *expr)
		: expr(expr) {}
	~NegativeExpr() override
	{
		delete expr;
	}
	std::string to_string() const override;

  private:
	ASTNode *expr;
};

class StringLiteralNode : public ASTNode
{
  public:
	StringLiteralNode(std::string string_literal)
		: string_literal(string_literal) {}
	std::string to_string() const override
	{
		return string_literal;
	}

  private:
	std::string string_literal;
};

class CharacterLiteralNode : public Expr
{
  public:
	CharacterLiteralNode(std::string character_literal)
		: character_literal(character_literal) {}
	std::string to_string() const override
	{
		return character_literal;
	}

  private:
	std::string character_literal;
};

class BooleanNode : public Expr
{
  public:
	BooleanNode(bool boolean)
		: boolean(boolean) {}
	std::string to_string() const override;

  private:
	bool boolean;
};

class SubprogramCall : public Expr
{
  public:
	SubprogramCall(ASTNode *id,
				   ASTNode *argument_list)
		: id(id),
		  argument_list(argument_list) {}
	~SubprogramCall() override
	{
		delete id;
		delete argument_list;
	}
	std::string to_string() const override;

  private:
	ASTNode *id;
	ASTNode *argument_list;
};

class ArgumentList : public ASTNodeList
{
  public:
	ArgumentList() {}
	std::string to_string() const override;
};

class CallStatement : public ASTNode
{
  public:
	CallStatement(ASTNode *call)
		: call(call) {}
	~CallStatement() override
	{
		delete call;
	}
	std::string to_string() const override;

  private:
	ASTNode *call;
};

class IfStatement : public ASTNode
{
  public:
	IfStatement(ASTNode *expr,
				ASTNode *statement_list,
				ASTNode *else_statement)
		: expr(expr),
		  statement_list(statement_list),
		  else_statement(else_statement) {}
	~IfStatement() override
	{
		delete expr;
		delete statement_list;
		delete else_statement;
	}
	std::string to_string() const override;

  private:
	ASTNode *expr;
	ASTNode *statement_list;
	ASTNode *else_statement;
};

class ElseStatement : public ASTNode
{
  public:
	ElseStatement(ASTNode *statement_list)
		: statement_list(statement_list) {}
	~ElseStatement() override
	{
		delete statement_list;
	}
	std::string to_string() const override;

  private:
	ASTNode *statement_list;
};

class WhileStatement : public ASTNode
{
  public:
	WhileStatement(ASTNode *expr,
				   ASTNode *statement_list)
		: expr(expr),
		  statement_list(statement_list) {}
	~WhileStatement() override
	{
		delete expr;
		delete statement_list;
	}
	std::string to_string() const override;

  private:
	ASTNode *expr;
	ASTNode *statement_list;
};

class ForStatement : public ASTNode
{
  public:
	ForStatement(ASTNode *assign,
				 ASTNode *expr,
				 ASTNode *statement_list)
		: assign(assign),
		  expr(expr),
		  statement_list(statement_list) {}
	~ForStatement() override
	{
		delete assign;
		delete expr;
		delete statement_list;
	}
	std::string to_string() const override;

  private:
	ASTNode *assign;
	ASTNode *expr;
	ASTNode *statement_list;
};

class NotDoWhileStatement : public ASTNode
{
  public:
	NotDoWhileStatement(ASTNode *expr,
						ASTNode *statement_list)
		: expr(expr),
		  statement_list(statement_list) {}
	~NotDoWhileStatement() override
	{
		delete expr;
		delete statement_list;
	}
	std::string to_string() const override;

  private:
	ASTNode *expr;
	ASTNode *statement_list;
};

class ReturnNode : public ASTNode
{
  public:
	ReturnNode(ASTNode *expr)
		: expr(expr) {}
	~ReturnNode() override
	{
		delete expr;
	}
	std::string to_string() const override;

  private:
	ASTNode *expr;
};

class WriteNode : public ASTNode
{
  public:
	WriteNode(ASTNode *argument_list)
		: argument_list(argument_list) {}
	~WriteNode() override
	{
		delete argument_list;
	}
	std::string to_string() const override;

  private:
	ASTNode *argument_list;
};

class ReadNode : public ASTNode
{
  public:
	ReadNode(ASTNode *expr)
		: expr(expr) {}
	~ReadNode() override
	{
		delete expr;
	}
	std::string to_string() const override;

  private:
	ASTNode *expr;
};

class NegateExpr : public Expr
{
  public:
	NegateExpr(ASTNode *expr)
		: expr(expr) {}
	~NegateExpr() override
	{
		delete expr;
	}
	std::string to_string() const override;

  private:
	ASTNode *expr;
};

#endif
