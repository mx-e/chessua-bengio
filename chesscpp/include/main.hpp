#ifndef MAIN_H
#define MAIN_H
#include <iostream>
#include "moves.hpp"
#include "bitboard.hpp"

UCIStrings generate_moves(Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove);

void runtime_benchmark(Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove, int runs);

inline C_Board bb_create_board(Board board)
{
    C_Board bitboard = mailbox_to_bitboard_representation(board);
    // TODO: support enpassant, castling, etc.

    return bitboard;
};

inline UCIStrings bb_generate_moves(Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove)
{
    C_Board bitboard = bb_create_board(board);
    return get_uci_moves(bitboard);
};

inline void bb_runtime_benchmark(Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove, int runs)
{
    C_Board bitboard = bb_create_board(board);
    for (int _ = 0; _ < runs; _++)
    {
        bitboard.collect_legal_moves();
    }
}

#endif
