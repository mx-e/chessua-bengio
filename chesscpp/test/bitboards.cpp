#include <gtest/gtest.h>
#include "custom.hpp"
#include <array>

void print_bitboard(uint64_t bb)
{
    std::string bb_str = "";
    for (int row = 0; row < 8; row++)
    {
        for (int col = 7; col >= 0; col--)
        {
            uint64_t index = 1;
            std::string bit = bb & (index << (col * 8 + row)) ? "1" : "∙";
            bb_str += bit;
            if (col != 0)
            {
                bb_str += " ";
            }
        }
        bb_str += '\n';
    }
    bb_str += '\n';
    std::cout << bb_str;
}

Board get_example_board()
{
    Board board = Board();
    board[0] = {5, 6, 0, 0, 0, 0, -6, -5};
    board[1] = {4, 6, 0, 0, 0, 0, -6, -4};
    board[2] = {3, 6, 0, 0, 0, 0, -6, -3};
    board[3] = {2, 6, 0, 0, 0, 0, -6, -2};
    board[4] = {1, 6, 0, 0, 0, 0, -6, -1};
    board[5] = {3, 6, 0, 0, 0, 0, -6, -3};
    board[6] = {4, 6, 0, 0, 0, 0, -6, -4};
    board[7] = {5, 6, 0, 0, 0, 0, -6, -5};
    return board;
}

C_Board get_new_game_board()
{
    C_Board board = C_Board();
    board.set_pieces(White, pPawn, white_pawns_starting_config);
    board.set_pieces(Black, pPawn, black_pawns_starting_config);
    board.set_pieces(White, pRook, white_rooks_starting_config);
    board.set_pieces(Black, pRook, black_rooks_starting_config);
    board.set_pieces(White, pKnight, white_knights_starting_config);
    board.set_pieces(Black, pKnight, black_knights_starting_config);
    board.set_pieces(White, pBishop, white_bishops_starting_config);
    board.set_pieces(Black, pBishop, black_bishops_starting_config);
    board.set_pieces(White, pQueen, white_queen_starting_config);
    board.set_pieces(Black, pQueen, black_queen_starting_config);
    board.set_pieces(White, pKing, white_king_starting_config);
    board.set_pieces(Black, pKing, black_king_starting_config);
    return board;
}

TEST(Bitboards, FieldIdxToRowColConversion)
{
    int i1 = 0;
    int i2 = 23;
    int i3 = 21;
    int i4 = 63;
    int i5 = 17;
    int i6 = 51;
    int i7 = 33;

    std::array<uint8_t, 2> col_row1 = position_idx_to_col_row_idx(i1);
    std::array<uint8_t, 2> col_row2 = position_idx_to_col_row_idx(i2);
    std::array<uint8_t, 2> col_row3 = position_idx_to_col_row_idx(i3);
    std::array<uint8_t, 2> col_row4 = position_idx_to_col_row_idx(i4);
    std::array<uint8_t, 2> col_row5 = position_idx_to_col_row_idx(i5);
    std::array<uint8_t, 2> col_row6 = position_idx_to_col_row_idx(i6);
    std::array<uint8_t, 2> col_row7 = position_idx_to_col_row_idx(i7);

    EXPECT_EQ((int)col_row1[0], 0);
    EXPECT_EQ((int)col_row1[1], 0);

    EXPECT_EQ((int)col_row2[0], 2);
    EXPECT_EQ((int)col_row2[1], 7);

    EXPECT_EQ((int)col_row3[0], 2);
    EXPECT_EQ((int)col_row3[1], 5);

    EXPECT_EQ((int)col_row4[0], 7);
    EXPECT_EQ((int)col_row4[1], 7);

    EXPECT_EQ((int)col_row5[0], 2);
    EXPECT_EQ((int)col_row5[1], 1);

    EXPECT_EQ((int)col_row6[0], 6);
    EXPECT_EQ((int)col_row6[1], 3);

    EXPECT_EQ((int)col_row7[0], 4);
    EXPECT_EQ((int)col_row7[1], 1);

    EXPECT_EQ(col_row_idx_to_position_idx(col_row1[0], col_row1[1]), i1);
    EXPECT_EQ(col_row_idx_to_position_idx(col_row2[0], col_row2[1]), i2);
    EXPECT_EQ(col_row_idx_to_position_idx(col_row3[0], col_row3[1]), i3);
    EXPECT_EQ(col_row_idx_to_position_idx(col_row4[0], col_row4[1]), i4);
    EXPECT_EQ(col_row_idx_to_position_idx(col_row5[0], col_row5[1]), i5);
    EXPECT_EQ(col_row_idx_to_position_idx(col_row6[0], col_row6[1]), i6);
    EXPECT_EQ(col_row_idx_to_position_idx(col_row7[0], col_row7[1]), i7);
}

