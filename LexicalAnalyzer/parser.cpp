#include "token.h"
#include "parser.h"

void Parser::ConsumeToken(TokenType expected) {
	if (currentToken.getType() != expected) {
		PrintParseError(expected, currentToken.getType());
		exit(-3);
	}
	std::cout << "Successfully read token: " << currentToken.getTypeAsString() << " : " << currentToken.getText() << std::endl;
	treeStack.push_back(LCRSTree(currentToken));
	currentToken = lexer->getNextUsefulToken();
}

void Parser::BuildTree(Token rootToken, int numTreesToPop) {
	if (numTreesToPop < 1) return;

	std::vector< LCRSTree > localStack;

	LCRSTree newRootTree = LCRSTree(rootToken);

	for (int numLeft = numTreesToPop; numLeft > 0; numLeft--) {
		localStack.push_back(treeStack.back());
		treeStack.pop_back();
	}

	BinaryTreeNode* recentSibling = newRootTree.getRoot()->AddChild(localStack.back().getRoot());
	localStack.pop_back();

	while (!localStack.empty()) {
		recentSibling = recentSibling->AddSibling(localStack.back().getRoot());
		localStack.pop_back();
	}

	treeStack.push_back(newRootTree);
}

void Parser::PrintStack() {
	while (!treeStack.empty()) {
		treeStack.back().getRoot()->PrintNode();
		treeStack.pop_back();
	}
}

void Parser::PrintTree() {
	treeStack.back().PrintTree();
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
	if (currentToken.getType() == TokenType::CONST) {
		ConsumeToken(TokenType::CONST);
		Const();
		while (currentToken.getType() == TokenType::COMMA) {
			ConsumeToken(TokenType::COMMA);
			Const();
		}
		ConsumeToken(TokenType::SEMICOLON);
	}
	else {
		//write consts -> empty
	}
}

void Parser::Const() {
	Name();
	ConsumeToken(TokenType::EQUAL);
	ConstValue();
}

void Parser::ConstValue() {
	switch (currentToken.getType()) {
	case TokenType::INTEGER:
		ConsumeToken(TokenType::INTEGER);
		break;
	case TokenType::CHAR:
		ConsumeToken(TokenType::CHAR);
		break;
	case TokenType::IDENTIFIER:
		Name();
		break;
	default:
		break;
	}
}

void Parser::Types() {
	if (currentToken.getType() == TokenType::TYPE) {
		ConsumeToken(TokenType::TYPE);
		do {
			Type();
			ConsumeToken(TokenType::SEMICOLON);
		} while (currentToken.getType() == TokenType::IDENTIFIER);
	}
	else {
		//write types -> empty
	}
}

void Parser::Type() {
Name();
ConsumeToken(TokenType::EQUAL);
LitList();
}

void Parser::LitList() {
	ConsumeToken(TokenType::PARENTHESIS_START);
	Name();
	while (currentToken.getType() == TokenType::COMMA) {
		ConsumeToken(TokenType::COMMA);
		Name();
	}
	ConsumeToken(TokenType::PARENTHESIS_END);
}

void Parser::SubProgs() {
	while (currentToken.getType() == TokenType::FUNCTION) {
		Fcn();
	}
}

void Parser::Fcn() {
	ConsumeToken(TokenType::FUNCTION);
	Name();
	ConsumeToken(TokenType::PARENTHESIS_START);
	Params();
	ConsumeToken(TokenType::PARENTHESIS_END);
	ConsumeToken(TokenType::COLON);
	Name();
	ConsumeToken(TokenType::SEMICOLON);
	Consts();
	Types();
	Dclns();
	Body();
	Name();
	ConsumeToken(TokenType::SEMICOLON);
}

void Parser::Params() {
	Dcln();
	while (currentToken.getType() == TokenType::SEMICOLON) {
		ConsumeToken(TokenType::SEMICOLON);
		Dcln();
	}
}

void Parser::Dclns() {
	if (currentToken.getType() == TokenType::VAR) {
		ConsumeToken(TokenType::VAR);
		do {
			Dcln();
			ConsumeToken(TokenType::SEMICOLON);
		} while (currentToken.getType() == TokenType::IDENTIFIER);
	}
	else {
		//write empty
	}

}

void Parser::Dcln() {
	Name();
	while (currentToken.getType() == TokenType::COMMA) {
		ConsumeToken(TokenType::COMMA);
		Name();
	}
	ConsumeToken(TokenType::COLON);
	Name();
}

void Parser::Body() {
	ConsumeToken(TokenType::BEGIN);
	Statement();
	while (currentToken.getType() == TokenType::SEMICOLON) {
		ConsumeToken(TokenType::SEMICOLON);
		Statement();
	}
	ConsumeToken(TokenType::END);
}

