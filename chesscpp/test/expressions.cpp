#include <gtest/gtest.h>
#include "test_utils.hpp"
#include "../include/expressions.hpp"

TEST(Expressions, GetPieces)
{
    C_Session session = construct_session();
    session.board_state = get_new_game_board();
    uint64_t b_bishops = get_pieces(session.board_state, Black, pBishop);
    uint64_t w_pawns = get_pieces(session.board_state, White, pPawn);
    uint64_t b_queen = get_pieces(session.board_state, Black, pQueen);

    EXPECT_EQ(b_bishops, black_bishops_starting_config);
    EXPECT_EQ(w_pawns, white_pawns_starting_config);
    EXPECT_EQ(b_queen, black_queen_starting_config);
}

TEST(Expressions, GetPiecesSpecific)
{
    C_Session session = construct_session();
    session.board_state = get_new_game_board();
    C_BoardState board = session.board_state;

    EXPECT_EQ(get_king(board, White), white_king_starting_config);
    EXPECT_EQ(get_king(board, Black), black_king_starting_config);
    EXPECT_EQ(get_queen(board, White), white_queen_starting_config);
    EXPECT_EQ(get_queen(board, Black), black_queen_starting_config);
    EXPECT_EQ(get_bishops(board, White), white_bishops_starting_config);
    EXPECT_EQ(get_bishops(board, Black), black_bishops_starting_config);
    EXPECT_EQ(get_knights(board, White), white_knights_starting_config);
    EXPECT_EQ(get_knights(board, Black), black_knights_starting_config);
    EXPECT_EQ(get_rooks(board, White), white_rooks_starting_config);
    EXPECT_EQ(get_rooks(board, Black), black_rooks_starting_config);
    EXPECT_EQ(get_pawns(board, White), white_pawns_starting_config);
    EXPECT_EQ(get_pawns(board, Black), black_pawns_starting_config);
}

TEST(Expressions, ColoredPieces)
{
    C_Session session = construct_session();
    session.board_state = get_new_game_board();
    C_BoardState board = session.board_state;
    EXPECT_EQ(get_pieces_of_color(board, White), row_1 | row_2);
    EXPECT_EQ(get_pieces_of_color(board, Black), row_7 | row_8);
}

TEST(Expressions, EmptyEnemyFields)
{
    C_Session session = construct_session();
    session.board_state = get_new_game_board();
    C_BoardState board = session.board_state;
    EXPECT_EQ(get_empty_fields(board), center_half);
    EXPECT_EQ(get_enemy_fields(board, Black), row_1 | row_2);
    EXPECT_EQ(get_enemy_fields(board, White), row_7 | row_8);
    EXPECT_EQ(get_empty_or_enemy(board, White), center_half | row_7 | row_8);
}

TEST(Expressions, PawnSingleMoves)
{
    // start board
    C_Session session = construct_session();
    session.board_state = get_new_game_board();
    C_BoardState board = session.board_state;
    EXPECT_EQ(get_pawn_single_moves(board, White), row_3);
    EXPECT_EQ(get_pawn_single_moves(board, Black), row_6);

    // some distributed pawns

    // check_no_overflow
}