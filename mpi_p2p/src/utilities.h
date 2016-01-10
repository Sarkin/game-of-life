#ifndef PARALLEL_GAME_OF_LIFE_UTILITIES_H_
#define PARALLEL_GAME_OF_LIFE_UTILITIES_H_

#include <string>
#include <stdexcept>

std::string ParseCSV(const std::string& s);

int GetFirstRowNumber(int n, int thread_cnt, int node_number);

int GetLastRowNumber(int n, int thread_cnt, int node_number);

#endif
