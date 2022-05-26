#include <gtest/gtest.h>
#include "test_utils.hpp"
#include "../include/board_transforms.hpp"
#include "../include/expressions.hpp"

TEST(BoardTransforms, SetSingleField)
{
    // set
    C_Session session = construct_session();
    C_BoardState board = session.board_state;

    set_single_piece(board, White, pPawn, flat(0, 0));
    std::vector<int> w_pawns = scan_board(get_pawns(board, White));
    std::vector<int> white_pieces = scan_board(get_pieces_of_color(board, White));

    EXPECT_EQ(exists<int>(w_pawns, [](int pos)
                          { return pos == 0; }),
              true);
    EXPECT_EQ(exists<int>(white_pieces, [](int pos)
                          { return pos == 0; }),
              true);

    set_single_piece(board, Black, pQueen, flat(1, 1));
    std::vector<int> b_queen = scan_board(get_queen(board, Black));
    std::vector<int> black_pieces = scan_board(get_pieces_of_color(board, Black));

    EXPECT_EQ(exists<int>(b_queen, [](int pos)
                          { return pos == 9; }),
              true);
    EXPECT_EQ(exists<int>(black_pieces, [](int pos)
                          { return pos == 9; }),
              true);

    // unset
    unset_single_piece(board, White, pPawn, flat(0, 0));
    w_pawns = scan_board(get_pawns(board, White));
    white_pieces = scan_board(get_pieces_of_color(board, White));

    EXPECT_EQ(exists<int>(w_pawns, [](int pos)
                          { return pos == 0; }),
              false);
    EXPECT_EQ(exists<int>(white_pieces, [](int pos)
                          { return pos == 0; }),
              false);

    unset_single_piece(board, Black, pQueen, flat(1, 1));
    b_queen = scan_board(get_queen(board, Black));
    black_pieces = scan_board(get_pieces_of_color(board, Black));

    EXPECT_EQ(exists<int>(b_queen, [](int pos)
                          { return pos == 9; }),
              false);
    EXPECT_EQ(exists<int>(black_pieces, [](int pos)
                          { return pos == 9; }),
              false);
}

TEST(BoardTransforms, SetPieces)
{
    C_Session session = construct_session();
    C_BoardState board = session.board_state;

    uint64_t knights = fill_bitboard_max(empty_board, {2, 23, 32, 58});
    set_pieces(board, White, pKnight, knights);
    EXPECT_EQ(get_knights(board, White), knights);
    EXPECT_EQ(get_knights(board, Black), empty_board);

    set_pieces(board, White, pKnight, empty_board);
    EXPECT_EQ(get_knights(board, White), empty_board);
}

TEST(BoardTransforms, SetUnsetCastlingRights)
{
    C_Session session = construct_session();
    C_BoardState board = session.board_state;

    unset_castling_rights(board, w_queenside);
    EXPECT_EQ((int)board.castling_rights, (int)UINT8_C(0x0D));

    unset_castling_rights(board, b_kingside);
    EXPECT_EQ((int)board.castling_rights, (int)UINT8_C(0x09));

    unset_castling_rights(board, b_queenside);
    unset_castling_rights(board, w_kingside);

    EXPECT_EQ((int)board.castling_rights, (int)UINT8_C(0x00));

    set_castling_rights(board, b_queenside);
    EXPECT_EQ((int)board.castling_rights, (int)UINT8_C(0x08));

    set_castling_rights(board, w_kingside);
    set_castling_rights(board, b_kingside);
    set_castling_rights(board, w_queenside);

    EXPECT_EQ((int)board.castling_rights, (int)UINT8_C(0x0F));
}
