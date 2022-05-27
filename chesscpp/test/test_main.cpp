#include <gtest/gtest.h>
#include "test_utils.hpp"
#include "../include/main.hpp"

TEST(Main, UCICastlingKingCaptureInTransit)
{
    C_BoardState board_state = init_board_state_for_test(White);
    board_state.half_moves = 0.;
    board_state.moves = 1.;

    set_pieces(board_state, White, pRook, fill_bitboard({{0, 0}, {7, 0}}));
    set_pieces(board_state, White, pPawn, fill_bitboard({{0, 1}}));
    set_pieces(board_state, White, pBishop, fill_bitboard({{4, 1}}));
    set_pieces(board_state, White, pKnight, fill_bitboard({{5, 1}}));

    set_pieces(board_state, Black, pRook, fill_bitboard({{3, 6}, {0, 7}}));
    set_pieces(board_state, Black, pQueen, fill_bitboard({{4, 7}}));
    set_pieces(board_state, Black, pBishop, fill_bitboard({{7, 3}}));
    set_pieces(board_state, Black, pPawn, fill_bitboard({{0, 2}}));

    UCIStrings uci_strings = get_uci_moves(board_state);

    EXPECT_EQ(exists<std::string>(
                  uci_strings, [](std::string uci)
                  { return uci == "e1c1"; }),
              false);

    EXPECT_EQ(exists<std::string>(
                  uci_strings, [](std::string uci)
                  { return uci == "a1b1"; }),
              true);
}