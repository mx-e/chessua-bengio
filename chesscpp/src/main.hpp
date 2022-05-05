#ifndef MAIN_H
#define MAIN_H
#include <iostream>
#include "moves.hpp"

Board get_board();

BoardStates get_possible_boards(BoardState boardState);
CastlingRights compute_castling_rights(BoardState boardState);

#endif

