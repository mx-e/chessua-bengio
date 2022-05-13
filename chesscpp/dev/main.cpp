#include <iostream>
#include "../include/bitboard_constants.hpp"
#include "../include/bitboard_types.hpp"
#include "devutils.hpp"

int main()
{
    C_Board board = get_new_game_board();
    for (int i = 0; i < 8; i++)
    {
        print_bitboard(board.pieces[i]);
    }
}
