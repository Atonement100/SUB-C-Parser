#pragma once 

#include <string>

typedef enum TokenType {
	WHITESPACE,
	ENDLINE,
	IDENTIFIER,
	INTEGER,
	CHAR,
	STRING,
	COMMENT,
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
	UNKNOWN, //unset
	INVALID //error
} TokenType;


class Token {
protected:
	std::string text;
	TokenType type;

public:
	Token();
	Token(std::string text, TokenType type);
	std::string getText() { return this->text; }
	TokenType getType() { return this->type; }
	std::string getTypeAsString() { return convertTokenTypeToString(this->type); }

	static std::string convertTokenTypeToString(TokenType type) {
		switch (type) {
		case WHITESPACE: return "whitespace";
		case ENDLINE: return "endline";
		case IDENTIFIER: return "<identifier>";
		case INTEGER: return "<integer>";
		case CHAR: return "<char>";
		case STRING: return "<string>";
		case COMMENT: return "<comment>";
		case PROGRAM: return "program";
		case VAR: return "var";
		case CONST: return "const";
		case TYPE: return "type";
		case FUNCTION: return "function";
		case RETURN: return "return";
		case BEGIN: return "begin";
		case END: return "end";
		case SWAP: return "swap";
		case ASSIGNMENT: return "assign";
		case OUTPUT: return "output";
		case IF: return "if";
		case THEN: return "then";
		case ELSE: return "else";
		case WHILE: return "while";
		case DO: return "do";
		case OF: return "of";
		case CASE_DOTS: return "..";
		case OTHERWISE: return "otherwise";
		case REPEAT: return "repeat";
		case FOR: return "for";
		case UNTIL: return "until";
		case LOOP: return "loop";
		case LOOP_END: return "pool";
		case EXIT: return "exit";
		case LESS_OR_EQUAL: return "<=";
		case NOT_EQUAL: return "<>";
		case LESS_THAN: return "<";
		case GREATER_OR_EQUAL: return ">=";
		case GREATER_THAN: return ">";
		case EQUAL: return "=";
		case MOD: return "%";
		case AND: return "and";
		case OR: return "or";
		case NOT: return "not";
		case READ: return "read";
		case SUCC: return "succ";
		case PRED: return "pred";
		case CHR: return "chr";
		case ORD: return "ord";
		case EOF_ID: return "eof";
		case COMMENT_START: return "{";
		case COMMENT_END: return "}";
		case COLON: return ":";
		case SEMICOLON: return ";";
		case DOT: return ".";
		case COMMA: return ",";
		case PARENTHESIS_START: return "(";
		case PARENTHESIS_END: return ")";
		case PLUS: return "+";
		case MINUS: return "-";
		case MULTIPLY: return "*";
		case DIVIDE: return "/";
		case UNKNOWN: return "UNKNOWN";
		case INVALID: return "INVALID";
		default: return "ERROR";
		}
	}
};