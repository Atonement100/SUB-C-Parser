all: lexer

clean: 
	rm -rf *.o 
	rm -rf *.exe

lexer: analyzer.cpp
	g++ -Wall -std=c++11 -o lexer analyzer.cpp analyzer.h
