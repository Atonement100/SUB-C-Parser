#include "token.h"

Token::Token(std::string text, TokenType type) {
	this->text = text;
	this->type = type;
}