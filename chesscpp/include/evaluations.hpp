#ifndef EVALUATIONS
#define EVALUATIONS

#include "expressions.hpp"

// https://en.wikipedia.org/wiki/Hamming_weight
int count_pieces(const C_BoardState &board, const float color, const uint8_t piece_type)
{
    uint64_t x = get_pieces(board, color, piece_type);
    int count = 0;
    for (count = 0; x; count++)
        x &= x - 1;

    return count;
}

inline float evaluate(C_BoardState &board_state)
{

    float score = 0;

    score += count_pieces(board_state, White, b_pawns);
    score += -count_pieces(board_state, Black, b_pawns);

    score += 3 * count_pieces(board_state, White, b_bishops);
    score += -3 * count_pieces(board_state, Black, b_bishops);

    score += 3 * count_pieces(board_state, White, b_knights);
    score += -3 * count_pieces(board_state, Black, b_knights);

    score += 5 * count_pieces(board_state, White, b_rooks);
    score += -5 * count_pieces(board_state, Black, b_rooks);

    score += 9 * count_pieces(board_state, White, b_queens);
    score += -9 * count_pieces(board_state, Black, b_queens);

    return board_state.turn * score;
}

#endif