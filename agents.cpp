#include <iostream>
#include <random>
#include <vector>

#include "TAxis.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLine.h"

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

void show_graph(const Graph &graph) {
    TCanvas *canvas = new TCanvas("canvas", "Path", 1200, 800);

    const long n = graph.nodes.size();
    TGraph *agents = new TGraph(n);

    agents->SetTitle("Graph");
    agents->SetMarkerColor(9);
    agents->SetMarkerStyle(29);
    agents->SetMarkerSize(1);

    agents->GetXaxis()->SetLabelSize(0);
    agents->GetYaxis()->SetLabelSize(0);
    agents->GetXaxis()->SetTickLength(0);
    agents->GetYaxis()->SetTickLength(0);

    long rand1, rand2;

    for (long i = 0; i < n; ++i) {
        for (long j = 0; j < n && i != j; ++j) {
            do {
                rand1 = rand() % 70;
                rand2 = rand() % 70;
            } while (rand1 == rand2);

            agents->SetPoint(i, rand1, rand2);
        }
    }

    agents->Draw("AP");

    double x1, y1, x2, y2;
    TLine *connection;

    // iterate through all nodes
    for (long i = 0; i < n; ++i) {
        agents->GetPoint(i, x1, y1);

        // iterate through all i-th node's edges
        for (long j = 0; j < graph.edges.at(i).size(); ++j) {
            agents->GetPoint(graph.edges.at(i).at(j).to, x2, y2);

            // draw line between i-th node and j-th node
            connection = new TLine(x1, y1, x2, y2);
            connection->Draw("same");
        }
    }
}

void agents() {
    Graph ER_graph = generate_ER(100, 0.5);

    show_graph(ER_graph);
}
