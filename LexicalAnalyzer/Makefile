CXX = g++
CXXFLAGS = -Wall -std=c++11
OBJS = main.o parser.o token.o analyzer.o

all: parser

clean: 
	rm -rf *.o 
	rm -rf *.exe

parser: $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) -o parser
	
main.o: main.cpp

token.o: token.cpp

analyzer.o: analyzer.cpp 

parser.o: parser.cpp