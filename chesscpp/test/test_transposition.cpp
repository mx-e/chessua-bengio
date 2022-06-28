#include <gtest/gtest.h>
#include "../include/transposition.hpp"
#include "../include/conversions.hpp"

TEST(ZobristHashing, InitHashState)
{
    HashState hash_state;
    construct_hash(hash_state);

    for(int i = 0; i < FEATURES; i++)
    {
        EXPECT_NE(hash_state.features[i], 0);
    }
}

TEST(ZobristHashing, ComputeHashEqualForSameBoard)
{
    HashState hash_state;
    construct_hash(hash_state);

    C_BoardState board_state;
    Board board = {{{0, 0, 0, 0, 0, 0, pKing, 0},
                    {0, 0, 0, pRook, 0, 0, 0, 0},
                    {0, pKnight, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {-pQueen, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, -pPawn, 0, 0, 0, 0, -pPawn, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pKing}}};
    marshall_board_state(
        board_state, board, White, {}, false, false, false, false, 0, 0);

    compute_hash(hash_state, board_state);
    u_int64_t hash = hash_state.hash;
    compute_hash(hash_state, board_state);

    EXPECT_EQ(hash_state.hash, hash);
}

TEST(ZobristHashing, ComputeHashUnequalForDifferentBoard)
{
    HashState hash_state;
    construct_hash(hash_state);

    C_BoardState board_state1;
    Board board1 = {{{0, 0, 0, 0, 0, pKing, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}};
    marshall_board_state(
        board_state1, board1, White, {}, false, false, false, false, 0, 0);

    C_BoardState board_state2;
    Board board2 = {{{0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}};
    marshall_board_state(
        board_state2, board2, White, {}, false, false, false, false, 0, 0);

    compute_hash(hash_state, board_state1);
    u_int64_t hash = hash_state.hash;
    compute_hash(hash_state, board_state2);

    EXPECT_NE(hash_state.hash, hash);
}

TEST(ZobristHashing, ComputeHashUnequalForDifferentToMove)
{
    HashState hash_state;
    construct_hash(hash_state);

    C_BoardState board_state1;
    Board board1 = {{{0, 0, 0, 0, 0, pKing, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}};
    marshall_board_state(
        board_state1, board1, White, {}, false, false, false, false, 0, 0);

    C_BoardState board_state2;
    Board board2 = {{{0, 0, 0, 0, 0, pKing, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}};
    marshall_board_state(
        board_state2, board2, Black, {}, false, false, false, false, 0, 0);

    compute_hash(hash_state, board_state1);
    u_int64_t hash = hash_state.hash;
    compute_hash(hash_state, board_state2);

    EXPECT_NE(hash_state.hash, hash);
}

TEST(ZobristHashing, UpdateHashBasicMove)
{
    HashState hash_state;
    construct_hash(hash_state);

    C_BoardState board_state1;
    Board board1 = {{{0, 0, 0, 0, 0, pKing, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}};
    marshall_board_state(
        board_state1, board1, White, {}, false, false, false, false, 0, 0);

    C_BoardState board_state2;
    Board board2 = {{{0, 0, 0, 0, 0, 0, pKing, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}};
    marshall_board_state(
        board_state2, board2, White, {}, false, false, false, false, 0, 0);
    compute_hash(hash_state, board_state1);

    move move{.src = 5, .dest = 6};
    update_hash(hash_state, board_state1, move);
    u_int64_t hash = hash_state.hash;

    HashState hash_state2;
    construct_hash(hash_state2);
    compute_hash(hash_state2, board_state2);

    EXPECT_EQ(hash_state.hash, hash_state2.hash);
}

TEST(ZobristHashing, UpdateHashCapture)
{
    HashState hash_state;
    construct_hash(hash_state);

    C_BoardState board_state1;
    Board board1 = {{{0, 0, 0, 0, 0, pKing, -pPawn, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}};
    marshall_board_state(
        board_state1, board1, White, {}, false, false, false, false, 0, 0);

    C_BoardState board_state2;
    Board board2 = {{{0, 0, 0, 0, 0, 0, pKing, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}};
    marshall_board_state(
        board_state2, board2, White, {}, false, false, false, false, 0, 0);
    compute_hash(hash_state, board_state1);

    move move{.src = 5, .dest = 6, .capture = pPawn};
    update_hash(hash_state, board_state1, move);
    u_int64_t hash = hash_state.hash;

    HashState hash_state2;
    construct_hash(hash_state2);
    compute_hash(hash_state2, board_state2);

    EXPECT_EQ(hash_state.hash, hash_state2.hash);
}

TEST(ZobristHashing, UpdateHashEnPassantCapture)
{
    HashState hash_state;
    construct_hash(hash_state);

    C_BoardState board_state1;
    Board board1 = {{{0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, -pPawn, 0, 0},
                    {0, 0, 0, 0, 0, pPawn, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}};
    marshall_board_state(
        board_state1, board1, White, {}, false, false, false, false, 0, 0);

    C_BoardState board_state2;
    Board board2 = {{{0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, pPawn, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}};
    marshall_board_state(
        board_state2, board2, White, {}, false, false, false, false, 0, 0);
    compute_hash(hash_state, board_state1);

    move move{.src = 21, .dest = 14, .ep = true};
    update_hash(hash_state, board_state1, move);
    u_int64_t hash = hash_state.hash;

    HashState hash_state2;
    construct_hash(hash_state2);
    compute_hash(hash_state2, board_state2);

    EXPECT_EQ(hash_state.hash, hash_state2.hash);
}

TEST(ZobristHashing, UpdateHashPromotionCapture)
{
    HashState hash_state;
    construct_hash(hash_state);

    C_BoardState board_state1;
    Board board1 = {{{0, 0, 0, 0, 0, 0, pPawn, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pRook},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}};
    marshall_board_state(
        board_state1, board1, White, {}, false, false, false, false, 0, 0);

    C_BoardState board_state2;
    Board board2 = {{{0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, pQueen},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}};
    marshall_board_state(
        board_state2, board2, White, {}, false, false, false, false, 0, 0);
    compute_hash(hash_state, board_state1);

    move move{.src = 6, .dest = 15, .capture = pRook, .promotion = pQueen};
    update_hash(hash_state, board_state1, move);
    u_int64_t hash = hash_state.hash;

    HashState hash_state2;
    construct_hash(hash_state2);
    compute_hash(hash_state2, board_state2);

    EXPECT_EQ(hash_state.hash, hash_state2.hash);
}