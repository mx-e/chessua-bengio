#include <gtest/gtest.h>
#include "../include/transposition.hpp"
#include "../include/conversions.hpp"
#include <optional>

u_int64_t hash_4_board(Board board, float color, std::optional<move> move)
{
    HashState hash_state;
    construct_hash(hash_state);

    C_BoardState board_state;
    marshall_board_state(
        board_state, board, color, {}, false, false, false, false, 0, 0);

    compute_hash(hash_state, board_state);

    if (move)
        update_hash(hash_state, board_state, *move);
    return hash_state.hash;
}

TEST(ZobristHashing, InitHashState)
{
    HashState hash_state;
    construct_hash(hash_state);

    for (int i = 0; i < FEATURES; i++)
    {
        EXPECT_NE(hash_state.features[i], 0);
    }
}

TEST(ZobristHashing, ComputeHashEqualForSameBoard)
{
    for (auto color : {White, Black})
    {
        Board board = {{{0, 0, 0, 0, 0, 0, pKing, 0},
                        {0, 0, 0, pRook, 0, 0, 0, 0},
                        {0, pKnight, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {-pQueen, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {0, -pPawn, 0, 0, 0, 0, -pPawn, 0},
                        {0, 0, 0, 0, 0, 0, 0, -pKing}}};

        EXPECT_EQ(
            hash_4_board(board, color, {}),
            hash_4_board(board, color, {}));
    }
}

TEST(ZobristHashing, ComputeHashUnequalForDifferentBoard)
{
    for (auto color : {White, Black})
    {
        EXPECT_NE(
            hash_4_board({{{0, 0, 0, 0, 0, pKing, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, -pKing, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0}}},
                         color, {}),

            hash_4_board({{{0, 0, 0, 0, 0, 0, pKing, 0},
                           {0, pPawn, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 0, -pPawn, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0}}},
                         color, {}));
    }
}

TEST(ZobristHashing, ComputeHashUnequalForDifferentToMove)
{
    EXPECT_NE(
        hash_4_board({{{0, 0, 0, 0, 0, pKing, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     White, {}),

        hash_4_board({{{0, 0, 0, 0, 0, 0, pKing, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     Black, {}));
}

TEST(ZobristHashing, UpdateHashBasicMoveWhite)
{
    EXPECT_EQ(
        hash_4_board({{{0, 0, 0, 0, 0, pKing, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     White, move{.src = 5, .dest = 6}),

        hash_4_board({{{0, 0, 0, 0, 0, 0, pKing, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     White, {}));
}

TEST(ZobristHashing, UpdateHashBasicMoveBlack)
{
    EXPECT_EQ(
        hash_4_board({{{0, 0, 0, 0, 0, -pKing, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     Black, move{.src = 5, .dest = 6}),

        hash_4_board({{{0, 0, 0, 0, 0, 0, -pKing, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     Black, {}));
}

TEST(ZobristHashing, UpdateHashCaptureWhite)
{
    EXPECT_EQ(
        hash_4_board({{{0, 0, 0, 0, 0, pKing, -pPawn, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     White, move{.src = 5, .dest = 6, .capture = pPawn}),

        hash_4_board({{{0, 0, 0, 0, 0, 0, pKing, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     White, {}));
}

TEST(ZobristHashing, UpdateHashCaptureBlack)
{
    EXPECT_EQ(
        hash_4_board({{{0, pPawn, -pKing, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     Black, move{.src = 2, .dest = 1, .capture = pPawn}),

        hash_4_board({{{0, -pKing, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     Black, {}));
}

TEST(ZobristHashing, UpdateHashEnPassantCaptureWhite)
{
    EXPECT_EQ(
        hash_4_board({{{0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, -pPawn, 0, 0},
                       {0, 0, 0, 0, 0, pPawn, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     White, move{.src = 21, .dest = 14, .ep = true}),

        hash_4_board({{{0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, pPawn, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     White, {}));
}

TEST(ZobristHashing, UpdateHashEnPassantCapture)
{
    EXPECT_EQ(
        hash_4_board({{{0, 0, -pPawn, 0, 0, 0, 0, 0},
                       {0, 0, pPawn, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     Black, move{.src = 2, .dest = 9, .ep = true}),

        hash_4_board({{{0, 0, 0, 0, 0, 0, 0, 0},
                       {0, -pPawn, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     Black, {}));
}

TEST(ZobristHashing, UpdateHashPromotionCaptureWhite)
{
    EXPECT_EQ(
        hash_4_board({{{0, 0, 0, 0, 0, 0, pPawn, 0},
                       {0, 0, 0, 0, 0, 0, 0, -pRook},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     White, move{.src = 6, .dest = 15, .capture = pRook, .promotion = pQueen}),

        hash_4_board({{{0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, pQueen},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     White, {}));
}

TEST(ZobristHashing, UpdateHashPromotionCaptureBlack)
{
    EXPECT_EQ(
        hash_4_board({{{0, -pPawn, 0, 0, 0, 0, 0, 0},
                       {pRook, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     Black, move{.src = 1, .dest = 8, .capture = pRook, .promotion = pQueen}),

        hash_4_board({{{0, 0, 0, 0, 0, 0, 0, 0},
                       {-pQueen, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0}}},
                     Black, {}));
}