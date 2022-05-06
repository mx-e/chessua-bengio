#include "../src/main.hpp"
#include "custom.hpp"
#include <gtest/gtest.h>

TEST(PossibleBoards, PawnOpeningWhite)
{
    Board board = get_board();
    board[0].at(1) = 6;

    BoardState boardState{board, COLOR_WHITE};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 2);

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][2] == 6; });

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][3] == 6; });
}

TEST(PossibleBoards, PawnOpeningBlack)
{
    Board board = get_board();
    board[0].at(6) = -6;

    BoardState boardState{board, COLOR_BLACK};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 2);

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][5] == -6; });

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][4] == -6; });
}

TEST(PossibleBoards, BlockedRook)
{
    Board board = get_board();
    board[0].at(0) = 5;
    board[1].at(0) = 6;
    board[0].at(3) = 6;

    BoardState boardState{board, COLOR_WHITE};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 4);
}

TEST(PossibleBoards, KingSideCastleWhite)
{
    Board board = get_board();
    board[4].at(0) = 1;
    board[7].at(0) = 5;

    BoardState boardState{board, COLOR_WHITE};
    boardState.castlingRights = CastlingRights{.white = ColorCastlingRights{false, true}};
    boardState.window = Window{4, 0, 8, 1};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 4);
    EXPECT_CASTLE_EXISTS(boardStates, KingSide, 1);
}

TEST(PossibleBoards, QueenSideCastleWhite)
{
    Board board = get_board();
    board[4].at(0) = 1;
    board[0].at(0) = 5;

    BoardState boardState{board, COLOR_WHITE};
    boardState.castlingRights = CastlingRights{.white = ColorCastlingRights{true, false}};
    boardState.window = Window{0, 0, 5, 1};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 5);
    EXPECT_CASTLE_EXISTS(boardStates, QueenSide, 1);
}

TEST(PossibleBoards, KingSideCastleBlack)
{
    Board board = get_board();
    board[4].at(7) = -1;
    board[7].at(7) = -5;

    BoardState boardState{board, COLOR_BLACK};
    boardState.castlingRights = CastlingRights{.black = ColorCastlingRights{false, true}};
    boardState.window = Window{4, 7, 8, 8};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 4);
    EXPECT_CASTLE_EXISTS(boardStates, KingSide, COLOR_BLACK);
}

TEST(PossibleBoards, QueenSideCastleBlack)
{
    Board board = get_board();
    board[4].at(7) = -1;
    board[0].at(7) = -5;

    BoardState boardState{board, COLOR_BLACK};
    boardState.castlingRights = CastlingRights{.black = ColorCastlingRights{true, false}};
    boardState.window = Window{0, 7, 5, 8};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 5);
    EXPECT_CASTLE_EXISTS(boardStates, QueenSide, COLOR_BLACK);
}

TEST(PossibleBoards, PawnCaptures)
{
    Board board = get_board();
    board[4].at(1) = 6;

    board[3].at(2) = -6;
    board[5].at(2) = -6;

    BoardState boardState{board, COLOR_WHITE};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 4);

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[3][2] == 6 && boardState.board[4][1] == 0; });

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[5][2] == 6 && boardState.board[4][1] == 0; });
}

TEST(PossibleBoards, QueenCapture)
{
    Board board = get_board();
    board[0].at(0) = 2;
    board[0].at(3) = -6;

    BoardState boardState{board, COLOR_WHITE};
    boardState.window = Window{0, 0, 1, 8};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 3);

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][3] == 2; });
}

TEST(PossibleBoards, PawnNoStraightCaptureWhite)
{
    Board board = get_board();
    board[0].at(1) = 6;
    board[1].at(1) = 6;

    board[0].at(2) = -6;
    board[1].at(3) = -6;

    BoardState boardState{board, COLOR_WHITE};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 3);
}

TEST(PossibleBoards, PawnNoStraightCaptureBlack)
{
    Board board = get_board();
    board[0].at(6) = -6;
    board[1].at(6) = -6;

    board[0].at(5) = 6;
    board[1].at(4) = 6;

    BoardState boardState{board, COLOR_BLACK};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 3);
}

TEST(PossibleBoards, EnPassantWhite)
{
    Board board = get_board();
    board[0].at(0) = 6;
    board[1].at(0) = -6;

    BoardState boardState{board, COLOR_WHITE};
    boardState.enpassant = EnPassants{{1, 1}};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 2);

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[1][1] == 6 && boardState.board[1][0] == 0; });
    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][1] == 6 && boardState.board[1][0] == -6; });
}

TEST(PossibleBoards, EnPassantBlack)
{
    Board board = get_board();
    board[0].at(3) = 6;
    board[1].at(3) = -6;

    BoardState boardState{board, COLOR_BLACK};
    boardState.enpassant = EnPassants{{0, 2}};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 2);

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][2] == -6 && boardState.board[0][3] == 0; });
    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[1][2] == -6 && boardState.board[0][3] == 6; });
}

TEST(PossibleBoards, EnPassantSetCorrectlyWhite)
{
    Board board = get_board();
    board[0].at(1) = 6;

    BoardState boardState{board, COLOR_WHITE};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 2);

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.enpassant.size() > 0 && boardState.enpassant[0].first == 0 && boardState.enpassant[0].second == 2; });
}

TEST(PossibleBoards, EnPassantSetCorrectlyBlack)
{
    Board board = get_board();
    board[0].at(6) = -6;

    BoardState boardState{board, COLOR_BLACK};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 2);

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.enpassant.size() > 0 && boardState.enpassant[0].first == 0 && boardState.enpassant[0].second == 5; });
}

TEST(PossibleBoards, PlayerChange)
{
    Board board = get_board();
    board[0].at(1) = 1;
    board[0].at(6) = -1;

    BoardState boardState{board, COLOR_WHITE};
    BoardStates boardStates = get_possible_boards(boardState);
    EXPECT_EQ(boardStates[0].color, COLOR_BLACK);

    BoardStates anotherBoardStates = get_possible_boards(boardStates[0]);
    EXPECT_EQ(anotherBoardStates[0].color, COLOR_WHITE);
}

TEST(PossibleBoards, RecomputeHalfAndFullMoves)
{
    Board board = get_board();
    board[0].at(0) = 6;
    board[1].at(3) = -6;

    BoardState boardState{board, COLOR_WHITE};
    boardState.window = Window{0, 0, 2, 4};
    BoardStates boardStates = get_possible_boards(boardState);
    EXPECT_EQ(boardStates[0].halfMove, 1);
    EXPECT_EQ(boardStates[0].fullMove, 1);

    boardStates = get_possible_boards(boardStates[0]);
    EXPECT_EQ(boardStates[0].halfMove, 2);
    EXPECT_EQ(boardStates[0].fullMove, 2);

    boardStates = get_possible_boards(boardStates[0]);
    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.halfMove == 0; });
    EXPECT_EQ(boardStates[0].fullMove, 3);
    EXPECT_EQ(boardStates.size(), 3);
}

TEST(PossibleBoards, CheckBlack)
{
    Board board = get_board();
    board[0].at(0) = 1;
    board[0].at(2) = -2;

    BoardState boardState{board, COLOR_BLACK};

    EXPECT_THROW(get_possible_boards(boardState), BoardInCheckException);
}

TEST(PossibleBoards, CheckWhite)
{
    Board board = get_board();
    board[0].at(0) = -1;
    board[0].at(2) = 2;

    BoardState boardState{board, COLOR_WHITE};

    EXPECT_THROW(get_possible_boards(boardState), BoardInCheckException);
}