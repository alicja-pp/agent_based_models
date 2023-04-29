#include "SI.hpp"

#include "graphs.hpp"
#include "mpark/patterns.hpp"

using namespace mpark::patterns;
using namespace std;

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
