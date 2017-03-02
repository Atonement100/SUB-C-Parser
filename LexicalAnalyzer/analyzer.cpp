#include "analyzer.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Not enough arguments." << std::endl;
		return -1;
	}

	Lexer lexer = Lexer(argv[1]);

	while (lexer.peekIfstream() != EOF) {
		std::cout << "token: " << lexer.getToken() << std::endl;
	}

	return 0;
}

Lexer::Lexer() {

}

Lexer::Lexer(char* filename) {
	this->inFile.open(filename, std::ifstream::in);


	if (!this->inFile.is_open()) {
		std::cerr << "Unable to open file " << filename << std::endl;
		exit(-2);
	}
}

std::string Lexer::getToken(std::ifstream& inFile) {
	std::string token = "";
	char nextch;
	inFile.get(nextch);
	token += nextch;

	if (isWhiteSpace(nextch)) completeWhiteSpaceToken(inFile, token, nextch);
	else if (isalpha(nextch) || nextch == '_') completeIdentifierToken(inFile, token, nextch);
	else if (isdigit(nextch)) completeNumericToken(inFile, token, nextch);
	else {
		switch (nextch) {
		case '\n':
		case '\r':
			token = "NEWLINE";
			break;
		case '\'': //Char
			//completeCharToken();
			break;
		case '"':  //String
			//completeStringToken();
			break;
		case '{':  //Comment
			//completeCommentToken();
			break;
		default:
			/*Either error or as-of-yet undefined behavior here*/
			token = std::to_string((int)(nextch));
			break;
		}
	}

	return token;
}

int Lexer::completeWhiteSpaceToken(std::ifstream& inFile, std::string& token, char& nextch) {
	while (isWhiteSpace((char)(inFile.peek()))) {
		inFile.get(nextch);
		token += nextch;
	}
	return token.length();
}

int Lexer::completeIdentifierToken(std::ifstream& inFile, std::string& token, char& nextch) {
	nextch = (char)(inFile.peek());
	while (isalnum(nextch) || nextch == '_') {
		inFile.get(nextch);
		token += nextch;
		nextch = (char)(inFile.peek());
	}
	return token.length();
}

int Lexer::completeNumericToken(std::ifstream& inFile, std::string& token, char& nextch) {
	nextch = (char)(inFile.peek());
	while (isdigit(nextch)) {
		inFile.get(nextch);
		token += nextch;
		nextch = (char)(inFile.peek());
	}
	return token.length();
}

std::string Lexer::getToken() {
	token = "";
	inFile.get(nextch);
	token += nextch;

	if (isWhiteSpace(nextch)) completeWhiteSpaceToken();
	else if (isalpha(nextch) || nextch == '_') completeIdentifierToken();
	else if (isdigit(nextch)) completeNumericToken();
	else {
		switch (nextch) {
		case '\n':
		case '\r':
			token = "NEWLINE";
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
		default:
			/*Either error or as-of-yet undefined behavior here*/
			token = std::to_string((int)(nextch)) + " = " + nextch + " << INVALID TOKEN ";
			break;
		}
	}

	return token;
}

int Lexer::completeWhiteSpaceToken() {
	while (isWhiteSpace((char)(inFile.peek()))) {
		inFile.get(nextch);
		token += nextch;
	}
	return token.length();
}

int Lexer::completeIdentifierToken() {
	nextch = (char)(inFile.peek());
	while (isalnum(nextch) || nextch == '_') {
		inFile.get(nextch);
		token += nextch;
		nextch = (char)(inFile.peek());
	}
	return token.length();
}

int Lexer::completeNumericToken() {
	nextch = (char)(inFile.peek());
	while (isdigit(nextch)) {
		inFile.get(nextch);
		token += nextch;
		nextch = (char)(inFile.peek());
	}
	return token.length();
}

int Lexer::completeCommentToken() {
	inFile.get(nextch);
	while (nextch != '}' && inFile.peek() != EOF) {
		token += nextch;
		inFile.get(nextch);
	}

	if (nextch == '}') {
		token += nextch;
	}
	else {
		token += "UNMATCHED BRACE";//Error
	}

	return token.length();
}

int Lexer::completeStringToken() {
	inFile.get(nextch);
	while (nextch != '"' && inFile.peek() != EOF) {
		token += nextch;
		inFile.get(nextch);
	}

	if (nextch == '"') {
		token += nextch;
	}
	else {
		token += "UNMATCHED QUOTE";//Error
	}

	return token.length();
}

int Lexer::completeCharToken() {
	char nextchs[2];
	inFile.get(nextchs,3);
	
	std::cout << nextch << nextchs[0] << nextchs[1];

	if (nextchs[0] != '\'' && nextchs[1] == '\'') {
		token += std::string(nextchs);
	}
	else {
		token += "INCORRECT CHAR FORMAT";//Error
	}

	return token.length();
}

bool Lexer::isWhiteSpace(const char ch) {
	return strchr("\t\v\f ", ch);
}