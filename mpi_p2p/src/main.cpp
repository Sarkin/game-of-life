#include <iostream>
#include <fstream>
#include <stdexcept>
#include <mpi.h>

#include "parallel_simulator.h"
#include "utilities.h"

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int world_size;
    int node_number;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &node_number);
    State game_state;
    ParallelSimulator p_simulator;
    int thread_cnt;
    int steps;
    if (argc == 4) {
        const char* filename = argv[1];
        sscanf(argv[2], "%d", &thread_cnt);
        sscanf(argv[3], "%d", &steps);
        std::ifstream fin(filename);
        fin >> game_state;
    } else if (argc == 5) {
        int n, m;
        sscanf(argv[1], "%d", &n);
        sscanf(argv[2], "%d", &m);
        sscanf(argv[3], "%d", &thread_cnt);
        sscanf(argv[4], "%d", &steps);
        game_state = State(n, m, true);
    }

    if (node_number == 0) {
        std::ofstream fout("answer.out");
        fout << game_state << std::endl;
        double start_time = MPI_Wtime();
        char** ans_grid = new char*[game_state.n()];
        for (int i = 0; i < game_state.n(); i++) {
            ans_grid[i] = new char[game_state.m() + 1];
        }
        for (int i = 1; i <= thread_cnt; i++) {
            int first_row = GetFirstRowNumber(game_state.n(), thread_cnt, i - 1);
            int last_row = GetLastRowNumber(game_state.n(), thread_cnt, i - 1);
            for (int j = first_row; j <= last_row; j++) {
                MPI_Recv(ans_grid[j], game_state.m(), MPI_CHAR,
                        i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        double end_time = MPI_Wtime();
        std::cout << end_time - start_time << " sec" << std::endl;
        for (int i = 0; i < game_state.n(); i++) {
            fout << ans_grid[i] << std::endl;
        }
        for (int i = 0; i < game_state.n(); i++) {
            delete[] ans_grid[i];
        }
        delete[] ans_grid;
    } else if (node_number <= thread_cnt) {
        p_simulator = ParallelSimulator(game_state, thread_cnt, node_number);
        p_simulator.Simulate(steps);
    }

    MPI_Finalize();
    return 0;
}
