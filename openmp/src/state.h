#ifndef PARALLEL_GAME_OF_LIFE_STATE_H_
#define PARALLEL_GAME_OF_LIFE_STATE_H_

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

typedef std::vector<std::string> Grid;

class State {
public:
    // Empty grid constructor
    State() { }
    // Random grid constructor
    State(int n, int m, bool random = false);
    State(const Grid& g);
    
    friend std::ostream& operator<<(std::ostream& out, const State& state);
    friend std::istream& operator>>(std::istream& in, State& state);

    friend void swap(State& a, State& b);

    void Resize(int n, int m);
    // Fill grid with random values
    void Randomize();
    bool DoesNextGenerationChange(int x, int y) const;
    int n() const {
        return (int)grid_.size();
    }
    int m() const {
        return (int)((n())? grid_[0].size(): 0);
    }
    int GetCell(int x, int y) const {
        if (x < 0 || y < 0 || x >= n() || y >= m()) {
            throw std::invalid_argument("bad cell coordinates");
        }
        return grid_[x][y];
    }
    int GetInvCell(int x, int y) const {
        if (x < 0 || y < 0 || x >= n() || y >= m()) {
            throw std::invalid_argument("bad cell coordinates");
        }
        return (grid_[x][y] == '0')? '1': '0';
    }
    void SetCell(int x, int y, int val) {
        if (x < 0 || y < 0 || x >= n() || y >= m()) {
            throw std::invalid_argument("bad cell coordinates");
        }
        if (val != '0' && val != '1') {
            throw std::invalid_argument("bad cell value");
        }
        grid_[x][y] = val;
    }
private:
    int CountAliveNeighbors(int x, int y) const;
    int IsAlive(int x, int y) const {
        return grid_[x][y] == '1';
    }

    Grid grid_;
};

#endif // PARALLEL_GAME_OF_LIFE_FIELD_H_

