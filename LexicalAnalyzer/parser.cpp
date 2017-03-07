#include "token.h"
#include "parser.h"
#include <algorithm>

void Parser::ConsumeToken(TokenType expected) {
	if (currentToken.getType() != expected) {
		PrintParseError(expected, currentToken.getType());
		exit(-3);
	}
	//std::cout << "Successfully read token: " << currentToken.getTypeAsString() << " : " << currentToken.getText() << std::endl;
	//treeStack.push_back(LCRSTree(currentToken));
	if (std::find(buildableTokens.begin(), buildableTokens.end(), currentToken.getType()) != buildableTokens.end()) {
		BuildTree(currentToken, 0);
	}

	currentToken = lexer->getNextUsefulToken();
}

void Parser::BuildTree(Token rootToken, int numTreesToPop) {
	LCRSTree newRootTree = LCRSTree(rootToken);

	if (numTreesToPop > 0) {
		std::vector< LCRSTree > localStack;
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
		BuildTree(Token("program", TokenType::PROGRAM), 7);
		break;
	default:
		break;
	}
}

void Parser::Consts() {
	if (currentToken.getType() == TokenType::CONST) {
		ConsumeToken(TokenType::CONST);
		Const();
		
		int numConsts = 1;
		while (currentToken.getType() == TokenType::COMMA) {
			ConsumeToken(TokenType::COMMA);
			Const();
			numConsts++;
		}

		ConsumeToken(TokenType::SEMICOLON);
		BuildTree(Token("consts", TokenType::CONSTS), numConsts);
	}
	else {
		//write consts -> empty
		BuildTree(Token("consts", TokenType::CONSTS), 0);
	}
}

void Parser::Const() {
	Name();
	ConsumeToken(TokenType::EQUAL);
	ConstValue();
	BuildTree(Token("const", TokenType::CONST), 2);
}

void Parser::ConstValue() {
	switch (currentToken.getType()) {
	case TokenType::INTEGER:
		ConsumeToken(TokenType::INTEGER);
		BuildTree(Token("<integer>", TokenType::INTEGER), 1);
		break;
	case TokenType::CHAR:
		ConsumeToken(TokenType::CHAR);
		BuildTree(Token("<char>", TokenType::CHAR), 1);
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

		int numTypes = 0;
		do {
			Type();
			ConsumeToken(TokenType::SEMICOLON);
			numTypes++;
		} while (currentToken.getType() == TokenType::IDENTIFIER);
		BuildTree(Token("types", TokenType::TYPES), numTypes);
	}
	else {
		//write types -> empty
		BuildTree(Token("types", TokenType::TYPES), 0);
	}
}

void Parser::Type() {
	Name();
	ConsumeToken(TokenType::EQUAL);
	LitList();
	BuildTree(Token("type", TokenType::TYPE), 2);
}

void Parser::LitList() {
	ConsumeToken(TokenType::PARENTHESIS_START);
	Name();

	int numNames = 1;
	while (currentToken.getType() == TokenType::COMMA) {
		ConsumeToken(TokenType::COMMA);
		Name();
		numNames++;
	}
	ConsumeToken(TokenType::PARENTHESIS_END);

	BuildTree(Token("lit", TokenType::LITLIST), numNames);
}

void Parser::SubProgs() {
	int numFncs = 0;
	while (currentToken.getType() == TokenType::FUNCTION) {
		Fcn();
		numFncs++;
	}
	BuildTree(Token("subprogs", TokenType::SUBPROGS), numFncs);
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
	BuildTree(Token("fcn", TokenType::FUNCTION), 8);
}

void Parser::Params() {
	Dcln();

	int numDclns = 1;
	while (currentToken.getType() == TokenType::SEMICOLON) {
		ConsumeToken(TokenType::SEMICOLON);
		Dcln();
		numDclns++;
	}
	BuildTree(Token("params", TokenType::PARAMS), numDclns);
}

