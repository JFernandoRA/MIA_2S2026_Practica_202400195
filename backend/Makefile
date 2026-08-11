CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread

SOURCES = main.cpp src/parser/Parser.cpp src/lexer/Lexer.cpp src/commands/CommandSpec.cpp

all: server

server: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o server

clean:
	rm -f server