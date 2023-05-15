#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "graphs.hpp"
#include "simulations.hpp"
#include "utils.hpp"
#include "mpark/patterns.hpp"

using namespace mpark::patterns;
using namespace std;

void ER_SI(double p, int N, double beta) {
    auto filename = format("data/ER_SI_p=%.2f_N=%d_B=%.2f.csv", p, N, beta);

    save_to_file_repeated(
        [N, p, beta](ofstream &output_file) {
            srand(2001);
            Graph ER_graph = generate_ER(N, p);

            srand(time(NULL));
            simulate_SI(ER_graph, beta, 1, output_file);
        },
        filename);
}

void BA_SI(int m, int N, double beta) {
    auto filename = format("data/BA_SI_m=%.2d_N=%d_B=%.2f.csv", m, N, beta);

    save_to_file_repeated(
        [N, m, beta](ofstream &output_file) {
            srand(2001);
            Graph BA_graph = generate_BA(N, m);

            srand(time(NULL));
            simulate_SI(BA_graph, beta, 1, output_file);
        },
        filename);
}

void ER_SIR(double p, int N, double beta, double gamma) {
    auto filename =
        format("data/ER_SIR_p=%.2f_N=%d_B=%.2f_g=%.2f.csv", p, N, beta, gamma);

    save_to_file_repeated(
        [N, p, beta, gamma](ofstream &output_file) {
            srand(2001);
            Graph ER_graph = generate_ER(N, p);

            srand(time(NULL));
            simulate_SIR(ER_graph, beta, gamma, 1, 0, output_file);
        },
        filename);
}

void BA_SIR(int m, int N, double beta, double gamma) {
    auto filename =
        format("data/BA_SIR_m=%.2d_N=%d_B=%.2f_g=%.2f.csv", m, N, beta, gamma);

    save_to_file_repeated(
        [N, m, beta, gamma](ofstream &output_file) {
            srand(2001);
            Graph BA_graph = generate_BA(N, m);

            srand(time(NULL));
            simulate_SIR(BA_graph, beta, gamma, 1, 0, output_file);
        },
        filename);
}

void ER_gossip(double p, int N, double beta, double gamma) {
    auto filename = format("data/ER_gossip_p=%.2f_N=%d_B=%.2f_g=%.2f.csv", p,
                           N, beta, gamma);

    save_to_file_repeated(
        [N, p, beta, gamma](ofstream &output_file) {
            srand(2001);
            Graph ER_graph = generate_ER(N, p);

            srand(time(NULL));
            simulate_gossip(ER_graph, beta, gamma, 1, 0, output_file);
        },
        filename);
}

void BA_gossip(int m, int N, double beta, double gamma) {
    auto filename = format("data/BA_gossip_m=%.2d_N=%d_B=%.2f_g=%.2f.csv", m,
                           N, beta, gamma);

    save_to_file_repeated(
        [N, m, beta, gamma](ofstream &output_file) {
            srand(2001);
            Graph BA_graph = generate_BA(N, m);

            srand(time(NULL));
            simulate_gossip(BA_graph, beta, gamma, 1, 0, output_file);
        },
        filename);
}

int main() {
    cout << "ER SI" << endl;
    ER_SI(0.05, 1000, 0.1);
    ER_SI(0.05, 1000, 0.4);

    cout << "BA SI" << endl;
    BA_SI(5, 1000, 0.1);
    BA_SI(5, 1000, 0.4);

    cout << "ER SIR" << endl;
    ER_SIR(0.05, 1000, 0.2, 0.2);
    ER_SIR(0.05, 1000, 0.2, 0.05);
    ER_SIR(0.05, 1000, 0.05, 0.2);

    cout << "BA SIR" << endl;
    BA_SIR(5, 100, 0.2, 0.2);
    BA_SIR(5, 100, 0.2, 0.05);
    BA_SIR(5, 100, 0.05, 0.2);

    cout << "ER gossip" << endl;
    ER_gossip(0.05, 1000, 0.2, 0.2);
    ER_gossip(0.05, 1000, 0.2, 0.05);
    ER_gossip(0.05, 1000, 0.05, 0.2);

    cout << "BA gossip" << endl;
    BA_gossip(5, 1000, 0.2, 0.2);
    BA_gossip(5, 1000, 0.2, 0.05);
    BA_gossip(5, 1000, 0.05, 0.2);

    Graph ER_graph = generate_ER(50, 0.05);
    Graph BA_graph = generate_BA(50, 5);

    show_graph(ER_graph, "ER.png");
    show_graph(BA_graph, "BA.png");

    return 0;
}
