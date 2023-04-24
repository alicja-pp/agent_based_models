#include "graph.h"

#include <iostream>
#include <random>
#include <vector>

using namespace std;

Graph generate_ER(long n, double p) {
    vector<Node> nodes(n);
    vector<vector<Edge>> edges;

    for (long i = 0; i < n; i++) {
        vector<Edge> neighbors(n);

        for (long j = 0; j < n && i != j; j++) {
            if (((double)rand() / (RAND_MAX)) < p) {
                neighbors.push_back(Edge{.to = j});
            }
        }

        edges.push_back(neighbors);
    }

    return Graph{.nodes = nodes, .edges = edges};
}