#include "../src/main.hpp"
#include "custom.hpp"
#include <gtest/gtest.h>

TEST(PossibleBoards, Knight) {
    Board board = get_board();
    board[0].at(0) = 2;

    BoardState boardState{ board, 1, NULL, NULL, 0, 0};
    Boards boards = get_possible_boards(boardState);

    EXPECT_GE(boards.size(), 2);
}

TEST(PossibleBoards, BlockedRook) {
    Board board = get_board();
    board[0].at(0) = 5;
    board[1].at(0) = 6;
    board[0].at(3) = 6;

    BoardState boardState{ board, 1, NULL, NULL, 0, 0};
    Boards boards = get_possible_boards(boardState);

    EXPECT_EQ(boards.size(), 4);
}

TEST(PossibleBoards, KingSideCastleWhite) {
    std::optional<Window> window = Window{ 4, 0, 8, 1};

    Board board = get_board();
    board[4].at(0) = 1;
    board[7].at(0) = 5;

    ColorCastlingRights kingSide{ false, true };
    CastlingRights rights{ kingSide, NULL };
    EnPassants enPassants;
    BoardState boardState{ board, 1, rights, enPassants, 0, 0, window};
    Boards boards = get_possible_boards(boardState);

    EXPECT_EQ(boards.size(), 4);
    EXPECT_CASTLE_EXISTS(boards, KingSide, 1);
}

TEST(PossibleBoards, QueenSideCastleWhite) {
    std::optional<Window> window = Window{ 0, 0, 5, 1};

    Board board = get_board();
    board[4].at(0) = 1;
    board[0].at(0) = 5;

    ColorCastlingRights queenSide{ true, false };
    CastlingRights rights{ queenSide, NULL };
    EnPassants enPassants;
    BoardState boardState{ board, 1, rights, enPassants, 0, 0, window};
    Boards boards = get_possible_boards(boardState);

    EXPECT_EQ(boards.size(), 5);
    EXPECT_CASTLE_EXISTS(boards, QueenSide, 1);
}

/**
 * @brief TODOS
 * Implement check-check
 * Implement captures as (conditional) moves
 * Implement enpassant
 * Document and Test everything
 */