#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

#include "TAxis.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TLine.h"
#include "TRandom.h"
#include "TRootCanvas.h"
#include "TSystem.h"
#include "mpark/patterns.hpp"

using namespace mpark::patterns;
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

Graph generate_ER(int N, double p) {
    vector<Node> nodes(N);
    vector<Edges> adj_list;

    double average_k = 0;

    for (int i = 0; i < N; i++) {
        Edges edges;

        for (int j = 0; j < N; j++) {
            if ((double)rand() / RAND_MAX < p && i != j) {
                edges.push_back(Edge{.to = j});
            }
        }

        adj_list.push_back(edges);
        average_k += (double)edges.size() / N;
        // cout << i << " " << edges.size() << endl;
    }

    cout << "Średnia liczba połączeń (ER): " << average_k << "\n";
    return Graph{.nodes = nodes, .adj_list = adj_list};
}

Graph generate_BA(int N, int m) {
    vector<int> node_indices;
    vector<Edges> adj_list;

    for (int i = 0; i < m; i++) {
        Edges initial_edges;

        for (int j = i + 1; j < m; j++) {
            if (i == j) continue;

            initial_edges.push_back(Edge{.to = j});
            node_indices.push_back(i);
            node_indices.push_back(j);
        }

        adj_list.push_back(initial_edges);
    }

    int rand_node_index;

    for (int i = m; i < N; i++) {
        Edges edges;

        for (int k = 0; k < m; k++) {
            Edge edge;

            do {
                rand_node_index = rand() % node_indices.size();
                edge.to = node_indices.at(rand_node_index);

            } while (find(edges.begin(), edges.end(), edge) != edges.end());

            edges.push_back(edge);
            node_indices.push_back(i);
            node_indices.push_back(edge.to);
        }

        adj_list.push_back(edges);
    }

    cout << "Średnia liczba połączeń (BA): " << (double)node_indices.size() / N
         << "\n";

    vector<Node> nodes(N);
    return Graph{.nodes = nodes, .adj_list = adj_list};
}

void simulate_SI(Graph &graph, double beta, int initial_infected) {
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

    bool should_infect;
    int infected = initial_infected;

    int edges_i, rand_edge;

    Node *node_i, *node_j;

    while (infected < N) {
        infected_graph->SetPoint(step, step, infected);
        susceptible_graph->SetPoint(step, step, N - infected);

        // iterate through all nodes
        for (int i = 0; i < N; ++i) {
            edges_i = graph.adj_list.at(i).size();

            if (edges_i == 0) continue;

            rand_edge = rand() % edges_i;

            should_infect = ((double)rand() / RAND_MAX) < beta;

            node_i = &graph.nodes.at(i);
            node_j = &graph.nodes.at(graph.adj_list.at(i).at(rand_edge).to);

            match(*node_i, *node_j)(
                pattern(Node::SUSCEPTIBLE, Node::INFECTED) =
                    [&, node_i] {
                        WHEN(should_infect) {
                            *node_i = Node::INFECTED;
                            infected++;
                        };
                    },
                pattern(Node::INFECTED, Node::SUSCEPTIBLE) =
                    [&, node_j] {
                        WHEN(should_infect) {
                            *node_j = Node::INFECTED;
                            infected++;
                        };
                    },
                pattern(_, _) = [] {});
        }
        step++;
    }
    infected_graph->SetLineColor(kRed);
    infected_graph->Draw();

    susceptible_graph->SetLineColor(kBlue);
    susceptible_graph->Draw("same");
    legend->Draw("same");
}

