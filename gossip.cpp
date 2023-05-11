#include "gossip.hpp"

using namespace mpark::patterns;
using namespace std;

void simulate_gossip(Graph graph, double beta, double gamma,
                     int initial_infected, int initial_resitant,
                     ofstream &output_file) {
    const int N = graph.nodes.size();

    for (int i = 0; i < initial_infected; i++)
        graph.nodes.at(i) = Node::INFECTED;

    for (int i = initial_infected; i < initial_infected + initial_resitant;
         i++)
        graph.nodes.at(i) = Node::RESISTANT;

    for (int i = initial_infected + initial_resitant; i < N; i++)
        graph.nodes.at(i) = Node::SUSCEPTIBLE;

    long step = 0;

    int infected = initial_infected;
    int resistant = initial_resitant;

    int node_i_neighbors, j;

    bool should_infect, should_resist;
    Node *node_i, *node_j;

    while (step < 100) {
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

        output_file << step << "," << N - infected - resistant << ","
                    << infected << "," << resistant << "\n";

        step++;
    }
}
