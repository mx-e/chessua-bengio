#ifndef CUSTOM_H
#define CUSTOM_H
#include "../src/main.hpp"
#include <gtest/gtest.h>

bool has_castle(Boards boards, CastleSide side, int color);

void fill_column_with_pawns(Board &board, int column, int color);

void fill_row_with_pawns(Board &board, int row, int color);

#define EXPECT_CASTLE_EXISTS(boards, side, color) EXPECT_EQ(has_castle(boards, side, color), true)

#endif
