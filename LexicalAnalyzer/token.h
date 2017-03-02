#include <string>

enum TokenType {
	WHITESPACE,
	PROGRAM,
	VAR,
	CONST,
	TYPE,
	FUNCTION,
	RETURN,
	BEGIN,
	END,
	SWAP,
	ASSIGNMENT,
	OUTPUT,
	IF,
	THEN,
	ELSE,
	WHILE,
	DO,
	CASE,
	OF,
	CASE_DOTS,
	OTHERWISE,
	REPEAT,
	FOR,
	UNTIL,
	LOOP,
	LOOP_END,
	EXIT,
	LESS_OR_EQUAL,
	NOT_EQUAL,
	LESS_THAN,
	GREATER_OR_EQUAL,
	GREATER_THAN,
	EQUAL,
	MOD,
	AND,
	OR,
	NOT,
	READ,
	SUCC,
	PRED,
	CHR,
	ORD,
	EOF_ID,
	COMMENT_START,
	COMMENT_END,
	COLON,
	SEMICOLON,
	DOT,
	COMMA,
	PARENTHESIS_START,
	PARENTHESIS_END,
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE,
	UNKNOWN // error
};


class Token {
protected:
	std::string text;
	TokenType type;

public:
	Token(std::string text, TokenType type);
	std::string getText() { return this->text; }
	TokenType getType() { return this->type; }
};