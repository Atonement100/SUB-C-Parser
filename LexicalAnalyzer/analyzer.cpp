#include "analyzer.h"
#include <algorithm>

Lexer::Lexer(char* filename) {
	this->inFile.open(filename, std::ifstream::in);

	if (!this->inFile.is_open()) {
		std::cerr << "Unable to open file " << filename << std::endl;
		exit(-2);
	}

	currentToken = Token("invalid", TokenType::INVALID);
}

Token Lexer::getNextUsefulToken() {
	Token nextUseful;
	do {
		nextUseful = getNextToken();
	} while (std::find(uselessTokenTypes.begin(), uselessTokenTypes.end(), nextUseful.getType()) != uselessTokenTypes.end());
	return nextUseful;
}

Token Lexer::getNextToken() {
	tokenStr = "";

	if (!inFile.get(nextch)) {
		return Token("FILE_END", TokenType::TRUE_EOF);
	}

	tokenStr += nextch;

	if (isWhiteSpace(nextch)) completeWhiteSpaceToken();
	else if (isalpha(nextch) || nextch == '_') completeAlphaToken(); 
	else if (isdigit(nextch)) completeNumericToken();
	else {
		switch (nextch) {
		case '\r':
			if (peekInput() == '\n') inFile.get(nextch); //Consume CRLF
		case '\n':
			tokenStr = "NEWLINE";
			nextTokenType = TokenType::ENDLINE;
			break;

		case '\'': //Char
			completeCharToken();
			break;
		case '"':  //String
			completeStringToken();
			break;
		case '{':  //Comment
			completeCommentToken();
			break;
		case '#':
			completeLineCommentToken();
			break;
		case ':': //Swap, Assignment, or standalone colon
			completeColonToken();
			break;
		case '<': //<, <=, <>
			completeLessThanToken();
			break;
		case '>': //>, >=
			completeGreaterThanToken();
			break;
		case '=': //=
			nextTokenType = TokenType::EQUAL;
			break;
		case ';':
			nextTokenType = TokenType::SEMICOLON;
			break;
		case '.':
			completeDotToken();
			break;
		case ',':
			nextTokenType = TokenType::COMMA;
			break;
		case '(':
			nextTokenType = TokenType::PARENTHESIS_START;
			break;
		case ')':
			nextTokenType = TokenType::PARENTHESIS_END;
			break;
		case '+':
			nextTokenType = TokenType::PLUS;
			break;
		case '-':
			nextTokenType = TokenType::MINUS;
			break;
		case '*':
			nextTokenType = TokenType::MULTIPLY;
			break;
		case '/':
			nextTokenType = TokenType::DIVIDE;
			break;
		default:
			/*Either error or as-of-yet undefined behavior here*/
			tokenStr = std::to_string((int)(nextch)) + " = " + nextch + " << INVALID TOKEN ";
			nextTokenType = TokenType::INVALID;
			break;
		}
	}

	currentToken = Token(tokenStr, nextTokenType);
	//std::cout << std::endl << currentToken.getText() << std::endl;
	return currentToken;
}

int Lexer::completeColonToken() {
	if (peekInput() == '=') {
		inFile.get(nextch);
		tokenStr += nextch;
		if (peekInput() == ':') { // :=:
			inFile.get(nextch);
			tokenStr += nextch;
			nextTokenType = TokenType::SWAP;
		}
		else { // :=
			nextTokenType = TokenType::ASSIGNMENT;
		}
	}
	else { // :
		nextTokenType = TokenType::COLON;
	}

	return tokenStr.length();
}

int Lexer::completeDotToken() {
	if (peekInput() == '.') {
		inFile.get(nextch);
		tokenStr += nextch;
		nextTokenType = TokenType::CASE_DOTS;
	}
	else {
		nextTokenType = TokenType::DOT;
	}
	
	return tokenStr.length();
}

int Lexer::completeGreaterThanToken() {
	if (peekInput() == '=') { // >=
		inFile.get(nextch);
		tokenStr += nextch;
		nextTokenType = TokenType::GREATER_OR_EQUAL;
	}
	else { // >
		nextTokenType = TokenType::GREATER_THAN;
	}

	return tokenStr.length();
}

int Lexer::completeLessThanToken() {
	if (peekInput() == '=') {
		inFile.get(nextch);
		tokenStr += nextch;
		nextTokenType = TokenType::LESS_OR_EQUAL;
	}
	else if (peekInput() == '>') {
		inFile.get(nextch);
		tokenStr += nextch;
		nextTokenType = TokenType::NOT_EQUAL;
	}
	else {
		nextTokenType = TokenType::LESS_THAN;
	}

	return tokenStr.length();
}

int Lexer::completeWhiteSpaceToken() {
	while (isWhiteSpace(peekInput())) {
		inFile.get(nextch);
		tokenStr += nextch;
	}
	nextTokenType = TokenType::WHITESPACE;
	return tokenStr.length();
}

int Lexer::completeAlphaToken() {
	nextch = peekInput();
	while (isalnum(nextch) || nextch == '_') {
		inFile.get(nextch);
		tokenStr += nextch;
		nextch = peekInput();
	}

	nextTokenType = identifyAlphaToken(tokenStr);
	return tokenStr.length();
}

int Lexer::completeNumericToken() {
	nextch = peekInput();
	while (isdigit(nextch)) {
		inFile.get(nextch);
		tokenStr += nextch;
		nextch = peekInput();
	}

	nextTokenType = TokenType::INTEGER;
	return tokenStr.length();
}

int Lexer::completeCommentToken() {
	inFile.get(nextch);
	while (nextch != '}' && peekInput() != EOF) {
		tokenStr += nextch;
		inFile.get(nextch);
	}

	if (nextch == '}') {
		tokenStr += nextch;
	}
	else { //EOF
		tokenStr += "UNMATCHED BRACE";//Error
		nextTokenType = TokenType::INVALID;
	}

	nextTokenType = TokenType::COMMENT;
	return tokenStr.length();
}

int Lexer::completeLineCommentToken()
{
	inFile.get(nextch);
	while (nextch != '\n' && peekInput() != EOF) {
		tokenStr += nextch;
		inFile.get(nextch);
	}
	nextTokenType = TokenType::COMMENT;
	return tokenStr.length();
}

int Lexer::completeStringToken() {
	inFile.get(nextch);
	while (nextch != '"' && peekInput() != EOF) {
		tokenStr += nextch;
		inFile.get(nextch);
	}

	if (nextch == '"') {
		tokenStr += nextch;
	}
	else { //EOF
		tokenStr += "UNMATCHED QUOTE";//Error
		nextTokenType = TokenType::INVALID;
	}

	nextTokenType = TokenType::STRING;
	return tokenStr.length();
}

int Lexer::completeCharToken() {
	char nextchs[2];
	inFile.get(nextchs,3);
	
	std::cout << nextch << nextchs[0] << nextchs[1];

	if (nextchs[0] != '\'' && nextchs[1] == '\'') {
		tokenStr += std::string(nextchs);
	}
	else {
		tokenStr += "INCORRECT CHAR FORMAT";//Error
		nextTokenType = TokenType::INVALID;
	}

	nextTokenType = TokenType::CHAR;
	return tokenStr.length();
}

TokenType Lexer::identifyAlphaToken(std::string token) {
	auto mapIter = mapAlphaToTokenType.find(token);
	if (mapIter != mapAlphaToTokenType.end()) {
		return mapIter->second;
	}
	else {
		return TokenType::IDENTIFIER;
	}
}

bool Lexer::isWhiteSpace(const char ch) {
	return strchr("\t\v\f ", ch);
}