CXX=g++

CXXFLAGS=-c -g -std=c++17 -pthread -m64 -I./mpark

all: program

main.o: main.cpp graphs.hpp simulations.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -o main.o main.cpp

graphs.o: graphs.cpp graphs.hpp
	$(CXX) $(CXXFLAGS) -I`root-config --cxx --cflags` -o graphs.o graphs.cpp

SI.o: SI.cpp graphs.hpp simulations.hpp
	$(CXX) $(CXXFLAGS) -o SI.o SI.cpp

SIR.o: SIR.cpp graphs.hpp simulations.hpp
	$(CXX) $(CXXFLAGS) -o SIR.o SIR.cpp

gossip.o: gossip.cpp simulations.hpp
	$(CXX) $(CXXFLAGS) -o gossip.o gossip.cpp

program: main.o graphs.o SI.o SIR.o gossip.o
	$(CXX) `root-config --cflags --glibs` -o program main.o graphs.o SI.o SIR.o gossip.o

clean:
	rm -f *.o
