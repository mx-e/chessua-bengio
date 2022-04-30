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
    board[1].at(0) = -6;
    board[0].at(3) = -6;

    BoardState boardState{ board, 1, NULL, NULL, 0, 0};
    Boards boards = get_possible_boards(boardState);

    EXPECT_EQ(boards.size(), 2);
}

TEST(PossibleBoards, KingSideCastleWhite) {
    Board board = get_board();
    board[4].at(0) = 1;
    board[7].at(0) = 5;

    fill_column_with_pawns(board, 1, -1);
    fill_row_with_pawns(board, 3, -1);

    ColorCastlingRights kingSide{ false, true };
    CastlingRights rights{ kingSide, NULL };
    EnPassants enPassants;
    BoardState boardState{ board, 1, rights, enPassants, 0, 0};
    Boards boards = get_possible_boards(boardState);

    // Rook up by two, King one down and one Castle
    EXPECT_EQ(boards.size(), 4);
    EXPECT_CASTLE_EXISTS(boards, KingSide, 1);
}

TEST(PossibleBoards, QueenSideCastleWhite) {
    Board board = get_board();
    board[4].at(0) = 1;
    board[0].at(0) = 5;

    fill_column_with_pawns(board, 1, -1);
    fill_row_with_pawns(board, 5, -1);

    ColorCastlingRights queenSide{ true, false };
    CastlingRights rights{ queenSide, NULL };
    EnPassants enPassants;
    BoardState boardState{ board, 1, rights, enPassants, 0, 0};
    Boards boards = get_possible_boards(boardState);

    // Rook down by two, King one up and one Castle
    EXPECT_EQ(boards.size(), 5);
    EXPECT_CASTLE_EXISTS(boards, QueenSide, 1);
}