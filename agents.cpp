#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <set>
#include <vector>

#include "TAxis.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TLine.h"
#include "TRandom.h"
#include "TRootCanvas.h"
#include "TSystem.h"
using namespace std;

struct Edge {
    int to;

    bool operator==(const Edge &other) const { return to == other.to; }
};

typedef vector<Edge> Edges;

enum class Node { SUSCEPTIBLE, INFECTED, RESISTANT };

struct Graph {
    vector<Node> nodes;
    const vector<Edges> adj_list;
};

Graph generate_ER(int n, double p) {
    vector<Node> nodes(n);
    vector<Edges> adj_list;

    for (int i = 0; i < n; i++) {
        Edges edges;

        for (int j = 0; j < n; j++) {
            if ((double)rand() / RAND_MAX < p && i != j) {
                edges.push_back(Edge{.to = j});
            }
        }

        adj_list.push_back(edges);
    }

    return Graph{.nodes = nodes, .adj_list = adj_list};
}

Graph generate_BA(int n, int m) {
    vector<int> node_indices;
    iota(begin(node_indices), end(node_indices), 0);

    vector<Edges> adj_list;

    int rand_node_index;

    for (int i = 0; i < n; i++) {
        Edges edges;

        for (int k = 0; k < m; k++) {
            Edge edge;

            do {
                rand_node_index = rand() % n;
                edge.to = rand_node_index;

            } while (find(edges.begin(), edges.end(), edge) != edges.end());

            edges.push_back(edge);
            node_indices.push_back(rand_node_index);
        }

        adj_list.push_back(edges);
    }

    vector<Node> nodes(n);
    return Graph{.nodes = nodes, .adj_list = adj_list};
}

