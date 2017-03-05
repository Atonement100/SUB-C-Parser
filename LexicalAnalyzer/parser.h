#pragma once 
#include "analyzer.h"
#include <iostream>
#include <vector>

class Parser {
protected:
	Token currentToken;
	Lexer *lexer;
public:
	Parser(Lexer *lexer) {
		this->lexer = lexer;
	};

	void ConsumeToken(TokenType expected);
	void Tiny();
	void Consts();
	void Const();
	void ConstValue();
	void Types();
	void Type();
	void LitList();
	void SubProgs();
	void Fcn();
	void Params();
	void Dclns();
	void Dcln();
	void Body();
	void Statement();
	void OutExp();
	void StringNode();
	void Caseclauses();
	void Caseclause();
	void CaseExpression();
	void OtherwiseClause();
	void Assignment();
	void ForStat();
	void ForExp();
	void Expression();
	void Term();
	void Factor();
	void Primary();
	void Name();

	void PrintParseError(std::vector<TokenType> expected, TokenType received);
	void PrintParseError(TokenType expected, TokenType received);
};