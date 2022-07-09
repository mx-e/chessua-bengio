#include <gtest/gtest.h>
#include "../include/pstables.hpp"
#include "../include/conversions.hpp"
#include "test_utils.hpp"

void _update_score(
    Board board,
    Phase phase,
    float color,
    move move,
    float &score,
    float &other_score)
{

    float init_score = score;
    float init_other_score = other_score;

    C_BoardState board_state;
    marshall_board_state(
        board_state, board, color, {}, false, false, false, false, 0, 0);

    update_score(Make, phase, score, other_score, board_state, move);

    float make_score = score;
    float make_other_score = other_score;
    update_score(Unmake, phase, make_score, make_other_score, board_state, move);

    EXPECT_EQ(init_score, make_score);
    EXPECT_EQ(init_other_score, make_other_score);
}

TEST(PSTables, WhiteMoveIsBlackMove)
{
    float white_score = -30;
    float other_score = 0;
    _update_score({{{0, 0, 0, 0, 0, 0, pKing, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}},
                  OPENING,
                  White, move{.src = 6, .dest = 7}, white_score, other_score);

    float black_score = -30;
    _update_score({{{0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, -pKing, 0, 0, 0, 0, 0, 0}}},
                  OPENING,
                  Black, move{.src = 57, .dest = 56}, black_score, other_score);

    EXPECT_EQ(white_score, black_score);
}

TEST(PSTables, WhiteMoveEarly)
{
    float score = -30;
    float other_score = 0;
    _update_score({{{0, 0, 0, 0, 0, 0, pKing, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}},
                  OPENING,
                  White, move{.src = 6, .dest = 15}, score, other_score);

    EXPECT_EQ(score, -40);
    EXPECT_EQ(other_score, 0);
}

TEST(PSTables, WhiteMoveLate)
{
    float score = -30;
    float other_score = 0;
    _update_score({{{0, 0, 0, 0, 0, 0, pKing, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}},
                  ENDGAME,
                  White, move{.src = 6, .dest = 7}, score, other_score);

    EXPECT_EQ(score, -50);
    EXPECT_EQ(other_score, 0);
}

TEST(PSTables, WhiteMoveCapture)
{
    float score = -30;
    float other_score = 10;
    _update_score({{{0, 0, 0, 0, 0, 0, pKing, 0},
                    {0, 0, 0, 0, 0, 0, -pPawn, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}},
                  OPENING,
                  White, move{.src = 6, .dest = 14, .capture = pPawn}, score, other_score);

    EXPECT_EQ(score, -40);
    EXPECT_EQ(other_score, 0);
}

TEST(PSTables, WhiteMoveEnPassant)
{
    float score = 20;
    float other_score = -5;
    _update_score({{{0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, -pPawn, 0, 0},
                    {0, 0, 0, 0, 0, pPawn, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}},
                  OPENING,
                  White, move{.src = 21, .dest = 14, .ep = true}, score, other_score);

    EXPECT_EQ(score, 50);
    EXPECT_EQ(other_score, 0);
}

TEST(PSTables, WhiteMoveQueenSideCastling)
{
    float score = 20;
    float other_score = 0;
    _update_score({{{pRook, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {pKing, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}},
                  OPENING,
                  White, move{.src = 32, .dest = 16, .castling = 2}, score, other_score);

    EXPECT_EQ(score, 25);
    EXPECT_EQ(other_score, 0);
}

TEST(PSTables, BlackMoveQueenSideCastling)
{
    float score = 20;
    float other_score = 0;
    _update_score({{{0, 0, 0, 0, 0, 0, 0, -pRook},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pKing},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}},
                  OPENING,
                  Black, move{.src = 39, .dest = 23, .castling = 2}, score, other_score);

    EXPECT_EQ(score, 25);
    EXPECT_EQ(other_score, 0);
}

TEST(PSTables, WhiteMoveKingSideCastling)
{
    float score = 20;
    float other_score = 0;
    _update_score({{{0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {pKing, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {pRook, 0, 0, 0, 0, 0, 0, 0}}},
                  OPENING,
                  White, move{.src = 32, .dest = 48, .castling = 1}, score, other_score);

    EXPECT_EQ(score, 35);
    EXPECT_EQ(other_score, 0);
}

TEST(PSTables, BlackMoveKingSideCastling)
{
    float score = 20;
    float other_score = 0;
    _update_score({{{0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pKing},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pRook}}},
                  OPENING,
                  Black, move{.src = 39, .dest = 55, .castling = 1}, score, other_score);

    EXPECT_EQ(score, 35);
    EXPECT_EQ(other_score, 0);
}

TEST(PSTables, WhitePromotion)
{
    float score = 50;
    float other_score = 0;
    _update_score({{{0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, pPawn, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}},
                  OPENING,
                  White, move{.src = 30, .dest = 31, .promotion = pQueen}, score, other_score);

    EXPECT_EQ(score, -5);
    EXPECT_EQ(other_score, 0);
}

TEST(PSTables, BlackPromotion)
{
    float score = 50;
    float other_score = 0;
    _update_score({{{0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, -pPawn, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}},
                  OPENING,
                  Black, move{.src = 25, .dest = 24, .promotion = pQueen}, score, other_score);

    EXPECT_EQ(score, -5);
    EXPECT_EQ(other_score, 0);
}

TEST(PSTables, WhitePromotionCapture)
{
    float score = 50;
    float other_score = -10;
    _update_score({{{0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pQueen},
                    {0, 0, 0, 0, 0, 0, pPawn, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}},
                  OPENING,
                  White, move{.src = 30, .dest = 23, .promotion = pQueen, .capture = true}, score, other_score);

    EXPECT_EQ(score, -10);
    EXPECT_EQ(other_score, 0);
}

TEST(PSTables, BlackPromotionCapture)
{
    float score = 50;
    float other_score = -10;
    _update_score({{{0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {pQueen, 0, 0, 0, 0, 0, 0, 0},
                    {0, -pPawn, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}},
                  OPENING,
                  Black, move{.src = 25, .dest = 16, .promotion = pQueen, .capture = true}, score, other_score);

    EXPECT_EQ(score, -10);
    EXPECT_EQ(other_score, 0);
}