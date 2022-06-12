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

// TEST(Bestmove, Capture)
// {
//     Board board = {{{pKing, 0, 0, 0, 0, 0, 0, -pKing},
//                     {0, 0, 0, 0, 0, 0, 0, 0},
//                     {0, 0, 0, 0, 0, 0, 0, 0},
//                     {0, pBishop, 0, 0, 0, 0, 0, 0},
//                     {0, 0, 0, 0, 0, 0, 0, 0},
//                     {0, 0, 0, 0, 0, 0, 0, 0},
//                     {0, 0, 0, 0, 0, 0, 0, 0},
//                     {0, 0, 0, 0, 0, -pRook, 0, 0}}};
//     std::string value = bestmove(board, 1., {}, false, false, false, false, 0, 0);

//     EXPECT_EQ("d2h6", value);
// }

TEST(Bestmove, Checkmate)
{
    Board board = {{{0, pRook, 0, 0, 0, 0, 0, pKing},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {pRook, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pKing}}};
    std::string value = bestmove_benchmark(4, board, 1., {}, false, false, false, false, 0, 0);

    EXPECT_EQ(value, "a2h2");
}

TEST(Bestmove, MoreComplexBoard)
{
    Board board = {{{0, pRook, 0, -pRook, 0, 0, pQueen, pKing},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, pKnight, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, -pQueen, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, -pPawn, 0, 0, 0, 0, -pPawn, 0},
                    {0, 0, -pBishop, 0, 0, 0, 0, -pKing}}};
    std::string value = bestmove_benchmark(10, board, 1., {}, false, false, false, false, 0, 0);

    EXPECT_EQ(value, "a2h2");
}