void Parser::Dclns() {
	if (currentToken.getType() == TokenType::VAR) {
		ConsumeToken(TokenType::VAR);
		int numDclns = 0;
		do {
			Dcln();
			ConsumeToken(TokenType::SEMICOLON);
			numDclns++;
		} while (currentToken.getType() == TokenType::IDENTIFIER);
		BuildTree(Token("dclns", TokenType::DCLNS), numDclns);
	}
	else {
		//write empty
		BuildTree(Token("dclns", TokenType::DCLNS), 0);
	}

}

void Parser::Dcln() {
	Name();
	int numNames = 2;
	while (currentToken.getType() == TokenType::COMMA) {
		ConsumeToken(TokenType::COMMA);
		Name();
		numNames++;
	}
	ConsumeToken(TokenType::COLON);
	Name();
	BuildTree(Token("var", TokenType::VAR), numNames);
}

void Parser::Body() {
	ConsumeToken(TokenType::BEGIN);
	Statement();
	int numStatements = 1;
	while (currentToken.getType() == TokenType::SEMICOLON) {
		ConsumeToken(TokenType::SEMICOLON);
		Statement();
		numStatements++;
	}
	ConsumeToken(TokenType::END);
	BuildTree(Token("block", TokenType::BLOCK), numStatements);
}

void Parser::Statement() {
	switch (currentToken.getType()) {
	case TokenType::IDENTIFIER:
		Assignment();
		break;
	case TokenType::OUTPUT: {
		ConsumeToken(TokenType::OUTPUT);
		ConsumeToken(TokenType::PARENTHESIS_START);
		OutExp();
		int numOutExps = 1;
		while (currentToken.getType() == TokenType::COMMA) {
			ConsumeToken(TokenType::COMMA);
			OutExp();
			numOutExps++;
		}
		ConsumeToken(TokenType::PARENTHESIS_END);
		BuildTree(Token("output", TokenType::OUTPUT), numOutExps);
		break;
	}
	case TokenType::IF:
		ConsumeToken(TokenType::IF);
		Expression();
		ConsumeToken(TokenType::THEN);
		Statement();
		if (currentToken.getType() == TokenType::ELSE){
			ConsumeToken(TokenType::ELSE);
			Statement();
			BuildTree(Token("if", TokenType::IF), 3);
		}
		else {
			BuildTree(Token("if", TokenType::IF), 2);
		}
		break;
	case TokenType::WHILE:
		ConsumeToken(TokenType::WHILE);
		Expression();
		ConsumeToken(TokenType::DO);
		Statement();
		BuildTree(Token("while", TokenType::WHILE), 2);
		break;
	case TokenType::REPEAT: {
		ConsumeToken(TokenType::REPEAT);
		Statement();
		int numStatements = 1;
		while (currentToken.getType() == TokenType::SEMICOLON) {
			ConsumeToken(TokenType::SEMICOLON);
			Statement();
			numStatements++;
		}
		ConsumeToken(TokenType::UNTIL);
		Expression();
		BuildTree(Token("repeat", TokenType::REPEAT), numStatements + 1); //+1 for expression
		break;
	}
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
		BuildTree(Token("for", TokenType::FOR), 4);
		break;
	case TokenType::LOOP: {
		ConsumeToken(TokenType::LOOP);
		Statement();
		int numStatements = 1;
		while (currentToken.getType() == TokenType::SEMICOLON) {
			ConsumeToken(TokenType::SEMICOLON);
			Statement();
			numStatements++;
		}
		ConsumeToken(TokenType::LOOP_END);
		BuildTree(Token("loop", TokenType::LOOP), numStatements);
		break;
	}
	case TokenType::CASE: {
		int numTreesToPop = 1;
		ConsumeToken(TokenType::CASE);
		Expression();
		ConsumeToken(TokenType::OF);
		numTreesToPop += Caseclauses();
		numTreesToPop += OtherwiseClause();
		ConsumeToken(TokenType::END);
		BuildTree(Token("case", TokenType::CASE), numTreesToPop);
		break;
	}
	case TokenType::READ: {
		ConsumeToken(TokenType::READ);
		ConsumeToken(TokenType::PARENTHESIS_START);
		Name();
		int numNames = 1;
		while (currentToken.getType() == TokenType::COMMA) {
			ConsumeToken(TokenType::COMMA);
			Name();
			numNames++;
		}
		ConsumeToken(TokenType::PARENTHESIS_END);
		BuildTree(Token("read", TokenType::READ), numNames);
		break;
	}
	case TokenType::EXIT:
		ConsumeToken(TokenType::EXIT);
		BuildTree(Token("exit", TokenType::EXIT), 0);
		break;
	case TokenType::RETURN:
		ConsumeToken(TokenType::RETURN);
		Expression();
		BuildTree(Token("return", TokenType::RETURN), 1);
		break;
	case TokenType::BEGIN:
		Body();
		break;
	default:
		BuildTree(Token("<null>", TokenType::NULL_ID), 0);
		break;
	}
}

