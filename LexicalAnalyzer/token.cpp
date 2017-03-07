#include "token.h"

/*Token::Token() {
	this->text = "";
	this->type = TokenType::UNKNOWN;
}*/

Token::Token(std::string text, TokenType type) {
	this->text = text;
	this->type = type;
}