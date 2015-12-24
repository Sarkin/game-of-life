#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <pthread.h>

#include "state.h"
#include "parallel_simulator.h"

using namespace std::chrono;

State game_state;
ParallelSimulator p_simulator;

void* Simulate(void* steps) {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    p_simulator.Simulate(*(int*)steps);
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    std::cout << duration << std::endl;
    return NULL;
}

int main() {
    while (1) {
        std::cout << "enter 'help' for help" << std::endl;
        char prompt;
        std::string cmd;
        std::cin >> cmd;
        if (cmd == "START") {
            int thread_cnt;
            std::cout << "(r)andomize initial grid or enter from (f)ile?" << std::endl;
            std::cin >> prompt;
            if (prompt == 'r') {
                std::cout << "enter grid's dimensions" << std::endl;
                int n, m;
                std::cin >> n >> m;
                game_state = State(n, m, true);
            } else if (prompt == 'f') {
                std::cout << "reading grid from 'test.csv'" << std::endl;
                std::ifstream fin("test.csv");
                fin >> game_state;
            } else {
                std::cout << "unknown parameter" << std::endl;
                continue;
            }
            std::cout << "enter the number of threads" << std::endl;
            std::cin >> thread_cnt;
            p_simulator = ParallelSimulator(game_state, thread_cnt);
        } else if (cmd == "STATUS") {
            if (p_simulator.IsRunning()) {
                std::cout << "cannot print status while running" << std::endl;
            } else {
                std::cout << p_simulator << std::endl;
            }
        } else if (cmd == "RUN") {
            if (p_simulator.IsRunning()) {
                std::cout << "already running" << std::endl;
            } else {
                std::cout << "enter number of iterations" << std::endl;
                int steps;
                std::cin >> steps;
                pthread_t simulation_thread;
                pthread_create(&simulation_thread, NULL, Simulate, (void *)(&steps));
            }
        } else if (cmd == "STOP") {
            if (p_simulator.IsStopped()) {
                std::cout << "already stopped" << std::endl;
            } else if (!p_simulator.IsRunning()) {
                std::cout << "not running" << std::endl;
            } else {
                p_simulator.Stop();
            }
        } else if (cmd == "QUIT") {
            break;
        } else {
            std::cout << "unknown command" << std::endl;
        }
    }
    return 0;
}
