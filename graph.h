#include <iostream>
#include <random>
#include <vector>

struct Edge {
    long to;
};

enum class Node { SUSCEPTIBLE, INFECTED, RESISTANT };

struct Graph {
    const std::vector<Node> nodes;
    const std::vector<std::vector<Edge>> edges;
};

Graph generate_ER(long n, double p);