#include "graphs.hpp"

#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// generate Erdosa-Benyi graph
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
    }

    cout << "Średnia liczba połączeń (ER): " << average_k << "\n";
    return Graph{.nodes = nodes, .adj_list = adj_list};
}

// generate Barabasi-Albert network
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

// draw nad show any graph in a nice way
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
