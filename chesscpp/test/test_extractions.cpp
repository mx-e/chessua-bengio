#include <gtest/gtest.h>
#include "../include/extractions.hpp"

void _print_bitboard(uint64_t bb)
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

int flat(int row, int col)
{
    return row * 8 + col;
}

void fill_bitboard(uint64_t &bitboard, std::vector<std::pair<int, int> > positions)
{
    for (auto position : positions)
    {
        uint8_t idx = flat(position.first, position.second);
        bitboard |= (most_sig_bit >> idx);
    }
}

template <class T>
bool exists(std::vector<T> iterable, std::function<bool(T)> condition)
{
    bool found;
    for (auto item : iterable)
    {
        found = found || condition(item);
    }
    return found;
}

TEST(Extractions, SingleMoveWhite)
{
    uint64_t bitboard;
    fill_bitboard(bitboard, {{0, 5}, {3, 4}});

    MoveList move_list;
    extract_moves_with_offset(bitboard, move_list, -pawn_shift_map.at(single));

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(0, 4) && move.dest == flat(0, 5); }),
              true);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(3, 3) && move.dest == flat(3, 4); }),
              true);
}

TEST(Extractions, SingleMoveBlack)
{
    uint64_t bitboard;
    fill_bitboard(bitboard, {{0, 5}, {3, 4}});

    MoveList move_list;
    extract_moves_with_offset(bitboard, move_list, pawn_shift_map.at(single));

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(0, 6) && move.dest == flat(0, 5); }),
              true);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(3, 5) && move.dest == flat(3, 4); }),
              true);
}

TEST(Extractions, PromotionWhite)
{
    uint64_t bitboard;
    fill_bitboard(bitboard, {{0, 7}});

    MoveList move_list;
    extract_promotions(bitboard, move_list, -pawn_shift_map.at(single));

    for (auto promotion_piece : {pQueen, pKnight, pRook, pBishop})
    {
        EXPECT_EQ(exists<move>(
                      move_list, [&promotion_piece](move move)
                      { return move.src == flat(0, 6) && move.dest == flat(0, 7) && move.promotion == promotion_piece; }),
                  true);
    }
}

TEST(Extractions, PromotionBlack)
{
    uint64_t bitboard;
    fill_bitboard(bitboard, {{0, 0}});

    MoveList move_list;
    extract_promotions(bitboard, move_list, pawn_shift_map.at(single));

    for (auto promotion_piece : {pQueen, pKnight, pRook, pBishop})
    {
        EXPECT_EQ(exists<move>(
                      move_list, [&promotion_piece](move move)
                      { return move.src == flat(0, 1) && move.dest == flat(0, 0) && move.promotion == promotion_piece; }),
                  true);
    }
}

TEST(Extractions, EnPassantCaptureLeftWhite)
{
    uint64_t bitboard;
    fill_bitboard(bitboard, {{0, 6}});

    MoveList move_list;
    extract_ep_captures(bitboard, move_list, -pawn_shift_map.at(attack_left));

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(1, 5) && move.dest == flat(0, 6) && move.capture == pPawn; }),
              true);
}

TEST(Extractions, EnPassantCaptureRightWhite)
{
    uint64_t bitboard;
    fill_bitboard(bitboard, {{1, 6}});

    MoveList move_list;
    extract_ep_captures(bitboard, move_list, -pawn_shift_map.at(attack_right));

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(0, 5) && move.dest == flat(1, 6) && move.capture == pPawn; }),
              true);
}

TEST(Extractions, EnPassantCaptureLeftBlack)
{
    uint64_t bitboard;
    fill_bitboard(bitboard, {{0, 1}});

    MoveList move_list;
    extract_ep_captures(bitboard, move_list, pawn_shift_map.at(attack_right));

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(1, 2) && move.dest == flat(0, 1) && move.capture == pPawn; }),
              true);
}

TEST(Extractions, EnPassantCaptureRightBlack)
{
    uint64_t bitboard;
    fill_bitboard(bitboard, {{1, 1}});

    MoveList move_list;
    extract_ep_captures(bitboard, move_list, pawn_shift_map.at(attack_left));

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(0, 2) && move.dest == flat(1, 1) && move.capture == pPawn; }),
              true);
}

TEST(Extractions, ExplicitSource)
{
    uint64_t bitboard;
    fill_bitboard(bitboard, {{1, 1}, {5, 6}});

    MoveList move_list;
    extract_moves_with_explicit_src(bitboard, move_list, 7);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == 7 && move.dest == flat(1, 1); }),
              true);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == 7 && move.dest == flat(5, 6); }),
              true);
}

TEST(Extractions, CaptureWithOffset)
{
    C_BoardState board_state;
    board_state.pieces[b_knights] |= (most_sig_bit >> flat(3, 3));

    uint64_t bitboard;
    fill_bitboard(bitboard, {{3, 3}});

    MoveList move_list;
    extract_captures_with_offset(board_state, bitboard, move_list, -pawn_shift_map.at(attack_left));

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(4, 2) && move.dest == flat(3, 3) && move.capture == pKnight; }),
              true);
}
