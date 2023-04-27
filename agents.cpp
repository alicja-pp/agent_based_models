#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#include "TAxis.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLine.h"
#include "TRandom.h"
#include "TRootCanvas.h"
#include "TSystem.h"

using namespace std;

struct Edge {
    long to;
};

enum class Node { SUSCEPTIBLE, INFECTED, RESISTANT };

struct Graph {
    vector<Node> nodes;
    const vector<vector<Edge>> edges;
};

Graph generate_ER(long n, double p) {
    vector<Node> nodes(n);
    vector<vector<Edge>> edges;

    for (long i = 0; i < n; i++) {
        vector<Edge> neighbors;

        for (long j = 0; j < n; j++) {
            if ((double)rand() / RAND_MAX < p && i != j) {
                neighbors.push_back(Edge{.to = j});
            }
        }

        edges.push_back(neighbors);
    }

    return Graph{.nodes = nodes, .edges = edges};
}

Graph generate_BA(long n, double m) {
    vector<Node> nodes(n);
    vector<vector<Edge>> edges;

    for (long i = 0; i < n; i++) {
        vector<Edge> neighbors;

        // TODO

        edges.push_back(neighbors);
    }

    return Graph{.nodes = nodes, .edges = edges};
}

void simulate_SI(Graph &graph, double beta, long initial_infected) {
    // starting with some (infected_nr) random infected nodes
    const long n = graph.nodes.size();
    for (int i = 0; i < initial_infected; i++) {
        graph.nodes.at(i) = Node::INFECTED;
    }
    for (int i = initial_infected; i < n; i++) {
        graph.nodes.at(i) = Node::SUSCEPTIBLE;
    }

    // simulate and show
    TCanvas *si_canvas = new TCanvas("si_canvas", "Graph", 1200, 800);
    TGraph *infected_graph = new TGraph();
    TGraph *susceptible_graph = new TGraph();

    infected_graph->SetTitle("SI;Time;Number of infected nodes");

    long step = 0;

    int infected = initial_infected;
    int node_i_neighbors, rand_neighbor, j;

    while (max(infected, infected - 100) < n) {
        infected_graph->SetPoint(step, step, infected);
        susceptible_graph->SetPoint(step, step, n - infected);

        // iterate through all nodes
        for (int i = 0; i < n; ++i) {
            node_i_neighbors = graph.edges.at(i).size();

            if (node_i_neighbors == 0) {
                cout << "i = " << i << ", with k = " << node_i_neighbors
                     << endl;
                continue;
            }

            rand_neighbor = rand() % node_i_neighbors;

            j = graph.edges.at(i).at(rand_neighbor).to;

            if (((double)rand() / RAND_MAX) < beta) {
                if (graph.nodes.at(i) == Node::INFECTED &&
                    graph.nodes.at(j) == Node::SUSCEPTIBLE) {
                    graph.nodes.at(j) = Node::INFECTED;
                    infected++;
                } else if (graph.nodes.at(j) == Node::INFECTED &&
                           graph.nodes.at(i) == Node::SUSCEPTIBLE) {
                    graph.nodes.at(i) = Node::INFECTED;
                    infected++;
                }
            }
        }
        step++;
    }

    infected_graph->Draw();
    susceptible_graph->Draw("same");
}

void show_graph(const Graph &graph) {
    TCanvas *canvas = new TCanvas("canvas", "Graph", 800, 800);
    int max_coordinate = 10;

    const long n = graph.nodes.size();
    TGraph *agents = new TGraph(n);

    canvas->Range(-max_coordinate - 2., -max_coordinate - 2.,
                  max_coordinate + 2, max_coordinate + 2);
    ((TRootCanvas *)canvas->GetCanvasImp())->ShowMenuBar(0);
    agents->SetTitle("Graph");
    agents->SetMarkerColor(9);
    agents->SetMarkerStyle(29);
    agents->SetMarkerSize(1);

    agents->GetXaxis()->SetLabelSize(0);
    agents->GetYaxis()->SetLabelSize(0);
    agents->GetXaxis()->SetTickLength(0);
    agents->GetYaxis()->SetTickLength(0);

    double rand1, rand2;
    for (long i = 0; i < n; ++i) {
        do {
            rand1 = sqrt((double)rand() / RAND_MAX) * max_coordinate;
            rand2 = ((double)rand() / RAND_MAX) * 2 * M_PI;
        } while (rand1 == rand2);
        agents->SetPoint(i, rand1 * cos(rand2), rand1 * sin(rand2));
    }

    double x1, y1, x2, y2;
    vector<TLine *> connections;

    // iterate through all nodes
    for (long i = 0; i < n; ++i) {
        agents->GetPoint(i, x1, y1);

        // iterate through all i-th node's edges
        for (long j = 0; j < graph.edges.at(i).size(); ++j) {
            agents->GetPoint(graph.edges.at(i).at(j).to, x2, y2);

            // draw line between i-th node and j-th node
            connections.push_back(new TLine(x1, y1, x2, y2));
        }
    }

    for (auto line : connections) line->Draw("same");
    agents->Draw("P, same");
}

void agents() {
    srand(time(NULL));

    Graph ER_graph = generate_ER(1000, 0.008);

    simulate_SI(ER_graph, 0.05, 1);
    show_graph(ER_graph);
}