TEST(Bitboards, BoardScan)
{
    C_Board board = get_new_game_board();
    uint64_t bb1 = board.get_bishops(Black);
    uint64_t bb2 = board.get_rooks(White);
    uint64_t bb3 = board.get_pawn_double_moves(board.get_pawn_single_moves(White), White);
    uint64_t bb4 = board.get_king_moves(col_row_idx_to_position_idx(4, 7));

    std::list<int> idx_list1 = scan_board(bb1);
    std::list<int> idx_list2 = scan_board(bb2);
    std::list<int> idx_list3 = scan_board(bb3);
    std::list<int> idx_list4 = scan_board(bb4);

    std::list<int> actual1 = {47, 23};
    std::list<int> actual2 = {56, 0};
    std::list<int> actual3 = {59,
                              51,
                              43,
                              35,
                              27,
                              19,
                              11,
                              3};
    std::list<int> actual4 = {};

    ASSERT_EQ(idx_list1.size(), actual1.size());
    ASSERT_EQ(idx_list2.size(), actual2.size());
    ASSERT_EQ(idx_list3.size(), actual3.size());
    ASSERT_EQ(idx_list4.size(), actual4.size());

    print_bitboard(bb1);
    while (actual1.size() > 0)
    {
        ASSERT_EQ(idx_list1.front(), actual1.front());
        idx_list1.pop_front();
        actual1.pop_front();
    }
    print_bitboard(bb2);
    while (actual2.size() > 0)
    {
        ASSERT_EQ(idx_list2.front(), actual2.front());
        idx_list2.pop_front();
        actual2.pop_front();
    }
    print_bitboard(bb3);
    while (actual3.size() > 0)
    {
        ASSERT_EQ(idx_list3.front(), actual3.front());
        idx_list3.pop_front();
        actual3.pop_front();
    }
    print_bitboard(bb4);
    while (actual4.size() > 0)
    {
        ASSERT_EQ(idx_list4.front(), actual4.front());
        idx_list4.pop_front();
        actual4.pop_front();
    }

    idx_list1 = scan_board_reverse(bb1);
    idx_list2 = scan_board_reverse(bb2);
    idx_list3 = scan_board_reverse(bb3);
    idx_list4 = scan_board_reverse(bb4);
    actual1 = {47, 23};
    actual2 = {56, 0};
    actual3 = {59,
               51,
               43,
               35,
               27,
               19,
               11,
               3};
    actual4 = {};

    ASSERT_EQ(idx_list1.size(), actual1.size());
    ASSERT_EQ(idx_list2.size(), actual2.size());
    ASSERT_EQ(idx_list3.size(), actual3.size());
    ASSERT_EQ(idx_list4.size(), actual4.size());

    print_bitboard(bb1);
    while (actual1.size() > 0)
    {
        ASSERT_EQ(idx_list1.back(), actual1.front());
        idx_list1.pop_back();
        actual1.pop_front();
    }
    print_bitboard(bb2);
    while (actual2.size() > 0)
    {
        ASSERT_EQ(idx_list2.back(), actual2.front());
        idx_list2.pop_back();
        actual2.pop_front();
    }
    print_bitboard(bb3);
    while (actual3.size() > 0)
    {
        ASSERT_EQ(idx_list3.back(), actual3.front());
        idx_list3.pop_back();
        actual3.pop_front();
    }
    print_bitboard(bb4);
    while (actual4.size() > 0)
    {
        ASSERT_EQ(idx_list4.back(), actual4.front());
        idx_list4.pop_back();
        actual4.pop_front();
    }
}

TEST(Bitboard, GetPieceTypeOfField)
{
    C_Board board = mailbox_to_bitboard_representation(get_example_board());
    EXPECT_EQ(board.get_pawns(1.), white_pawns_starting_config);

    int ex1 = get_piece_type_of_field(&board, 0);
    int ex2 = get_piece_type_of_field(&board, 7);
    int ex3 = get_piece_type_of_field(&board, 9);
    int ex4 = get_piece_type_of_field(&board, 39);

    EXPECT_EQ(ex1, pRook);
    EXPECT_EQ(ex2, pRook);
    EXPECT_EQ(ex3, pPawn);
    EXPECT_EQ(ex4, pKing);
}

TEST(Bitboard, GetSetIndividualBit)
{
    uint64_t ex1 = row_1;
    uint64_t ex2 = col_d;
    uint64_t ex3 = edge_half;
    uint64_t ex4 = row_5;

    EXPECT_TRUE(get_board_at_idx(ex1, 0));
    EXPECT_TRUE(get_board_at_idx(ex2, 31));
    EXPECT_TRUE(get_board_at_idx(ex3, 7));
    EXPECT_TRUE(get_board_at_idx(ex4, 44));

    EXPECT_FALSE(get_board_at_idx(ex1, 1));

    uint64_t ex1_b = set_board_1_at_idx(ex1, 1);
    EXPECT_TRUE(get_board_at_idx(ex1_b, 1));

    uint64_t ex1_c = set_board_0_at_idx(ex1_b, 1);
    EXPECT_FALSE(get_board_at_idx(ex1_c, 1));
}

