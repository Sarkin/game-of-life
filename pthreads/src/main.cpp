#include <iostream>
#include <fstream>
#include <string>

#include "state.h"

State game_state;

int main() {
    while (1) {
        char prompt;
        std::string cmd;
        std::cin >> cmd;
        if (cmd == "START") {
            int thread_cnt;
            std::cout << "(r)andomize initial grid or enter from (f)ile?" << std::endl;
            std::cin >> prompt;
            if (prompt == 'r') {
                std::cout << "enter grid's dimensions" << std::endl;
                int n, m;
                std::cin >> n >> m;
                game_state = State(n, m, true);
            } else {
                std::cout << "reading grid from 'test.csv'" << std::endl;
                std::ifstream fin("test.csv");
                fin >> game_state;
            }
            std::cout << "enter the number of threads" << std::endl;
            std::cin >> thread_cnt;
        } else if (cmd == "STATUS") {
        } else if (cmd == "RUN") {
        } else if (cmd == "STOP") {
        } else if (cmd == "QUIT") {
        } else {
            std::cout << "unknown command" << std::endl;
        }
    }
    return 0;
}
