#ifndef _GRAPHS_H
#define _GRAPHS_H

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

#include "TCanvas.h"
#include "TGraph.h"
#include "TLine.h"

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

void show_graph(const Graph &graph, const char *graph_file_name);

#endif
