#include <fstream>

#include "graphs.hpp"
#include "mpark/patterns.hpp"

void simulate_gossip(Graph graph, double beta, double gamma,
                     int initial_infected, int initial_resitant,
                     std::ofstream &output_file);