void simulate_SIR(Graph &graph, double beta, double gamma,
                  int initial_infected, int initial_resitant) {
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
    Node *node_i, *node_j;

    while (infected > 0) {
        susceptible_graph->AddPoint(step, N - resistant - infected);
        infected_graph->AddPoint(step, infected);
        resistant_graph->AddPoint(step, resistant);
        // cout << step << " " << N - resistant - infected << " " << infected
        // << " " << resistant << "\n";

        // iterate through all nodes
        for (int i = 0; i < N; ++i) {
            node_i_neighbors = graph.adj_list.at(i).size();

            if (node_i_neighbors == 0) continue;

            j = graph.adj_list.at(i).at(rand() % node_i_neighbors).to;

            should_infect = ((double)rand() / RAND_MAX) < beta;
            should_resist = ((double)rand() / RAND_MAX) < gamma;

            node_i = &graph.nodes.at(i);
            node_j = &graph.nodes.at(j);

            if (should_resist && *node_i == Node::INFECTED) {
                *node_i = Node::RESISTANT;
                resistant++;
                infected--;
            } else {
                match(*node_i, *node_j)(
                    pattern(Node::SUSCEPTIBLE, Node::INFECTED) =
                        [&, node_i] {
                            WHEN(should_infect) {
                                *node_i = Node::INFECTED;
                                infected++;
                            };
                        },
                    pattern(Node::INFECTED, Node::SUSCEPTIBLE) =
                        [&, node_j] {
                            WHEN(should_infect) {
                                *node_j = Node::INFECTED;
                                infected++;
                            };
                        },
                    pattern(_, _) = [] {});
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

void simulate_gossip(Graph &graph, double beta, double gamma,
                     int initial_infected, int initial_resitant) {
    const int N = graph.nodes.size();

    for (int i = 0; i < initial_infected; i++)
        graph.nodes.at(i) = Node::INFECTED;

    for (int i = initial_infected; i < initial_infected + initial_resitant;
         i++)
        graph.nodes.at(i) = Node::RESISTANT;

    for (int i = initial_infected + initial_resitant; i < N; i++)
        graph.nodes.at(i) = Node::SUSCEPTIBLE;

    // simulate and show
    TCanvas *sir_canvas =
        new TCanvas("gossip_canvas", "gossip model", 1200, 800);
    TGraph *susceptible_graph = new TGraph();
    TGraph *infected_graph = new TGraph();
    TGraph *resistant_graph = new TGraph();
    TLegend *legend = new TLegend(0.7, 0.8, 0.85, 0.65);
    legend->AddEntry(susceptible_graph, "susceptible", "l");
    legend->AddEntry(infected_graph, "infected", "l");
    legend->AddEntry(resistant_graph, "resistant", "l");

    susceptible_graph->SetTitle("Gossip;Step;Number of nodes");

    long step = 0;

    int infected = initial_infected;
    int resistant = initial_resitant;

    int node_i_neighbors, rand_neighbor, j;

    bool should_infect, should_resist;
    Node *node_i, *node_j;

    while (infected > 0) {
        susceptible_graph->AddPoint(step, N - resistant - infected);
        infected_graph->AddPoint(step, infected);
        resistant_graph->AddPoint(step, resistant);
        cout << step << " " << N - resistant - infected << " " << infected
             << " " << resistant << "\n";
        // iterate through all nodes
        for (int i = 0; i < N; ++i) {
            node_i_neighbors = graph.adj_list.at(i).size();

            if (node_i_neighbors == 0) continue;

            j = graph.adj_list.at(i).at(rand() % node_i_neighbors).to;

            should_infect = ((double)rand() / RAND_MAX) < beta;
            should_resist = ((double)rand() / RAND_MAX) < gamma;

            node_i = &graph.nodes.at(i);
            node_j = &graph.nodes.at(j);

            match(*node_i, *node_j)(
                pattern(Node::SUSCEPTIBLE, Node::INFECTED) =
                    [&, node_i] {
                        WHEN(should_infect) {
                            *node_i = Node::INFECTED;
                            infected++;
                        };
                    },
                pattern(Node::INFECTED, Node::SUSCEPTIBLE) =
                    [&, node_j] {
                        WHEN(should_infect) {
                            *node_j = Node::INFECTED;
                            infected++;
                        };
                    },
                pattern(Node::INFECTED, Node::INFECTED) =
                    [&, node_i] {
                        WHEN(should_resist) {
                            *node_i = Node::RESISTANT;
                            resistant++;
                            infected--;
                        };
                    },
                pattern(Node::INFECTED, Node::RESISTANT) =
                    [&, node_i] {
                        WHEN(should_resist) {
                            *node_i = Node::RESISTANT;
                            resistant++;
                            infected--;
                        };
                    },
                pattern(Node::RESISTANT, Node::INFECTED) =
                    [&, node_j] {
                        WHEN(should_resist) {
                            *node_j = Node::RESISTANT;
                            resistant++;
                            infected--;
                        };
                    },
                pattern(_, _) = [] {});
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

    Graph ER_graph = generate_ER(100, 0.05);
    Graph BA_graph = generate_BA(500, 7);

    // simulate_SI(ER_graph, 0.05, 1);
    simulate_SIR(ER_graph, 0.2, 0.05, 1, 0);
    // simulate_gossip(ER_graph, 0.05, 0.05, 1, 0);

    // simulate_SI(BA_graph, 0.5, 1);
    // simulate_SIR(BA_graph, 0.2, 0.05, 1, 0);
    // simulate_gossip(BA_graph, 0.2, 0.05, 1, 0);

    show_graph(ER_graph);
    // show_graph(BA_graph);
}
