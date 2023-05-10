#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "SI.hpp"
#include "SIR.hpp"
#include "gossip.hpp"
#include "graphs.hpp"
#include "mpark/patterns.hpp"

using namespace mpark::patterns;
using namespace std;

void ER_SI(double p, int N, double beta) {
    // TODO: beautify with sprintd or something else...
    string filename = "data/ER-p=" + to_string(p) + "_SI-N=" + to_string(N) +
                      "-beta=" + to_string(beta) + ".csv";

    ofstream output_file(filename, std::ofstream::app);

    output_file.close();
    output_file.open(filename, std::ofstream::out | std::ofstream::trunc);

    for (int i = 0; i < 100; i++) {
        srand(2001);

        Graph ER_graph = generate_ER(100, p);
        simulate_SI(ER_graph, beta, 1, output_file);
    }

    output_file.close();
}

void BA_SI(int m, int N, double beta) {
    // TODO: beautify with sprintd or something else...
    string filename = "data/BA-m=" + to_string(m) + "_SI-N=" + to_string(N) +
                      "-beta=" + to_string(beta) + ".csv";

    ofstream output_file(filename, std::ofstream::app);

    output_file.close();
    output_file.open(filename, std::ofstream::out | std::ofstream::trunc);

    for (int i = 0; i < 100; i++) {
        srand(2001);

        Graph BA_graph = generate_BA(100, m);
        simulate_SI(BA_graph, beta, 1, output_file);
    }

    output_file.close();
}

void ER_SIR(double p, int N, double beta, double gamma) {
    // TODO: beautify with sprintd or something else...
    string filename = "data/ER-p=" + to_string(p) + "_SIR-N=" + to_string(N) +
                      "-beta=" + to_string(beta) +
                      "-gamma=" + to_string(gamma) + ".csv";

    ofstream output_file(filename, std::ofstream::app);

    output_file.close();
    output_file.open(filename, std::ofstream::out | std::ofstream::trunc);

    for (int i = 0; i < 100; i++) {
        srand(2001);

        Graph ER_graph = generate_ER(100, p);
        simulate_SIR(ER_graph, beta, gamma, 1, 0, output_file);
    }

    output_file.close();
}

void BA_SIR(int m, int N, double beta, double gamma) {
    // TODO: beautify with sprintd or something else...
    string filename = "data/BA-m=" + to_string(m) + "_SIR-N=" + to_string(N) +
                      "-beta=" + to_string(beta) +
                      "-gamma=" + to_string(gamma) + ".csv";

    ofstream output_file(filename, std::ofstream::app);

    output_file.close();
    output_file.open(filename, std::ofstream::out | std::ofstream::trunc);

    for (int i = 0; i < 100; i++) {
        srand(2001);

        Graph BA_graph = generate_BA(100, m);
        simulate_SIR(BA_graph, beta, gamma, 1, 0, output_file);
    }

    output_file.close();
}

void ER_gossip(double p, int N, double beta, double gamma) {
    // TODO: beautify with sprintd or something else...
    string filename =
        "data/ER-p=" + to_string(p) + "_gossip-N=" + to_string(N) +
        "-beta=" + to_string(beta) + "-gamma=" + to_string(gamma) + ".csv";

    ofstream output_file(filename, std::ofstream::app);

    output_file.close();
    output_file.open(filename, std::ofstream::out | std::ofstream::trunc);

    for (int i = 0; i < 100; i++) {
        srand(2001);

        Graph ER_graph = generate_ER(100, p);
        simulate_gossip(ER_graph, beta, gamma, 1, 0, output_file);
    }

    output_file.close();
}

void BA_gossip(int m, int N, double beta, double gamma) {
    // TODO: beautify with sprintd or something else...
    string filename =
        "data/BA-m=" + to_string(m) + "_gossip-N=" + to_string(N) +
        "-beta=" + to_string(beta) + "-gamma=" + to_string(gamma) + ".csv";

    ofstream output_file(filename, std::ofstream::app);

    output_file.close();
    output_file.open(filename, std::ofstream::out | std::ofstream::trunc);

    for (int i = 0; i < 100; i++) {
        srand(2001);

        Graph BA_graph = generate_BA(100, m);
        simulate_gossip(BA_graph, beta, gamma, 1, 0, output_file);
    }

    output_file.close();
}

int main() {
    cout << "ER SI" << endl;
    ER_SI(0.05, 100, 0.1);
    ER_SI(0.05, 100, 0.4);

    cout << "BA SI" << endl;
    BA_SI(5, 100, 0.1);
    BA_SI(5, 100, 0.4);

    cout << "ER SIR" << endl;
    ER_SIR(0.05, 100, 0.2, 0.2);
    ER_SIR(0.05, 100, 0.2, 0.05);
    ER_SIR(0.05, 100, 0.05, 0.2);

    // cout << "BA SIR" << endl;
    // BA_SIR(5, 100, 0.2, 0.2);
    // BA_SIR(5, 100, 0.2, 0.05);
    // BA_SIR(5, 100, 0.05, 0.2);

    cout << "ER gossip" << endl;
    ER_gossip(0.05, 100, 0.2, 0.2);
    ER_gossip(0.05, 100, 0.2, 0.05);
    ER_gossip(0.05, 100, 0.05, 0.2);

    cout << "BA gossip" << endl;
    BA_gossip(5, 100, 0.2, 0.2);
    BA_gossip(5, 100, 0.2, 0.05);
    BA_gossip(5, 100, 0.05, 0.2);

    return 0;
}
