#include "state.h"

#include <cstdlib>
#include <iostream>

#include "utilities.h"

State::State(int n, int m, bool random) {
    Resize(n, m);
    if (random) {
        Randomize();
    }
}

State::State(const Grid& grid) {
    grid_ = grid;
    for (int i = 0; i < n(); i++) {
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
    for (int i = 0; i < n(); i++) {
        for (int j = 0; j < m(); j++) {
            grid_[i][j] = rand() % 2 + '0';
        }
    }
}

int State::CountAliveNeighbors(int x, int y) const {
    if (n() == 0 || m() == 0) {
        return 0;
    }
    const int DX[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int DY[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int ans = 0;
    for (int d = 0; d < 8; d++) {
        int nx = (x + DX[d] + n()) % n();
        int ny = (y + DY[d] + m()) % m();
        //std::cout << nx << " " << ny << std::endl;
        ans += IsAlive(nx, ny);
    }
    return ans;
}

bool State::DoesNextGenerationChange(int x, int y) const {
    if (x < 0 || y < 0 || x >= n() || y >= m()) {
        throw std::invalid_argument("invalid coordinates");
    }
    int alive_neighbors = CountAliveNeighbors(x, y);
    if (IsAlive(x, y) && (alive_neighbors < 2 || alive_neighbors >  3)) {
        return true;
    } else if (!IsAlive(x, y) && alive_neighbors == 3) {
        return true;
    }
    return false;
}

void State::SetRow(int i, char* row) {
    for (int j = 0; j < m(); j++) {
        grid_[i][j] = row[j];
    }
}

std::ostream& operator<<(std::ostream& out, const State& state) {
    for (int i = 0; i < state.n(); i++) {
        out << state.grid_[i][0];
        for (int j = 1; j < state.m(); j++) {
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

void swap(State& a, State& b) {
    using std::swap;

    a.grid_.swap(b.grid_);
}
