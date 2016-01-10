#ifndef PARALLEL_GAME_OF_LIFE_PARALLEL_SIMULATOR_H_
#define PARALLEL_GAME_OF_LIFE_PARALLEL_SIMULATOR_H_

#include <pthread.h>
#include <vector>
#include <iostream>

#include "state.h"
#include "simulator.h"

class RowThread;

class ParallelSimulator: public Simulator {
public:
    friend class RowThread;

    ParallelSimulator() {
    }
    ParallelSimulator(const State& initial, int thread_cnt, int node_number);

    friend std::ostream& operator<<(std::ostream& out, const ParallelSimulator& p_simulator);

    void Simulate(int steps);
private:
    void Initialize();
    void Step();
    void InitializeThreads();

    int node_number_;
    int prev_node_;
    int next_node_;
    int first_row_;
    int last_row_;
    State next_state_;
};

#endif // PARALLEL_GAME_OF_LIFE_PARALLEL_SIMULATOR_H_
