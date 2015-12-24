#include "row_thread.h"
#include <iostream>

pthread_mutex_t creation_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

bool RowThread::StartInternalThread(ParallelSimulator* p_simulator, int first_row, int last_row) {
    PthreadArg* arg = new PthreadArg(this, p_simulator, first_row, last_row);
    bool status = (pthread_create(&thread_, NULL, InternalThreadEntryFunction, (void*)arg) == 0);
    return status;
}

void RowThread::WaitForInternalThreadToExit() {
    (void)pthread_join(thread_, NULL);
}

void* RowThread::InternalThreadEntryFunction(void* varg) {
    PthreadArg* arg = (PthreadArg*)varg;
    arg->this_->InternalThreadEntry(arg->p_simulator_, arg->first_row_, arg->last_row_);
    delete arg;
    return NULL;
}

void RowThread::InternalThreadEntry(ParallelSimulator* p_simulator, int first_row, int last_row) {
    while (true) {
        if (p_simulator->ended_) {
            return;
        }
        for (int row = first_row; row <= last_row; row++) {
            for (int column = 0; column < p_simulator->next_state_.m(); column++) {
                if (p_simulator->state_.DoesNextGenerationChange(row, column)) {
                    p_simulator->next_state_.SetCell(row, column, p_simulator->state_.GetInvCell(row, column));
                } else {
                    p_simulator->next_state_.SetCell(row, column, p_simulator->state_.GetCell(row, column));
                }
            }
        }
        pthread_mutex_lock(&(p_simulator->new_step_mutex));
        p_simulator->working_threads_--;
        if (p_simulator->working_threads_ == 0) {
            pthread_cond_signal(&p_simulator->finish_cond);
        }
        pthread_cond_wait(&(p_simulator->new_step_cond), &(p_simulator->new_step_mutex));
        pthread_mutex_unlock(&(p_simulator->new_step_mutex));
    }
}
