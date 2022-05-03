#ifndef CUSTOM_H
#define CUSTOM_H
#include "../src/main.hpp"
#include <gtest/gtest.h>

bool has_castle(BoardStates boardStates, CastleSide side, int color);
void fill_column_with_pawns(Board &board, int column, int color);
void fill_row_with_pawns(Board &board, int row, int color);
bool exists(BoardStates boardStates, std::function<bool(BoardState)> boardStateCondition);

#define EXPECT_CASTLE_EXISTS(boardStates, side, color) EXPECT_EQ(has_castle(boardStates, side, color), true)
#define EXPECT_EXISTS(boardStates, boardStateCondition) EXPECT_EQ(exists(boardStates, boardStateCondition), true)

#endif
