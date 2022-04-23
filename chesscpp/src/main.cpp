#include <array>
#include "main.hpp"

float big_loop(int size) {
    int result = 0;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            result += 1;
        }
    }
    return (float) result;
}

Board get_board() {
    Board m = {};
    return m;
}

Board add_1_to_board(Board board) {
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            board[i].at(j) += 1;
        }
    }
    return board;
}