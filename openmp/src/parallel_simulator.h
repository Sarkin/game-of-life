#ifndef PARALLEL_GAME_OF_LIFE_PARALLEL_SIMULATOR_H_
#define PARALLEL_GAME_OF_LIFE_PARALLEL_SIMULATOR_H_

#include <vector>
#include <iostream>

#include "state.h"
#include "simulator.h"

class ParallelSimulator: public Simulator {
public:
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

    int thread_cnt_;
    bool stopped_;
    bool running_;
    bool ended_;
    State next_state_;
};

#endif // PARALLEL_GAME_OF_LIFE_PARALLEL_SIMULATOR_H_
