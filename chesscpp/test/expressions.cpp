#include <gtest/gtest.h>
#include "test_utils.hpp"
#include "../include/expressions.hpp"
#include "../include/board_transforms.hpp"

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
    session = construct_session();
    board = session.board_state;
    uint64_t pawns_distr = fill_bitboard_max(empty_board, {2, 14, 62});
    set_pieces(board, Black, pPawn, pawns_distr);
    auto serial_moves = scan_board(get_pawn_single_moves(board, Black));
    EXPECT_EQ(exists<int>(serial_moves, [](int pos)
                          { return pos == 1; }),
              true);
    EXPECT_EQ(exists<int>(serial_moves, [](int pos)
                          { return pos == 13; }),
              true);
    EXPECT_EQ(exists<int>(serial_moves, [](int pos)
                          { return pos == 61; }),
              true);

    // check_no_overflow
    session = construct_session();
    board = session.board_state;
    pawns_distr = fill_bitboard_max(empty_board, {7, 15, 62});
    set_pieces(board, White, pPawn, pawns_distr);
    serial_moves = scan_board(get_pawn_single_moves(board, White));
    EXPECT_EQ(exists<int>(serial_moves, [](int pos)
                          { return pos == 8; }),
              false);
    EXPECT_EQ(exists<int>(serial_moves, [](int pos)
                          { return pos == 16; }),
              false);
    EXPECT_EQ(exists<int>(serial_moves, [](int pos)
                          { return pos == 63; }),
              true);
}

TEST(Expressions, PawnDoubleMove)
{
    // start board
    C_Session session = construct_session();
    session.board_state = get_new_game_board();
    C_BoardState board = session.board_state;
    EXPECT_EQ(get_pawn_double_moves(board, get_pawn_single_moves(board, White), White), row_4);
    EXPECT_EQ(get_pawn_double_moves(board, get_pawn_single_moves(board, Black), Black), row_5);

    // illegal double moves
    session = construct_session();
    board = session.board_state;
    uint64_t pawns_distr = fill_bitboard_max(empty_board, {1, 8, 18, 46});
    set_pieces(board, White, pPawn, pawns_distr);
    auto serial_moves = scan_board(get_pawn_double_moves(board, get_pawn_single_moves(board, White), White));
    EXPECT_EQ(exists<int>(serial_moves, [](int pos)
                          { return pos == 3; }),
              true);
    EXPECT_EQ(exists<int>(serial_moves, [](int pos)
                          { return pos == 10; }),
              false);
    EXPECT_EQ(exists<int>(serial_moves, [](int pos)
                          { return pos == 20; }),
              false);
    EXPECT_EQ(exists<int>(serial_moves, [](int pos)
                          { return pos == 48; }),
              false);
}

TEST(Expressions, PawnAttacks)
{
    // start board
    C_Session session = construct_session();
    session.board_state = get_new_game_board();
    C_BoardState board = session.board_state;
    EXPECT_EQ(get_pawn_attacks(board, attack_left, White), row_3 & ~col_h);
    EXPECT_EQ(get_pawn_attacks(board, attack_right, White), row_3 & ~col_a);

    EXPECT_EQ(get_pawn_attacks(board, attack_left, Black), row_6 & ~col_a);
    EXPECT_EQ(get_pawn_attacks(board, attack_right, Black), row_6 & ~col_h);

    // overflow attack move
    session = construct_session();
    board = session.board_state;
    uint64_t pawns_distr = fill_bitboard_max(empty_board, {7, 63});
    set_pieces(board, White, pPawn, pawns_distr);

    uint64_t pawns_distr2 = fill_bitboard_max(empty_board, {0, 40});
    set_pieces(board, Black, pPawn, pawns_distr2);

    EXPECT_EQ(get_pawn_attacks(board, attack_left, White), 0);
    EXPECT_EQ(get_pawn_attacks(board, attack_left, Black), 0);
}

TEST(Expressions, BishopMoves)
{
    // Test1 - corner + block
    C_Session session = construct_session();
    C_BoardState board = session.board_state;
    uint64_t blockers = fill_bitboard_max(empty_board, {9, 18});
    EXPECT_EQ(get_bishop_moves_and_attacks(0, blockers), row_2 & col_b);
    EXPECT_EQ(get_bishop_moves_and_attacks(2, blockers), (uint64_t)0x0050080402010000LL);

    // Test2 - block all sides
    session = construct_session();
    board = session.board_state;
    blockers = fill_bitboard_max(empty_board, {0, 18, 2, 16});
    EXPECT_EQ(get_bishop_moves_and_attacks(9, blockers), blockers);
}

TEST(Expression, RookMoves)
{
    // Test1 - corner + block
    C_Session session = construct_session();
    C_BoardState board = session.board_state;
    uint64_t blockers = fill_bitboard_max(empty_board, {8, 1});
    EXPECT_EQ(get_rook_moves_and_attacks(0, blockers), blockers);
    EXPECT_EQ(get_rook_moves_and_attacks(7, blockers), 0x7E01010101010101);

    // Test2 - middle
    EXPECT_EQ(get_rook_moves_and_attacks(43, blockers), (row_4 | col_f) & ~(row_4 & col_f));
}