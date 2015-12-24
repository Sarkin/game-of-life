#ifndef PARALLEL_GAME_OF_LIFE_ROW_THREAD_H_
#define PARALLEL_GAME_OF_LIFE_ROW_THREAD_H_

#include <pthread.h>

#include "parallel_simulator.h"

class ParallelSimulator;

class RowThread {
public:
    bool StartInternalThread(ParallelSimulator* p_simulator, int first_row, int last_row);
    void WaitForInternalThreadToExit();

private:
    void InternalThreadEntry(ParallelSimulator* p_simulator, int first_row, int last_row);
    static void* InternalThreadEntryFunction(void* varg);

    pthread_t thread_;
    pthread_cond_t finish_cond;
};

struct PthreadArg {
    RowThread* this_;
    ParallelSimulator* p_simulator_;
    int first_row_;
    int last_row_;
    PthreadArg(RowThread* _this, ParallelSimulator* p_simulator, int first_row, int last_row)
        : this_(_this),
        p_simulator_(p_simulator),
        first_row_(first_row),
        last_row_(last_row) {
    }
};

#endif // PARALLEL_GAME_OF_LIFE_ROW_THREAD_H_
