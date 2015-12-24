#include "parallel_simulator.h"

#include <vector>
#include <cstdlib>
#include <iostream>

ParallelSimulator::ParallelSimulator() {
    Initialize();
}

ParallelSimulator::ParallelSimulator(const State& state, int thread_cnt): Simulator(state), next_state_(state) {
    threads.resize(thread_cnt);
    Initialize();
}

void ParallelSimulator::Initialize() {
    working_threads_ = 0; 
    running_ = false;
    stopped_ = false;
    ended_ = false;
    threads_initialized_ = false;
    pthread_mutex_init(&new_step_mutex, NULL);
    pthread_cond_init(&new_step_cond, NULL);
    pthread_mutex_init(&finish_mutex, NULL);
    pthread_cond_init(&finish_cond, NULL);
}

void ParallelSimulator::Stop() {
    stopped_ = true;
}

void ParallelSimulator::Quit() {
    ended_ = true;
}

void ParallelSimulator::Simulate(int steps) {
    running_ = true;
    for (int i = 0; i < steps && !stopped_; i++) {
        Step();
        swap(state_, next_state_);
    }
    running_ = false;
}

void ParallelSimulator::Step() {
    working_threads_ = threads.size();
    if (!threads_initialized_) {
        InitializeThreads();
    } else {
        pthread_cond_broadcast(&new_step_cond);
    }
    pthread_mutex_lock(&finish_mutex);
    pthread_cond_wait(&finish_cond, &finish_mutex);
    pthread_mutex_lock(&new_step_mutex);
    pthread_mutex_unlock(&new_step_mutex);
    pthread_mutex_unlock(&finish_mutex);
}

void ParallelSimulator::InitializeThreads() {
    int thread_row_cnt = (state_.n() + threads.size() - 1) / threads.size();
    for (int i = 0; i < (int)threads.size(); i++) {
        int first_row = thread_row_cnt * i;
        int last_row = std::min(first_row + thread_row_cnt - 1, state_.n() - 1);
        threads[i].StartInternalThread(this, first_row, last_row);
    }
    threads_initialized_ = true;
}

std::ostream& operator<<(std::ostream& out, const ParallelSimulator& p_simulator) {
    out << p_simulator.state_;
    return out;
}
