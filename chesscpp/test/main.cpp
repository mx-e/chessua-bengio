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