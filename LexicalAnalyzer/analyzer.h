#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include "token.h"

class Lexer {
private:
	std::ifstream inFile;
	std::string tokenStr;
	TokenType nextTokenType;
	char nextch;

protected:
	bool isWhiteSpace(char ch);

	int completeWhiteSpaceToken();
	int completeAlphaToken();
	int completeNumericToken();
	int completeCommentToken();
	int completeStringToken();
	int completeCharToken();
	int completeColonToken();
	int completeGreaterThanToken();
	int completeLessThanToken();
	int completeDotToken();

public:
	Lexer(char* filename);

	Token getToken();
	int peekInput() { return (char)(inFile.peek()); }
	TokenType identifyAlphaToken(std::string token);

	const std::map< std::string, TokenType > mapAlphaToTokenType = {
		{ "program",	TokenType::PROGRAM },
		{ "var",		TokenType::VAR },
		{ "const",		TokenType::CONST },
		{ "type",		TokenType::TYPE },
		{ "function",	TokenType::FUNCTION },
		{ "return",		TokenType::RETURN },
		{ "begin",		TokenType::BEGIN },
		{ "end",		TokenType::END },
		{ "output",		TokenType::OUTPUT },
		{ "if",			TokenType::IF },
		{ "then",		TokenType::THEN },
		{ "else",		TokenType::ELSE },
		{ "while",		TokenType::WHILE },
		{ "do",			TokenType::DO },
		{ "case",		TokenType::CASE },
		{ "of",			TokenType::OF },
		{ "otherwise",	TokenType::OTHERWISE },
		{ "repeat",		TokenType::REPEAT },
		{ "for",		TokenType::FOR },
		{ "until",		TokenType::UNTIL },
		{ "loop",		TokenType::LOOP },
		{ "pool",		TokenType::LOOP_END },
		{ "exit",		TokenType::EXIT },
		{ "mod",		TokenType::MOD },
		{ "and",		TokenType::AND },
		{ "or",			TokenType::OR },
		{ "not",		TokenType::NOT },
		{ "read",		TokenType::READ },
		{ "succ",		TokenType::SUCC },
		{ "pred",		TokenType::PRED },
		{ "chr",		TokenType::CHR },
		{ "ord",		TokenType::ORD },
		{ "eof",		TokenType::EOF_ID }
	};
};

int main(int argc, char* argv[]);