void simulate_SI(Graph graph, double beta, int initial_infected) {
    // starting with some (infected_nr) random infected nodes
    const int N = graph.nodes.size();
    for (int i = 0; i < initial_infected; i++)
        graph.nodes.at(i) = Node::INFECTED;

    for (int i = initial_infected; i < N; i++)
        graph.nodes.at(i) = Node::SUSCEPTIBLE;

    // simulate and show
    TCanvas *si_canvas = new TCanvas("si_canvas", "SI model", 1200, 800);
    TGraph *infected_graph = new TGraph();
    TGraph *susceptible_graph = new TGraph();
    TLegend *legend = new TLegend(0.7, 0.8, 0.85, 0.65);
    legend->AddEntry(susceptible_graph, "susceptible", "l");
    legend->AddEntry(infected_graph, "infected", "l");

    infected_graph->SetTitle("SI;Step;Number of nodes");

    long step = 0;

    int infected = initial_infected;
    int node_i_neighbors, rand_neighbor, j;

    while (max(infected, infected - 100) < N) {
        infected_graph->SetPoint(step, step, infected);
        susceptible_graph->SetPoint(step, step, N - infected);

        // iterate through all nodes
        for (int i = 0; i < N; ++i) {
            node_i_neighbors = graph.adj_list.at(i).size();

            if (node_i_neighbors == 0) continue;

            rand_neighbor = rand() % node_i_neighbors;

            j = graph.adj_list.at(i).at(rand_neighbor).to;

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
    infected_graph->SetLineColor(kRed);
    infected_graph->Draw();

    susceptible_graph->SetLineColor(kBlue);
    susceptible_graph->Draw("same");
    legend->Draw("same");
}

void simulate_SIR(Graph graph, double beta, double gamma, int initial_infected,
                  int initial_resitant) {
    const int N = graph.nodes.size();

    for (int i = 0; i < initial_infected; i++)
        graph.nodes.at(i) = Node::INFECTED;

    for (int i = initial_infected; i < initial_infected + initial_resitant;
         i++)
        graph.nodes.at(i) = Node::RESISTANT;

    for (int i = initial_infected + initial_resitant; i < N; i++)
        graph.nodes.at(i) = Node::SUSCEPTIBLE;

    // simulate and show
    TCanvas *sir_canvas = new TCanvas("sir_canvas", "SIR model", 1200, 800);
    TGraph *susceptible_graph = new TGraph();
    TGraph *infected_graph = new TGraph();
    TGraph *resistant_graph = new TGraph();
    TLegend *legend = new TLegend(0.7, 0.8, 0.85, 0.65);
    legend->AddEntry(susceptible_graph, "susceptible", "l");
    legend->AddEntry(infected_graph, "infected", "l");
    legend->AddEntry(resistant_graph, "resistant", "l");

    susceptible_graph->SetTitle("SIR;Step;Number of nodes");

    long step = 0;

    int infected = initial_infected;
    int resistant = initial_resitant;

    int node_i_neighbors, rand_neighbor, j;

    bool should_infect, should_resist;

    while (infected > 0) {
        // cout << N - resistant - infected << ", " << resistant << ", " <<
        // infected << "\n";

        susceptible_graph->SetPoint(step, step, N - resistant - infected);
        infected_graph->SetPoint(step, step, infected);
        resistant_graph->SetPoint(step, step, resistant);

        // iterate through all nodes
        for (int i = 0; i < N; ++i) {
            node_i_neighbors = graph.adj_list.at(i).size();

            if (node_i_neighbors == 0) continue;

            j = graph.adj_list.at(i).at(rand() % node_i_neighbors).to;

            should_infect = ((double)rand() / RAND_MAX) < beta;
            should_resist = ((double)rand() / RAND_MAX) < gamma;

            if (should_infect && graph.nodes.at(i) == Node::INFECTED &&
                graph.nodes.at(j) == Node::SUSCEPTIBLE) {
                graph.nodes.at(j) = Node::INFECTED;
                infected++;

            } else if (should_infect && graph.nodes.at(j) == Node::INFECTED &&
                       graph.nodes.at(i) == Node::SUSCEPTIBLE) {
                graph.nodes.at(i) = Node::INFECTED;
                infected++;

            } else if (should_resist && graph.nodes.at(i) == Node::INFECTED &&
                       graph.nodes.at(j) == Node::INFECTED) {
                graph.nodes.at(i) = Node::RESISTANT;
                infected--;
                resistant++;

            } else if (should_resist && graph.nodes.at(i) == Node::INFECTED &&
                       graph.nodes.at(j) == Node::RESISTANT) {
                graph.nodes.at(i) = Node::RESISTANT;
                infected--;
                resistant++;

            } else if (should_resist && graph.nodes.at(j) == Node::INFECTED &&
                       graph.nodes.at(i) == Node::RESISTANT) {
                graph.nodes.at(j) = Node::RESISTANT;
                infected--;
                resistant++;
            }
        }

        step++;
    }

    susceptible_graph->SetLineColor(kBlue);
    susceptible_graph->GetYaxis()->SetRangeUser(0, N);
    susceptible_graph->Draw();

    infected_graph->SetLineColor(kRed);
    infected_graph->GetYaxis()->SetRangeUser(0, N);
    infected_graph->Draw("same");

    resistant_graph->SetLineColor(kGreen);
    resistant_graph->GetYaxis()->SetRangeUser(0, N);
    resistant_graph->Draw("same");

    legend->Draw("same");
}

void show_graph(const Graph &graph) {
    TCanvas *canvas = new TCanvas("canvas", "Graph", 800, 800);
    int max_coordinate = 10;

    const int N = graph.nodes.size();
    TGraph *agents = new TGraph(N);

    canvas->Range(-max_coordinate - 2., -max_coordinate - 2.,
                  max_coordinate + 2, max_coordinate + 2);
    agents->SetTitle("Graph");
    agents->SetMarkerColor(9);
    agents->SetMarkerStyle(29);
    agents->SetMarkerSize(1);

    double rand1, rand2;
    for (int i = 0; i < N; ++i) {
        do {
            rand1 = sqrt((double)rand() / RAND_MAX) * max_coordinate;
            rand2 = ((double)rand() / RAND_MAX) * 2 * M_PI;
        } while (rand1 == rand2);
        agents->SetPoint(i, rand1 * cos(rand2), rand1 * sin(rand2));
    }

    double x1, y1, x2, y2;
    vector<TLine *> connections;

    // iterate through all nodes
    for (int i = 0; i < N; ++i) {
        agents->GetPoint(i, x1, y1);

        // iterate through all i-th node's edges
        for (int j = 0; j < graph.adj_list.at(i).size(); ++j) {
            agents->GetPoint(graph.adj_list.at(i).at(j).to, x2, y2);

            // draw line between i-th node and j-th node
            connections.push_back(new TLine(x1, y1, x2, y2));
        }
    }

    for (auto line : connections) line->Draw("same");
    agents->Draw("P, same");
}

void agents() {
    srand(time(NULL));

    // Graph ER_graph = generate_ER(1000, 0.008);
    Graph BA_graph = generate_BA(1000, 3);

    // simulate_SI(ER_graph, 0.05, 1);
    // simulate_SIR(ER_graph, 0.1, 0.05, 10, 0);
    simulate_SI(BA_graph, 0.5, 1);
    simulate_SIR(BA_graph, 0.1, 0.05, 10, 0);

    show_graph(BA_graph);
}
