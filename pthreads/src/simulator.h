#ifndef PARALLEL_GAME_OF_LIFE_SIMULATOR_H_
#define PARALLEL_GAME_OF_LIFE_SIMULATOR_H_

#include "state.h"

class Simulator {
public:
    Simulator() { }
    Simulator(const State& initial): state_(initial) { }

    virtual void Simulate(int steps) = 0;
protected:
    State state_;
};

#endif // PARALLEL_GAME_OF_LIFE_SIMULATOR_H_
