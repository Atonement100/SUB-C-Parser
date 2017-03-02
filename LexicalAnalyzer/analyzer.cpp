#include "analyzer.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Not enough arguments." << std::endl;
		return -1;
	}

	std::ifstream inFile;
	inFile.open(argv[1], std::ifstream::in);
	
	if (!inFile.is_open()) {
		std::cerr << "Unable to open file " << argv[1] << std::endl;
		return -2;
	}

	Lexer lexer = Lexer();

	while (inFile.peek() != EOF) {
		std::cout << "token: " << lexer.getToken(inFile) << std::endl;
	}

	return 0;
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


bool Lexer::isWhiteSpace(const char ch) {
	return strchr("\t\v\f ", ch);
}