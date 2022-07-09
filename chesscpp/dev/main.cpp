#include <iostream>
#include "../include/main.hpp"
#include "devutils.hpp"
#include "../include/pstables.hpp"

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

float get_total_score_of_starting_board(float color, Phase phase, C_BoardState &board)
{
    float score = 0;
    for (int i = 0; i < 64; i++)
    {
        uint64_t color_pieces = get_pieces_of_color(board, color);
        if (get_board_at_idx(color_pieces, i) == 0)
        {
            continue;
        }
        int piece_type = get_piece_type_of_field(board, i);
        score += get_score_for_type(phase, color, piece_type, i);
    }
    return score;
}

int main()
{
    // bitboard_table_from_logic(passed_pawns, 1);
    // bitboard_table_from_logic(is_king_shield, Black);
    C_BoardState board = get_new_game_board();
    printf("%f \n", get_total_score_of_starting_board(White, OPENING, board));
    printf("%f \n", get_total_score_of_starting_board(Black, OPENING, board));

    printf("%f \n", get_total_score_of_starting_board(White, ENDGAME, board));
    printf("%f \n", get_total_score_of_starting_board(Black, ENDGAME, board));
}
