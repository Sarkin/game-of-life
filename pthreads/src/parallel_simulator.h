#ifndef PARALLEL_GAME_OF_LIFE_PARALLEL_SIMULATOR_H_
#define PARALLEL_GAME_OF_LIFE_PARALLEL_SIMULATOR_H_

#include <pthread.h>
#include <vector>
#include <iostream>

#include "state.h"
#include "simulator.h"
#include "row_thread.h"

class RowThread;

class ParallelSimulator: public Simulator {
public:
    friend RowThread;

    ParallelSimulator();
    ParallelSimulator(const State& initial, int thread_cnt);

    friend std::ostream& operator<<(std::ostream& out, const ParallelSimulator& p_simulator);

    void Simulate(int steps);
    void Stop();
    void Quit();
    bool IsRunning() const {
        return running_;
    }
    bool IsStopped() const {
        return stopped_;
    }
private:
    void Initialize();
    void Step();
    void InitializeThreads();

    int working_threads_;
    bool stopped_;
    bool running_;
    bool ended_;
    bool threads_initialized_;
    State next_state_;
    std::vector<RowThread> threads;
    pthread_mutex_t new_step_mutex;
    pthread_cond_t new_step_cond;
    pthread_mutex_t finish_mutex;
    pthread_cond_t finish_cond;
};

#endif // PARALLEL_GAME_OF_LIFE_PARALLEL_SIMULATOR_H_
