#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

class Lexer {
private:
	std::ifstream inFile;
	std::string token;
	char nextch;

protected:
	bool isWhiteSpace(char ch);

	int completeWhiteSpaceToken();
	int completeIdentifierToken();
	int completeNumericToken();
	int completeCommentToken();
	int completeStringToken();
	int completeCharToken();

public:
	std::string getToken();
	int peekIfstream() { return inFile.peek(); }
	Lexer(char* filename);
};

int main(int argc, char* argv[]);
