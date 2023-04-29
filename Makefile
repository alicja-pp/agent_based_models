CXX=g++

CXXFLAGS=-c -g -std=c++17 -I`root-config --cxx --cflags`

all: program

agents.o: agents.cpp graphs.hpp SI.hpp SIR.hpp gossip.hpp
	$(CXX) $(CXXFLAGS) -o agents.o agents.cpp

graphs.o: graphs.cpp graphs.hpp
	$(CXX) $(CXXFLAGS) -o graphs.o graphs.cpp

SI.o: SI.cpp graphs.hpp SI.hpp
	$(CXX) $(CXXFLAGS) -o SI.o SI.cpp

SIR.o: SIR.cpp graphs.hpp SIR.hpp
	$(CXX) $(CXXFLAGS) -o SIR.o SIR.cpp

gossip.o: gossip.cpp graphs.hpp gossip.hpp
	$(CXX) $(CXXFLAGS) -o gossip.o gossip.cpp

program: agents.o graphs.o SI.o SIR.o gossip.o
	$(CXX) `root-config --cflags --glibs` -o program agents.o graphs.o SI.o SIR.o gossip.o

clean:
	rm -f *.o
