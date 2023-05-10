CXX=g++

CXXFLAGS=-c -g -std=c++20 -shared -pthread -m64 -I./mpark

all: program

main.o: main.cpp graphs.hpp SI.hpp SIR.hpp gossip.hpp
	$(CXX) $(CXXFLAGS) -o main.o main.cpp

graphs.o: graphs.cpp graphs.hpp
	$(CXX) $(CXXFLAGS) -o graphs.o graphs.cpp

SI.o: SI.cpp graphs.hpp SI.hpp
	$(CXX) $(CXXFLAGS) -o SI.o SI.cpp

SIR.o: SIR.cpp graphs.hpp SIR.hpp
	$(CXX) $(CXXFLAGS) -o SIR.o SIR.cpp

gossip.o: gossip.cpp gossip.hpp
	$(CXX) $(CXXFLAGS) -o gossip.o gossip.cpp

program: main.o graphs.o SI.o SIR.o gossip.o
	$(CXX) -o program main.o graphs.o SI.o SIR.o gossip.o

clean:
	rm -f *.o
