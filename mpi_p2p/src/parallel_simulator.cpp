#include "parallel_simulator.h"

#include <vector>
#include <cstdlib>
#include <iostream>
#include <mpi.h>

#include "utilities.h"

ParallelSimulator::ParallelSimulator(const State& state, int thread_cnt, int node_number): Simulator(state), node_number_(node_number), next_state_(state) {
    node_number--;
    prev_node_ = (node_number - 1 + thread_cnt) % thread_cnt + 1;
    next_node_ = (node_number + 1) % thread_cnt + 1;
    first_row_ = GetFirstRowNumber(state_.n(), thread_cnt, node_number);
    last_row_ = GetLastRowNumber(state_.n(), thread_cnt, node_number);
}

void ParallelSimulator::Initialize() {
}

void ParallelSimulator::Simulate(int steps) {
    for (int i = 0; i < steps; i++) {
        Step();
        swap(state_, next_state_);
    }
    for (int row = first_row_; row <= last_row_; row++) {
        MPI_Send((void*)state_.GetRow(row), state_.m(), MPI_CHAR,
                0, 0, MPI_COMM_WORLD);
    }
}

void ParallelSimulator::Step() {
    MPI_Bsend((void*)state_.GetRow(first_row_), state_.m(), MPI_CHAR,
            prev_node_, 0, MPI_COMM_WORLD);
    MPI_Bsend((void*)state_.GetRow(last_row_), state_.m(), MPI_CHAR,
            next_node_, 0, MPI_COMM_WORLD);
    char* prev_row = new char[state_.m() + 1];
    MPI_Recv(prev_row, state_.m(), MPI_CHAR,
            prev_node_, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    char* next_row = new char[state_.m() + 1];
    MPI_Recv(next_row, state_.m(), MPI_CHAR,
            next_node_, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    state_.SetRow((first_row_ - 1 + state_.n()) % state_.n(), prev_row);
    state_.SetRow((first_row_ + 1) % state_.n(), next_row);
    delete[] prev_row;
    delete[] next_row;
    for (int row = first_row_; row <= last_row_; row++) {
        for (int column = 0; column < next_state_.m(); column++) {
            if (state_.DoesNextGenerationChange(row, column)) {
                next_state_.SetCell(row, column, state_.GetInvCell(row, column));
            } else {
                next_state_.SetCell(row, column, state_.GetCell(row, column));
            }
        }
    }
}

std::ostream& operator<<(std::ostream& out, const ParallelSimulator& p_simulator) {
    out << p_simulator.state_;
    return out;
}
