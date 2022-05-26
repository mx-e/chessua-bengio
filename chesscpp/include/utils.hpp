#ifndef UTILS
#define UTILS

#include "bitboard_constants.hpp"

inline uint8_t get_piece_type_of_field(const C_Board &board, int position_idx)
{
    uint8_t piece = 0;
    uint64_t mask = most_sig_bit >> position_idx;
    piece += pKing * (bool)(mask & board.pieces[b_kings]);
    piece += pQueen * (bool)(mask & board.pieces[b_queens]);
    piece += pBishop * (bool)(mask & board.pieces[b_bishops]);
    piece += pKnight * (bool)(mask & board.pieces[b_knights]);
    piece += pRook * (bool)(mask & board.pieces[b_rooks]);
    piece += pPawn * (bool)(mask & board.pieces[b_pawns]);
    return piece;
}

#endif