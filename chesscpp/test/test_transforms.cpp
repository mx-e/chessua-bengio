#include <gtest/gtest.h>
#include "test_utils.hpp"
#include "../include/transforms.hpp"
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

TEST(BoardTransform, ExecuteMoveForwardBackward)
{
    move queen_move_black = create_move(20, 36, pPawn, 0, 0, 0, 0);
    move rook_capture_white = create_move(0, 7, pRook, 0, 0, 0, 0);
    move castling_move_black = create_move(39, 55, 0, 0, 1, 0, 0);
    move king_in_check_move_white = create_move(32, 33, pPawn);
    move illegal_castling_move_black = create_move(39, 23, 0, 0, 2, 0, 0);
}

TEST(BoardTransform, PushPopBoardPawnSingleMove)
{
    move pawn_move_white = create_move(3, 4);
    C_Session session = construct_session();
    C_BoardState board = init_board_state_for_test(White);
    session.board_state = board;
    uint64_t pawns = fill_bitboard_max(empty_board, {3, 15});
    set_pieces(board, White, pPawn, pawns);

    board.moves = 5.;
    board.half_moves = 20.;
    board.en_passant = 34;

    MoveList moves = MoveList();
    push_move(board, pawn_move_white, moves);

    EXPECT_EQ(get_piece_type_of_field(board, 3), 0);
    EXPECT_EQ(get_piece_type_of_field(board, 15), pPawn);
    EXPECT_EQ(get_piece_type_of_field(board, 4), pPawn);
    EXPECT_EQ(board.moves, 5.);
    EXPECT_EQ(board.half_moves, 0.);
    EXPECT_EQ(board.en_passant, 0);
    EXPECT_EQ(board.turn, -1.0);

    move top_of_stack = board.move_stack.back();

    EXPECT_EQ(top_of_stack.prev_half_move_c, 20.);
    EXPECT_EQ(top_of_stack.ep_field, 34);

    pop_move(board, moves);

    EXPECT_EQ(get_piece_type_of_field(board, 3), pPawn);
    EXPECT_EQ(get_piece_type_of_field(board, 15), pPawn);
    EXPECT_EQ(get_piece_type_of_field(board, 4), 0);
    EXPECT_EQ(board.half_moves, 20.);
    EXPECT_EQ(board.en_passant, 34);
    EXPECT_EQ(board.turn, 1.0);
}

TEST(BoardTransform, PushPopPawnDoubleMove)
{
    move pawn_double_move_black = create_move(6, 4, 0, 0, 0, 0, 5);
    C_Session session = construct_session();
    C_BoardState board = init_board_state_for_test(Black);
    session.board_state = board;
    uint64_t pawns = fill_bitboard_max(empty_board, {6, 15});
    set_pieces(board, Black, pPawn, pawns);

    board.en_passant = 60;
    board.moves = 5.;

    push_move(board, pawn_double_move_black, session.move_list_stack[0]);

    EXPECT_EQ(get_piece_type_of_field(board, 4), pPawn);
    EXPECT_EQ(get_piece_type_of_field(board, 6), 0);
    EXPECT_EQ(get_piece_type_of_field(board, 15), pPawn);
    EXPECT_EQ(board.moves, 6.);
    EXPECT_EQ(board.en_passant, 5);
    EXPECT_EQ(board.turn, 1.);

    move top_of_stack = board.move_stack.back();

    EXPECT_EQ(top_of_stack.prev_half_move_c, 0.);
    EXPECT_EQ(top_of_stack.ep_field, 60);

    pop_move(board, session.move_list_stack[0]);

    EXPECT_EQ(get_piece_type_of_field(board, 4), 0);
    EXPECT_EQ(get_piece_type_of_field(board, 6), pPawn);
    EXPECT_EQ(board.half_moves, 0.);
    EXPECT_EQ(board.en_passant, 60);
    EXPECT_EQ(board.turn, -1.);
}

TEST(BoardTransform, PushPopPawnPromotion)
{
    move pawn_promotion_white = create_move(62, 63, 0, pQueen, 0, 0, 0);
    C_Session session = construct_session();
    C_BoardState board = init_board_state_for_test(White);
    session.board_state = board;
    uint64_t pawns = fill_bitboard_max(empty_board, {62});
    set_pieces(board, White, pPawn, pawns);

    push_move(board, pawn_promotion_white, session.move_list_stack[0]);

    EXPECT_EQ(get_piece_type_of_field(board, 62), 0);
    EXPECT_EQ(get_piece_type_of_field(board, 63), pQueen);

    pop_move(board, session.move_list_stack[0]);
    EXPECT_EQ(get_piece_type_of_field(board, 62), pPawn);
    EXPECT_EQ(get_piece_type_of_field(board, 63), 0);
}

TEST(BoardTransform, PushPopPawnPromotionCapture)
{
    move pawn_promotion_capture_black = create_move(33, 24, pBishop, pRook, 0, 0, 0);
    C_Session session = construct_session();
    C_BoardState board = init_board_state_for_test(Black);
    session.board_state = board;
    uint64_t pawns = fill_bitboard_max(empty_board, {33});
    uint64_t bishops = fill_bitboard_max(empty_board, {24});
    set_pieces(board, Black, pPawn, pawns);
    set_pieces(board, White, pBishop, bishops);

    board.en_passant = 60;
    board.moves = 5.;

    push_move(board, pawn_promotion_capture_black, session.move_list_stack[0]);

    EXPECT_EQ(get_piece_type_of_field(board, 33), 0);
    EXPECT_EQ(get_piece_type_of_field(board, 24), pRook);

    pop_move(board, session.move_list_stack[0]);
    EXPECT_EQ(get_piece_type_of_field(board, 33), pPawn);
    EXPECT_EQ(get_piece_type_of_field(board, 24), pBishop);
    EXPECT_EQ(board.en_passant, 60);
    EXPECT_EQ(board.moves, 5);
}

TEST(BoardTransform, PushPopEnPassantCapture)
{
    move en_passant_capture_white = create_move(4, 13, pPawn, 0, 0, 1);
    C_Session session = construct_session();
    C_BoardState board = init_board_state_for_test(White);
    session.board_state = board;
    board.en_passant = 13;
    board.half_moves = 20.;

    uint64_t pawns_b = fill_bitboard_max(empty_board, {12});
    uint64_t pawns_w = fill_bitboard_max(empty_board, {4});

    set_pieces(board, Black, pPawn, pawns_b);
    set_pieces(board, White, pPawn, pawns_w);

    push_move(board, en_passant_capture_white, session.move_list_stack[0]);

    EXPECT_EQ(get_piece_type_of_field(board, 4), 0);
    EXPECT_EQ(get_piece_type_of_field(board, 13), pPawn);
    EXPECT_EQ(get_piece_type_of_field(board, 12), 0);
}