#include <gtest/gtest.h>
#include "test_utils.hpp"
#include "../include/main.hpp"

TEST(UCI, CastlingKingCaptureInTransit)
{
    C_BoardState board_state = init_board_state_for_test(White);

    set_pieces(board_state, White, pRook, fill_bitboard({{0, 0}}));
    set_pieces(board_state, Black, pRook, fill_bitboard({{3, 6}}));

    UCIStrings uci_strings = get_uci_moves(board_state);

    EXPECT_EQ(exists<std::string>(
                  uci_strings, [](std::string uci)
                  { return uci == "e1c1"; }),
              false);

    EXPECT_EQ(exists<std::string>(
                  uci_strings, [](std::string uci)
                  { return uci == "a1b1"; }),
              true);

    for(auto st: uci_strings)
    {
        std::cout << st + "\n";
    }
}