void Parser::OutExp() {
	if (currentToken.getType() == TokenType::STRING) {
		StringNode();
		BuildTree(Token("string", TokenType::STRING_NOBRACE), 2);
	}
	else {
		Expression();
		BuildTree(Token("integer", TokenType::INTEGER_NOBRACE), 1);
	}
}

void Parser::StringNode() {
	ConsumeToken(TokenType::STRING);
	ConsumeToken(TokenType::SEMICOLON);
	BuildTree(Token("<string>", TokenType::STRING), 1);
}

int Parser::Caseclauses() {
	TokenType nextType;
	int numCaseclauses = 0;
	do {
		Caseclause();
		ConsumeToken(TokenType::SEMICOLON);
		nextType = currentToken.getType();
		numCaseclauses++;
	} while (nextType == TokenType::INTEGER || nextType == TokenType::CHAR || nextType == TokenType::IDENTIFIER);
	return numCaseclauses;
}

void Parser::Caseclause() {
	CaseExpression();
	int numCaseExpressions = 1;
	while (currentToken.getType() == TokenType::COMMA) {
		ConsumeToken(TokenType::COMMA);
		CaseExpression();
		numCaseExpressions++;
	}
	ConsumeToken(TokenType::COLON);
	Statement();
	BuildTree(Token("case_clause", TokenType::CASE_CLAUSE), numCaseExpressions+1); //+1 statement
}

void Parser::CaseExpression() {
	ConstValue();
	if (currentToken.getType() == TokenType::CASE_DOTS) {
		ConsumeToken(TokenType::CASE_DOTS);
		ConstValue();
		BuildTree(Token("..", TokenType::CASE_DOTS), 2);
	}
}

int Parser::OtherwiseClause() {
	switch (currentToken.getType()) {
	case TokenType::OTHERWISE:
		ConsumeToken(TokenType::OTHERWISE);
		Statement();
		BuildTree(Token("otherwise", TokenType::OTHERWISE), 1);
		return 1;
	default:
		return 0;
	}
}

void Parser::Assignment() {
	Name();
	switch (currentToken.getType()) {
	case TokenType::ASSIGNMENT:
		ConsumeToken(TokenType::ASSIGNMENT);
		Expression();
		BuildTree(Token("assign", TokenType::ASSIGNMENT), 2);
		break;
	case TokenType::SWAP:
		ConsumeToken(TokenType::SWAP);
		Name();
		BuildTree(Token("swap", TokenType::SWAP), 2);
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
		BuildTree(Token("<null>", TokenType::NULL_ID), 0);
	}
}

