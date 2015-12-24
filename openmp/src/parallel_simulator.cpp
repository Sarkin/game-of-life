#include "parallel_simulator.h"

#include <vector>
#include <cstdlib>
#include <iostream>

ParallelSimulator::ParallelSimulator() {
    Initialize();
}

ParallelSimulator::ParallelSimulator(const State& state, int thread_cnt): Simulator(state), thread_cnt_(thread_cnt), next_state_(state) {
    Initialize();
}

void ParallelSimulator::Initialize() {
    running_ = false;
    stopped_ = false;
    ended_ = false;
}

void ParallelSimulator::Stop() {
    stopped_ = true;
}

void ParallelSimulator::Quit() {
    ended_ = true;
}

void ParallelSimulator::Simulate(int steps) {
    running_ = true;
    int step = 0;
    #pragma omp parallel num_threads(thread_cnt_) shared(step)
    for (; step < steps && !stopped_; ) {
        #pragma omp barrier
        #pragma omp single
            step++;
        #pragma omp barrier
        #pragma omp for nowait
        for (int row = 0; row < next_state_.n(); row++) {
            for (int column = 0; column < next_state_.m(); column++) {
                if (state_.DoesNextGenerationChange(row, column)) {
                    next_state_.SetCell(row, column, state_.GetInvCell(row, column));
                } else {
                    next_state_.SetCell(row, column, state_.GetCell(row, column));
                }
            }
        }
        #pragma omp barrier
        #pragma omp single
            swap(state_, next_state_);
    }
    running_ = false;
}

std::ostream& operator<<(std::ostream& out, const ParallelSimulator& p_simulator) {
    out << p_simulator.state_;
    return out;
}
