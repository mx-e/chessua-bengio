#include <iostream>
#include "../include/main.hpp"
#include "devutils.hpp"

bool passed_pawns(int col, int row, int c, int r, float color)
{
    return ((c == col || c == col - 1 || c == col + 1) &&
            ((color == 1. && r > row) ||
             (color == -1 && r < row)));
}

bool is_king_shield(int col, int row, int c, int r, float color)
{
    return ((c == col || c == col - 1 || c == col + 1) &&
            ((color == 1 && r > row && r <= (row + 2)) ||
             (color == -1. && r < row && r >= (row - 2)))

    );
}

int main()
{
    // bitboard_table_from_logic(passed_pawns, 1);
    bitboard_table_from_logic(is_king_shield, Black);
}