void Parser::ForExp() {
	if (currentToken.getType() == TokenType::SEMICOLON) {
		//write null
		BuildTree(Token("true", TokenType::BOOL_TRUE), 2);
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
		BuildTree(Token("<=", TokenType::LESS_OR_EQUAL), 2);
		break;
	case TokenType::LESS_THAN:
		ConsumeToken(TokenType::LESS_THAN);
		Term();
		BuildTree(Token("<", TokenType::LESS_THAN), 2);
		break;
	case TokenType::GREATER_OR_EQUAL:
		ConsumeToken(TokenType::GREATER_OR_EQUAL);
		Term();
		BuildTree(Token(">=", TokenType::GREATER_OR_EQUAL), 2);
		break;
	case TokenType::GREATER_THAN:
		ConsumeToken(TokenType::GREATER_THAN);
		Term();
		BuildTree(Token(">", TokenType::GREATER_THAN), 2);
		break;
	case TokenType::EQUAL:
		ConsumeToken(TokenType::EQUAL);
		Term();
		BuildTree(Token("=", TokenType::EQUAL), 2);
		break;
	case TokenType::NOT_EQUAL:
		ConsumeToken(TokenType::NOT_EQUAL);
		Term();
		BuildTree(Token("<>", TokenType::NOT_EQUAL), 2);
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
			BuildTree(Token("+", TokenType::PLUS), 2);
			break;
		case TokenType::MINUS:
			ConsumeToken(TokenType::MINUS);
			Factor();
			BuildTree(Token("-", TokenType::MINUS), 2);
			break;
		case TokenType::OR:
			ConsumeToken(TokenType::OR);
			Factor();
			BuildTree(Token("or", TokenType::OR), 2);
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
			BuildTree(Token("*", TokenType::MULTIPLY), 2);
			break;
		case TokenType::DIVIDE:
			ConsumeToken(TokenType::DIVIDE);
			Primary();
			BuildTree(Token("/", TokenType::DIVIDE), 2);
			break;
		case TokenType::AND:
			ConsumeToken(TokenType::AND);
			Primary();
			BuildTree(Token("and", TokenType::AND), 2);
			break;
		case TokenType::MOD:
			ConsumeToken(TokenType::MOD);
			Primary();
			BuildTree(Token("mod", TokenType::MOD), 2);
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
		BuildTree(Token("-", TokenType::MINUS), 1);
		break;
	case TokenType::PLUS:
		ConsumeToken(TokenType::PLUS);
		Primary();
		break;
	case TokenType::NOT:
		ConsumeToken(TokenType::NOT);
		Primary();
		BuildTree(Token("not", TokenType::NOT), 1);
		break;
	case TokenType::EOF_ID:
		ConsumeToken(TokenType::EOF_ID);
		BuildTree(Token("eof", TokenType::EOF_ID), 0);
		break;
	case TokenType::INTEGER:
		ConsumeToken(TokenType::INTEGER);
		BuildTree(Token("<integer>", TokenType::INTEGER), 1);
		break;
	case TokenType::CHAR:
		ConsumeToken(TokenType::CHAR);
		BuildTree(Token("<char>", TokenType::CHAR), 1);
		break;
	case TokenType::IDENTIFIER:
		Name();
		if (currentToken.getType() == TokenType::PARENTHESIS_START) {
			ConsumeToken(TokenType::PARENTHESIS_START);
			Expression();
			int numExpressions = 1;
			while (currentToken.getType() == TokenType::COMMA) {
				ConsumeToken(TokenType::COMMA);
				Expression();
				numExpressions++;
			}
			ConsumeToken(TokenType::PARENTHESIS_END);
			BuildTree(Token("call", TokenType::CALL), numExpressions+1);
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
		BuildTree(Token("succ", TokenType::SUCC), 1);
		break;
	case TokenType::PRED:
		ConsumeToken(TokenType::PRED);
		ConsumeToken(TokenType::PARENTHESIS_START);
		Expression();
		ConsumeToken(TokenType::PARENTHESIS_END);
		BuildTree(Token("pred", TokenType::PRED), 1);
		break;
	case TokenType::CHR:
		ConsumeToken(TokenType::CHR);
		ConsumeToken(TokenType::PARENTHESIS_START);
		Expression();
		ConsumeToken(TokenType::PARENTHESIS_END);
		BuildTree(Token("chr", TokenType::CHR), 1);
		break;
	case TokenType::ORD:
		ConsumeToken(TokenType::ORD);
		ConsumeToken(TokenType::PARENTHESIS_START);
		Expression();
		ConsumeToken(TokenType::PARENTHESIS_END);
		BuildTree(Token("ord", TokenType::ORD), 1);
		break;
	default:
		break;
	}
}

void Parser::Name() {
	ConsumeToken(TokenType::IDENTIFIER);
	BuildTree(Token("<identifier>", TokenType::IDENTIFIER), 1);
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
