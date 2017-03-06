#include "analyzer.h"
#include "parser.h"
#include "main.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Not enough arguments." << std::endl;
	    return -1;
	}

	recursiveDescent(argv[1]);

	return 0;
}	

void recursiveDescent(char* filename) {
	Lexer lexer = Lexer(filename);
	Parser parser = Parser(&lexer);
	parser.Tiny();
}

void parseAll(char* filename) {
	Lexer lexer = Lexer(filename);

	Token currentToken;
	while (lexer.peekInput() != EOF) {
		currentToken = lexer.getNextToken();
		std::cout << "token: " << currentToken.getText() << " type: " << currentToken.getTypeAsString() << std::endl;
	}
}