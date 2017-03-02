#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

class Lexer {
private:

protected:
	bool isWhiteSpace(char ch);
	int completeWhiteSpaceToken(std::ifstream& inFile, std::string& token, char& nextch);
	int completeIdentifierToken(std::ifstream& inFile, std::string& token, char& nextch);
	int completeNumericToken(std::ifstream& inFile, std::string& token, char& nextch);

public:
	std::string getToken(std::ifstream& inFile);

};

int main(int argc, char* argv[]);
