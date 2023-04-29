#include "SIR.hpp"

#include "graphs.hpp"
#include "mpark/patterns.hpp"

using namespace mpark::patterns;
using namespace std;

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

    int node_i_neighbors, j;

    bool should_infect, should_resist;
    Node *node_i, *node_j;

    while (infected > 0 && step < N * N) {
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

    sir_canvas->SaveAs("SIR.png");
}