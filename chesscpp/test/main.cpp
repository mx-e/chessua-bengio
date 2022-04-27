#include "../src/main.hpp"
#include <gtest/gtest.h>

TEST(HelloTest, BasicAssertions) {
    Board board = get_board();
    board[0].at(0) = 2;

    Boards boards = get_possible_boards(board, 1);

    EXPECT_GE(boards.size(), 2);
}