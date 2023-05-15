CXX=g++

CXXFLAGS=-c -g -shared -pthread -m64 -I./mpark -I`root-config --cxx --cflags`

all: program

main.o: main.cpp graphs.hpp simulations.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -o main.o main.cpp

graphs.o: graphs.cpp graphs.hpp
	$(CXX) $(CXXFLAGS) -o graphs.o graphs.cpp

SI.o: SI.cpp graphs.hpp simulations.hpp
	$(CXX) $(CXXFLAGS) -o SI.o SI.cpp

SIR.o: SIR.cpp graphs.hpp simulations.hpp
	$(CXX) $(CXXFLAGS) -o SIR.o SIR.cpp

gossip.o: gossip.cpp simulations.hpp
	$(CXX) $(CXXFLAGS) -o gossip.o gossip.cpp

program: main.o graphs.o SI.o SIR.o gossip.o
	$(CXX) -o program main.o graphs.o SI.o SIR.o gossip.o `root-config --libs`

clean:
	rm -f *.o
