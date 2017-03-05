#include "token.h"
#include "parser.h"

void Parser::ConsumeToken(TokenType expected) {
	if (currentToken.getType() != expected) {
		PrintParseError(expected, currentToken.getType());
		exit(-3);
	}
	std::cout << "Successfully read token: " << currentToken.getTypeAsString() << std::endl;
	currentToken = lexer->getNextUsefulToken();
}


void Parser::Tiny() {
	currentToken = lexer->getNextUsefulToken(); //Initialize descent
	switch (currentToken.getType()) {
		case TokenType::PROGRAM:
			ConsumeToken(TokenType::PROGRAM); //Read program
			Name();
			ConsumeToken(TokenType::COLON); //Read :
			Consts();
			Types();
			Dclns();
			SubProgs();
			Body();
			Name();
			ConsumeToken(TokenType::DOT); //Read .
			//Write program
			break;
		default:
			//ok so its possible to be expecting several types of token types at a given point, fix this tomorrow
			PrintParseError(TokenType::PROGRAM, currentToken.getType());
			//print parse error (expected, received)
			break;
	}
}

void Parser::Consts() {
}

void Parser::ConstValue() {
}

void Parser::Types() {
}

void Parser::Type() {
}

void Parser::LitList() {
}

void Parser::SubProgs() {
}

void Parser::Fcn() {
}

void Parser::Params() {
}

void Parser::Dclns() {
}

void Parser::Dcln() {
}

void Parser::Body() {
}

void Parser::Statement() {
}

void Parser::OutExp() {
}

void Parser::StringNode() {
}

void Parser::Caseclauses() {
}

void Parser::Caseclause() {
}

void Parser::CaseExpression() {
}

void Parser::OtherwiseClause() {
}

void Parser::Assignment() {
}

void Parser::ForStat() {
}

void Parser::ForExp() {
}

void Parser::Expression() {
}

void Parser::Term() {
}

void Parser::Factor() {
}

void Parser::Primary() {
}

void Parser::Name() {
}

void Parser::PrintParseError(std::vector<TokenType> expected, TokenType received) {
	std::cout << "Error when attempting to parse for token. Received " << Token::convertTokenTypeToString(received);
	
	if (expected.size() == 1) {
		std::cout << " but expected " << Token::convertTokenTypeToString(expected[0]) << "." << std::endl;
	}
	else {
		std::cout << " but expected one of: ";
		for (auto iter = expected.begin(); iter != expected.end(); iter++) {
			std::cout << Token::convertTokenTypeToString(*iter);
			if (!(iter + 1 == expected.end())) {
				std::cout << ", ";
			}
			
		}
		std::cout << "." << std::endl;
	}
}

void Parser::PrintParseError(TokenType expected, TokenType received){
	std::cout << "Error when attempting to parse for token. Received " << Token::convertTokenTypeToString(received) << " but expected " << Token::convertTokenTypeToString(expected) << "." << std::endl;;
}
