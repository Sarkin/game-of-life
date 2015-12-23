#ifndef PARALLEL_GAME_OF_LIFE_STATE_H_
#define PARALLEL_GAME_OF_LIFE_STATE_H_

#include <vector>
#include <string>
#include <iostream>

typedef std::vector<std::string> Grid;

class State {
public:
    // Empty grid constructor
    State() { }
    // Random grid constructor
    State(int n, int m, int random);
    State(const Grid& g);
    
    friend std::ostream& operator<<(std::ostream& out, const State& state);
    friend std::istream& operator>>(std::istream& in, State& state);

    void Resize(int n, int m);
    // Fill grid with random values
    void Randomize();
private:
    size_t n() const {
        return grid_.size();
    }
    size_t m() const {
        return (n())? grid_[0].size(): 0;
    }

    Grid grid_;
};

#endif // PARALLEL_GAME_OF_LIFE_FIELD_H_