void Parser::Statement() {
	switch (currentToken.getType()) {
	case TokenType::IDENTIFIER:
		Assignment();
		break;
	case TokenType::OUTPUT:
		ConsumeToken(TokenType::OUTPUT);
		ConsumeToken(TokenType::PARENTHESIS_START);
		OutExp();
		while (currentToken.getType() == TokenType::COMMA) {
			ConsumeToken(TokenType::COMMA);
			OutExp();
		}
		ConsumeToken(TokenType::PARENTHESIS_END);
		break;
	case TokenType::IF:
		ConsumeToken(TokenType::IF);
		Expression();
		ConsumeToken(TokenType::THEN);
		Statement();
		if (currentToken.getType() == TokenType::ELSE){
			ConsumeToken(TokenType::ELSE);
			Statement();
		}
		break;
	case TokenType::WHILE:
		ConsumeToken(TokenType::WHILE);
		Expression();
		ConsumeToken(TokenType::DO);
		Statement();
		break;
	case TokenType::REPEAT:
		ConsumeToken(TokenType::REPEAT);
		Statement();
		while (currentToken.getType() == TokenType::SEMICOLON) {
			ConsumeToken(TokenType::SEMICOLON);
			Statement();
		}
		ConsumeToken(TokenType::UNTIL);
		Expression();
		break;
	case TokenType::FOR:
		ConsumeToken(TokenType::FOR);
		ConsumeToken(TokenType::PARENTHESIS_START);
		ForStat();
		ConsumeToken(TokenType::SEMICOLON);
		ForExp();
		ConsumeToken(TokenType::SEMICOLON);
		ForStat();
		ConsumeToken(TokenType::PARENTHESIS_END);
		Statement();
		break;
	case TokenType::LOOP:
		ConsumeToken(TokenType::LOOP);
		Statement();
		while (currentToken.getType() == TokenType::SEMICOLON) {
			ConsumeToken(TokenType::SEMICOLON);
			Statement();
		}
		ConsumeToken(TokenType::LOOP_END);
		break;
	case TokenType::CASE:
		ConsumeToken(TokenType::CASE);
		Expression();
		ConsumeToken(TokenType::OF);
		Caseclauses();
		OtherwiseClause();
		ConsumeToken(TokenType::END);
		break;
	case TokenType::READ:
		ConsumeToken(TokenType::READ);
		ConsumeToken(TokenType::PARENTHESIS_START);
		Name();
		while (currentToken.getType() == TokenType::COMMA) {
			ConsumeToken(TokenType::COMMA);
			Name();
		}
		ConsumeToken(TokenType::PARENTHESIS_END);
		break;
	case TokenType::EXIT:
		ConsumeToken(TokenType::EXIT);
		break;
	case TokenType::RETURN:
		ConsumeToken(TokenType::RETURN);
		Expression();
		break;
	case TokenType::BEGIN:
		Body();
		break;
	default:
		//write empty
		break;
	}
}

void Parser::OutExp() {
	if (currentToken.getType() == TokenType::STRING) {
		StringNode();
	}
	else {
		Expression();
	}
}

void Parser::StringNode() {
	ConsumeToken(TokenType::STRING);
	ConsumeToken(TokenType::SEMICOLON);
}

void Parser::Caseclauses() {
	TokenType nextType;
	do {
		Caseclause();
		ConsumeToken(TokenType::SEMICOLON);
		nextType = currentToken.getType();
	} while (nextType == TokenType::INTEGER || nextType == TokenType::CHAR || nextType == TokenType::IDENTIFIER);
}

void Parser::Caseclause() {
	CaseExpression();
	while (currentToken.getType() == TokenType::COMMA) {
		ConsumeToken(TokenType::COMMA);
		CaseExpression();
	}
	ConsumeToken(TokenType::COLON);
	Statement();
}

void Parser::CaseExpression() {
	ConstValue();
	if (currentToken.getType() == TokenType::CASE_DOTS) {
		ConsumeToken(TokenType::CASE_DOTS);
		ConstValue();
	}
}

void Parser::OtherwiseClause() {
	switch (currentToken.getType()) {
	case TokenType::OTHERWISE:
		ConsumeToken(TokenType::OTHERWISE);
		Statement();
		break;
	default:
		break;
	}
}

void Parser::Assignment() {
	Name();
	switch (currentToken.getType()) {
	case TokenType::ASSIGNMENT:
		ConsumeToken(TokenType::ASSIGNMENT);
		Expression();
		break;
	case TokenType::SWAP:
		ConsumeToken(TokenType::SWAP);
		Name();
		break;
	default:
		break;
	}
}

void Parser::ForStat() {
	if (currentToken.getType() == TokenType::IDENTIFIER) {
		Assignment();
	}
	else {
		//write null
	}
}

