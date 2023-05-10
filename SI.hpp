#include <fstream>

#include "graphs.hpp"
#include "mpark/patterns.hpp"

void simulate_SI(Graph graph, double beta, int initial_infected,
                 std::ofstream &output_file);
