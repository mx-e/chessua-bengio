#include <gtest/gtest.h>
#include "test_utils.hpp"
#include "../include/board_transforms.hpp"
#include "../include/expressions.hpp"

TEST(BoardTransforms, SetSingleField)
{
    C_Session session = construct_session();
    C_BoardState board = session.board_state;
    board.pieces[b_pawns] = 0;
    set_single_piece(board, White, pPawn, flat(0, 0));

    std::vector<int> pawns = scan_board(get_pawns(board, White));

    EXPECT_EQ(exists<int>(pawns, [](int pos)
                          { return pos == 0; }),
              true);
}