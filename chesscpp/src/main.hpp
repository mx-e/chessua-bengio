#ifndef MAIN_H
#define MAIN_H
#include <iostream>
#include "moves.hpp"

Board get_board();
CastlingRights compute_castling_rights(BoardState boardState);

#endif