TEST(Bitboard, MailboxImport)
{
    C_Board board = mailbox_to_bitboard_representation(get_example_board());
    uint64_t pawns = board.get_pawns(board.turn);
    EXPECT_TRUE(get_board_at_idx(pawns, 1));
    EXPECT_TRUE(get_board_at_idx(pawns, 49));

    uint64_t b_king = board.get_king(Black);
    EXPECT_TRUE(get_board_at_idx(b_king, col_row_idx_to_position_idx(4, 7)));
}

TEST(Bitboard, PushPopMoves)
{
    C_Board board = mailbox_to_bitboard_representation(get_example_board());
    board.collect_legal_moves();

    // first move
    auto legal_moves = board.legal_moves;
    move m = create_move(0, 0);
    for (move mv : legal_moves)
    {
        if (mv.src == 8 && mv.dest == 18)
        {
            m = mv;
        }
    }
    EXPECT_EQ((int)m.dest, 18);
    EXPECT_EQ((int)m.capture, 0);
    EXPECT_EQ((int)m.flag, 0);

    board.push_move(m);
    uint8_t dest_piece_type = get_piece_type_of_field(&board, m.dest);
    uint8_t src_piece_type = get_piece_type_of_field(&board, m.src);
    EXPECT_EQ((int)dest_piece_type, (int)pKnight);
    EXPECT_EQ((int)src_piece_type, 0);
    EXPECT_EQ((int)m.prev_half_move_c, 0);
    EXPECT_EQ((int)board.half_moves, 1);
    EXPECT_EQ((int)board.moves, 0);
    EXPECT_EQ((int)board.turn, -1);

    // second_move
    board.collect_legal_moves();
    legal_moves = board.legal_moves;

    for (move mv : legal_moves)
    {

        if (mv.src == col_row_idx_to_position_idx(3, 6) && mv.dest == 28)
        {
            m = mv;
        }
    }
    EXPECT_EQ((int)m.dest, 28);
    EXPECT_EQ((int)m.capture, 0);
    EXPECT_EQ((int)m.flag, 0);

    board.push_move(m);
    dest_piece_type = get_piece_type_of_field(&board, m.dest);
    src_piece_type = get_piece_type_of_field(&board, m.src);

    EXPECT_EQ((int)dest_piece_type, (int)pPawn);
    EXPECT_EQ((int)src_piece_type, 0);
    EXPECT_EQ((int)board.move_stack.top().prev_half_move_c, 1);
    EXPECT_EQ((int)board.half_moves, 0);
    EXPECT_EQ((int)board.moves, 1);
    EXPECT_EQ((int)board.turn, 1);

    // third_move (capture)
    board.collect_legal_moves();
    legal_moves = board.legal_moves;

    for (move mv : legal_moves)
    {
        if (mv.capture != 0)
        {
            m = mv;
        }
    }
    EXPECT_EQ((int)m.dest, 28);
    EXPECT_EQ((int)m.src, 18);
    EXPECT_EQ((int)m.capture, 6);
    EXPECT_EQ((int)m.flag, 0);

    board.push_move(m);
    dest_piece_type = get_piece_type_of_field(&board, m.dest);
    src_piece_type = get_piece_type_of_field(&board, m.src);
    EXPECT_EQ((int)dest_piece_type, (int)pKnight);
    EXPECT_EQ((int)src_piece_type, 0);
    EXPECT_EQ((int)board.move_stack.top().prev_half_move_c, 0);
    EXPECT_EQ((int)board.half_moves, 0);
    EXPECT_EQ((int)board.moves, 1);
    EXPECT_EQ((int)board.turn, -1);

    // rolling back capture
    m = board.pop_move();
    dest_piece_type = get_piece_type_of_field(&board, m.dest);
    src_piece_type = get_piece_type_of_field(&board, m.src);
    EXPECT_EQ((int)dest_piece_type, (int)pPawn);
    EXPECT_TRUE((bool)get_board_at_idx(board.pieces[b_black], m.dest));
    EXPECT_EQ((int)src_piece_type, pKnight);
    EXPECT_EQ((int)board.move_stack.top().prev_half_move_c, 1);
    EXPECT_EQ((int)board.half_moves, 0);
    EXPECT_EQ((int)board.moves, 1);
    EXPECT_EQ((int)board.turn, 1);

    // pawn promotion and rollback
    C_Board promo_board = C_Board();
    promo_board.set_single_piece(White, pPawn, 6);
    promo_board.collect_legal_moves();
    legal_moves = promo_board.legal_moves;

    // TODO

    // pawn promotion capture and rollback
    // TODO
    // EXPECT_EQ(false, true);
}
