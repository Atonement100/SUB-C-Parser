#pragma once 
#include "analyzer.h"
#include "tree.h"
#include <iostream>
#include <vector>
#include <stack>

class Parser {
protected:
	Token currentToken = Token("invalid", TokenType::INVALID);
	Lexer *lexer;
	std::vector< LCRSTree > treeStack;
	//std::stack< LCRSTree, std::vector<LCRSTree> > treeStack;
	std::vector<TokenType> buildableTokens = {TokenType::IDENTIFIER, TokenType::INTEGER, TokenType::CHAR, TokenType::STRING};

public:
	Parser(Lexer *lexer) {
		this->lexer = lexer;
	};

	void BuildTree(Token rootToken, int numTreesToPop);

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
	int Caseclauses();
	void Caseclause();
	void CaseExpression();
	int OtherwiseClause();
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
	void PrintStack();
	void PrintTree();
};