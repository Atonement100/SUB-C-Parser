#include "analyzer.h"
#include "parser.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Not enough arguments." << std::endl;
		return -1;
	}

	Lexer lexer = Lexer(argv[1]);

	Token currentToken;
	while (lexer.peekInput() != EOF) {
		currentToken = lexer.getToken();
		std::cout << "token: " << currentToken.getText() << " type: " << currentToken.getTypeAsString() << std::endl;
	}

	std::cout << Token::convertTokenTypeToString(TokenType::WHITESPACE);

	return 0;
}	