#ifndef _SIMULATIONS_HPP
#define _SIMULATIONS_HPP

#include <fstream>

#include "graphs.hpp"
#include "mpark/patterns.hpp"

void simulate_SI(Graph graph, double beta, int initial_infected,
                 std::ofstream &output_file);

void simulate_SIR(Graph graph, double beta, double gamma, int initial_infected,
                  int initial_resitant, std::ofstream &output_file);

void simulate_gossip(Graph graph, double beta, double gamma,
                     int initial_infected, int initial_resitant,
                     std::ofstream &output_file);

#endif
