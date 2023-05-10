#ifndef _GRAPHS_H
#define _GRAPHS_H

#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

struct Edge {
    int to;

    bool operator==(const Edge &other) const { return to == other.to; }
};

typedef std::vector<Edge> Edges;

enum class Node { SUSCEPTIBLE, INFECTED, RESISTANT };

struct Graph {
    std::vector<Node> nodes;
    const std::vector<Edges> adj_list;
};

Graph generate_ER(int N, double p);
Graph generate_BA(int N, int m);

#endif
