#include <gtest/gtest.h>
#include "custom.hpp"
#include <array>

void print_bitboard(uint64_t bb)
{
    std::string bb_str = "";
    for (int col = 0; col < 8; col++)
    {
        for (int row = 0; row < 8; row++)
        {
            uint64_t index = 1;
            std::string bit = bb & (index << (63 - (row * 8 + col))) ? "1" : "∙";
            bb_str += bit;
            if (row != 7)
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

    EXPECT_EQ(row_col_idx_to_position_idx(col_row1[0], col_row1[1]), i1);
    EXPECT_EQ(row_col_idx_to_position_idx(col_row2[0], col_row2[1]), i2);
    EXPECT_EQ(row_col_idx_to_position_idx(col_row3[0], col_row3[1]), i3);
    EXPECT_EQ(row_col_idx_to_position_idx(col_row4[0], col_row4[1]), i4);
    EXPECT_EQ(row_col_idx_to_position_idx(col_row5[0], col_row5[1]), i5);
    EXPECT_EQ(row_col_idx_to_position_idx(col_row6[0], col_row6[1]), i6);
    EXPECT_EQ(row_col_idx_to_position_idx(col_row7[0], col_row7[1]), i7);
}

TEST(Bitboards, BoardScan)
{
    C_Board board = get_new_game_board();
    uint64_t bb1 = board.get_bishops(Black);
    uint64_t bb2 = board.get_rooks(White);
    uint64_t bb3 = board.get_pawn_double_moves(White);
    uint64_t bb4 = board.get_king_moves_and_attacks(Black);

    std::list<int> idx_list1 = scan_board(bb1);
    std::list<int> idx_list2 = scan_board(bb2);
    std::list<int> idx_list3 = scan_board(bb3);
    std::list<int> idx_list4 = scan_board(bb4);

    std::list<int> actual1 = {40, 16};
    std::list<int> actual2 = {63, 7};
    std::list<int> actual3 = {60,
                              52,
                              44,
                              36,
                              28,
                              20,
                              12,
                              4};
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
    actual1 = {40, 16};
    actual2 = {63, 7};
    actual3 = {60,
               52,
               44,
               36,
               28,
               20,
               12,
               4};
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
    EXPECT_TRUE(get_board_at_idx(b_king, row_col_idx_to_position_idx(4, 7)));
}
