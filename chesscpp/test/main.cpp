#include "../src/main.hpp"
#include <gtest/gtest.h>

TEST(PossibleBoards, Knight) {
    Board board = get_board();
    board[0].at(0) = 2;

    Boards boards = get_possible_boards(board, 1);

    EXPECT_GE(boards.size(), 2);
}

TEST(PossibleBoards, BlockedRook) {
    Board board = get_board();
    board[0].at(0) = 5;
    board[1].at(0) = -6;
    board[0].at(3) = -6;

    Boards boards = get_possible_boards(board, 1);

    EXPECT_EQ(boards.size(), 2);
}

void fill_column_with_pawns(Board &board, int column, int color)
{
    for(int x = 0; x < 8; x++)
    {
        board[x].at(column) = color * 6;
    }
}

void fill_row_with_pawns(Board &board, int row, int color)
{
    for(int x = 0; x < 8; x++)
    {
        board[row].at(x) = color * 6;
    }
}


TEST(PossibleBoards, KingSideCastleWhite) {
    Board board = get_board();
    board[4].at(0) = 1;
    board[7].at(0) = 5;

    fill_column_with_pawns(board, 1, -1);
    fill_row_with_pawns(board, 3, -1);

    Boards boards = get_possible_boards(board, 1);

    // Rook up by two, King one down (side perspective) and 1 Castle
    EXPECT_EQ(boards.size(), 3);
}