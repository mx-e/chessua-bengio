#include <gtest/gtest.h>
#include "../include/pstables.hpp"
#include "../include/conversions.hpp"
#include "test_utils.hpp"

void _update_score(
    Board board,
    GamePhase phase,
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

    board_state.phase = phase;

    update_score(Make, score, other_score, board_state, move);

    float make_score = score;
    float make_other_score = other_score;
    update_score(Unmake, make_score, make_other_score, board_state, move);

    EXPECT_EQ(init_score, make_score);
    EXPECT_EQ(init_other_score, make_other_score);
}

TEST(PSTables, WhiteMoveIsBlackMove)
{
    float white_score = -40;
    float other_score = 0;
    _update_score({{{0, 0, 0, 0, 0, 0, pKing, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}},
                  EarlyGame,
                  White, move{.src = 6, .dest = 7}, white_score, other_score);

    float black_score = -40;
    _update_score({{{0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, -pKing, 0, 0, 0, 0, 0, 0}}},
                  EarlyGame,
                  Black, move{.src = 57, .dest = 56}, black_score, other_score);

    EXPECT_EQ(white_score, black_score);
}

TEST(PSTables, WhiteMoveEarly)
{
    float score = -40;
    float other_score = 0;
    _update_score({{{0, 0, 0, 0, 0, 0, pKing, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}},
                  EarlyGame,
                  White, move{.src = 6, .dest = 7}, score, other_score);

    EXPECT_EQ(score, -30);
    EXPECT_EQ(other_score, 0);
}

TEST(PSTables, WhiteMoveLate)
{
    float score = -40;
    float other_score = 0;
    _update_score({{{0, 0, 0, 0, 0, 0, pKing, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}},
                  LateGame,
                  White, move{.src = 6, .dest = 7}, score, other_score);

    EXPECT_EQ(score, -50);
    EXPECT_EQ(other_score, 0);
}

TEST(PSTables, WhiteMoveCapture)
{
    float score = -40;
    float other_score = 0;
    _update_score({{{0, 0, 0, 0, 0, 0, pKing, -pPawn},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}},
                  LateGame,
                  White, move{.src = 6, .dest = 7, .capture = pPawn}, score, other_score);

    EXPECT_EQ(score, -50);
    EXPECT_EQ(other_score, 0);
}