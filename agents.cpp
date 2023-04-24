#include <iostream>
#include <random>
#include <vector>

#include "TCanvas.h"
#include "TGraph.h"
// #include "graph.h"

using namespace std;
struct Edge {
    long to;
};

enum class Node { SUSCEPTIBLE, INFECTED, RESISTANT };

struct Graph {
    const std::vector<Node> nodes;
    const std::vector<std::vector<Edge>> edges;
};

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

long N = 100;

void agents() {
    TCanvas *canvas = new TCanvas("canvas", "Path", 1200, 800);
    canvas->Divide(3, 1);

    TGraph *agents = new TGraph(N);
    TGraph *lines = new TGraph(N + 1);

    agents->SetTitle("Graph");
    agents->SetMarkerColor(9);
    agents->SetMarkerStyle(29);
    agents->SetMarkerSize(1);

    auto graph = generate_ER(N, 0.5);

    int x, y;

    for (long i = 0; i < N; ++i) {
        for (long j = 0; j < N && i != j; ++j) {
            do {
                x = rand() % 70;
                y = rand() % 70;
            } while (x == y);

            agents->SetPoint(i, x, y);
        }
    }

    agents->Draw("AP");
}