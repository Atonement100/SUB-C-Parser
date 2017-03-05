#include "analyzer.h"
#include "parser.h"
#include "main.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Not enough arguments." << std::endl;
		return -1;
	}

	parseAll(argv[1]);

	return 0;
}	

void parseAll(char* filename) {
	Lexer lexer = Lexer(filename);

	Token currentToken;
	while (lexer.peekInput() != EOF) {
		currentToken = lexer.getToken();
		std::cout << "token: " << currentToken.getText() << " type: " << currentToken.getTypeAsString() << std::endl;
	}
}