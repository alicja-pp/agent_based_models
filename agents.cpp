#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

#include "SI.hpp"
#include "SIR.hpp"
#include "TApplication.h"
#include "gossip.hpp"
#include "graphs.hpp"
#include "mpark/patterns.hpp"

using namespace mpark::patterns;
using namespace std;

int main() {
    TApplication app("app", 0, 0);

    srand(time(NULL));
    Graph ER_graph = generate_ER(100, 0.05);
    Graph BA_graph = generate_BA(500, 7);

    // simulate_SI(ER_graph, 0.05, 1);
    // simulate_SIR(ER_graph, 0.2, 0.05, 1, 0);
    // simulate_gossip(ER_graph, 0.05, 0.05, 1, 0);

    // simulate_SI(BA_graph, 0.5, 1);
    // simulate_SIR(BA_graph, 0.2, 0.05, 1, 0);
    simulate_gossip(BA_graph, 0.2, 0.05, 1, 0);

    // show_graph(ER_graph);
    //  show_graph(BA_graph);

    app.Run();

    return 0;
}
