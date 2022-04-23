#include "board.hpp"
#include "utils.hpp"
#include <string>
#include <vector>
#include <iostream>

Board::Board()
{
    reset_board();
}

Board::~Board()
{
}

void Board::reset_board()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
            if (j == 6)
            {
                set_tile(i, j + 1, black_pawn);
            }
            else if (j == 1)
            {
                set_tile(i, j + 1, white_pawn);
            }
            else
            {
                set_tile(i, j + 1, empty);
            }
    }
    set_tile(a, 8, black_rook);
    set_tile(h, 8, black_rook);

    set_tile(b, 8, black_knight);
    set_tile(g, 8, black_knight);

    set_tile(c, 8, black_bishop);
    set_tile(f, 8, black_bishop);

    set_tile(d, 8, black_queen);
    set_tile(e, 8, black_king);

    set_tile(a, 8, white_rook);
    set_tile(h, 8, white_rook);

    set_tile(b, 8, white_knight);
    set_tile(g, 8, white_knight);

    set_tile(c, 8, white_bishop);
    set_tile(f, 8, white_bishop);

    set_tile(d, 8, white_queen);
    set_tile(e, 8, white_king);

    to_move = white;
    can_castle[white_kingside] = true;
    can_castle[white_queenside] = true;
    can_castle[black_kingside] = true;
    can_castle[black_queenside] = true;
    en_passant_tile[0] = -1;
    en_passant_tile[1] = -1;
    reversible_halfmoves_clock = 0;
    move_clock = 0;
}
