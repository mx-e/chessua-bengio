#ifndef MAIN_H
#define MAIN_H
#include <iostream>
#include "moves.hpp"

Board get_board();
CastlingRights compute_castling_rights(BoardState boardState);

typedef std::vector<std::string> UCIStrings;

UCIStrings generate_moves(Board board, int color, EnPassants enpassant, bool kingSideWhite, bool queenSideWhite, bool kingSideBlack, bool queenSideBlack, int halfMove, int fullMove);

#endif

