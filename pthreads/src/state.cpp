#include "state.h"

#include <cstdlib>

#include "utilities.h"

State::State(int n, int m, bool random) {
    Resize(n, m);
    if (random) {
        Randomize();
    }
}

State::State(const Grid& grid) {
    grid_ = grid;
    for (size_t i = 0; i < n(); i++) {
        if (grid_[i].size() != grid_[0].size()) {
            throw std::invalid_argument("invalid grid");
        }
    }
}

void State::Resize(int n, int m) {
    grid_.clear();
    grid_.resize(n, std::string(m, '0'));
}

void State::Randomize() {
    for (size_t i = 0; i < n(); i++) {
        for (size_t j = 0; j < m(); j++) {
            grid_[i][j] = rand() % 2 + '0';
        }
    }
}

std::ostream& operator<<(std::ostream& out, const State& state) {
    for (size_t i = 0; i < state.n(); i++) {
        out << state.grid_[i][0];
        for (size_t j = 0; j < state.m(); j++) {
            out << "," << state.grid_[i][j];
        }
        out << std::endl;
    }
    return out;
}

std::istream& operator>>(std::istream& in, State& state) {
    std::string tmp;
    Grid grid;
    while (std::getline(in, tmp)) {
        try {
            grid.push_back(ParseCSV(tmp));
        } catch (const std::invalid_argument& e) {
            in.setstate(std::ios_base::failbit);
        }
    }
    state = State(grid);
    return in;
}