void Parser::ForExp() {
	if (currentToken.getType() == TokenType::SEMICOLON) {
		//write null
	}
	else {
		Expression();
	}
}

//Do I really need the switch statement here?
void Parser::Expression() {
	Term();
	switch (currentToken.getType()) {
	case TokenType::LESS_OR_EQUAL:
		ConsumeToken(TokenType::LESS_OR_EQUAL);
		Term();
		break;
	case TokenType::LESS_THAN:
		ConsumeToken(TokenType::LESS_THAN);
		Term();
		break;
	case TokenType::GREATER_OR_EQUAL:
		ConsumeToken(TokenType::GREATER_OR_EQUAL);
		Term();
		break;
	case TokenType::GREATER_THAN:
		ConsumeToken(TokenType::GREATER_THAN);
		Term();
		break;
	case TokenType::EQUAL:
		ConsumeToken(TokenType::EQUAL);
		Term();
		break;
	case TokenType::NOT_EQUAL:
		ConsumeToken(TokenType::NOT_EQUAL);
		Term();
		break;
	default:
		break;
	}
}

void Parser::Term() {
	Factor();

	bool invalidTokenFound = false;
	while (!invalidTokenFound) {
		switch (currentToken.getType()) {
		case TokenType::PLUS:
			ConsumeToken(TokenType::PLUS);
			Factor();
			break;
		case TokenType::MINUS:
			ConsumeToken(TokenType::MINUS);
			Factor();
			break;
		case TokenType::OR:
			ConsumeToken(TokenType::OR);
			Factor();
			break;
		default:
			invalidTokenFound = true;
		}
	}
}

void Parser::Factor() {
	Primary();

	bool invalidTokenFound = false;
	while (!invalidTokenFound) {
		switch (currentToken.getType()) {
		case TokenType::MULTIPLY:
			ConsumeToken(TokenType::MULTIPLY);
			Primary();
			break;
		case TokenType::DIVIDE:
			ConsumeToken(TokenType::DIVIDE);
			Primary();
			break;
		case TokenType::AND:
			ConsumeToken(TokenType::AND);
			Primary();
			break;
		case TokenType::MOD:
			ConsumeToken(TokenType::MOD);
			Primary();
			break;
		default:
			invalidTokenFound = true;
		}
	}
}

void Parser::Primary() {
	switch (currentToken.getType()) {
	case TokenType::MINUS:
		ConsumeToken(TokenType::MINUS);
		Primary();
		break;
	case TokenType::PLUS:
		ConsumeToken(TokenType::PLUS);
		Primary();
		break;
	case TokenType::NOT:
		ConsumeToken(TokenType::NOT);
		Primary();
		break;
	case TokenType::EOF_ID:
		ConsumeToken(TokenType::EOF_ID);
		break;
	case TokenType::INTEGER:
		ConsumeToken(TokenType::INTEGER);
		break;
	case TokenType::CHAR:
		ConsumeToken(TokenType::CHAR);
		break;
	case TokenType::IDENTIFIER:
		Name();
		if (currentToken.getType() == TokenType::PARENTHESIS_START) {
			ConsumeToken(TokenType::PARENTHESIS_START);
			Expression();
			while (currentToken.getType() == TokenType::COMMA) {
				ConsumeToken(TokenType::COMMA);
				Expression();
			}
			ConsumeToken(TokenType::PARENTHESIS_END);
		}
		break;
	case TokenType::PARENTHESIS_START:
		ConsumeToken(TokenType::PARENTHESIS_START);
		Expression();
		ConsumeToken(TokenType::PARENTHESIS_END);
		break;
	case TokenType::SUCC:
		ConsumeToken(TokenType::SUCC);
		ConsumeToken(TokenType::PARENTHESIS_START);
		Expression();
		ConsumeToken(TokenType::PARENTHESIS_END);
		break;
	case TokenType::PRED:
		ConsumeToken(TokenType::PRED);
		ConsumeToken(TokenType::PARENTHESIS_START);
		Expression();
		ConsumeToken(TokenType::PARENTHESIS_END);
		break;
	case TokenType::CHR:
		ConsumeToken(TokenType::CHR);
		ConsumeToken(TokenType::PARENTHESIS_START);
		Expression();
		ConsumeToken(TokenType::PARENTHESIS_END);
		break;
	case TokenType::ORD:
		ConsumeToken(TokenType::ORD);
		ConsumeToken(TokenType::PARENTHESIS_START);
		Expression();
		ConsumeToken(TokenType::PARENTHESIS_END);
		break;
	default:
		break;
	}
}

void Parser::Name() {
	ConsumeToken(TokenType::IDENTIFIER);

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
