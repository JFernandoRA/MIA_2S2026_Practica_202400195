CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread

all: server

server: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o server

clean:
	rm -f server