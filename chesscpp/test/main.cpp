#include "../src/main.hpp"
#include "custom.hpp"
#include <gtest/gtest.h>

TEST(PossibleBoards, PawnOpeningWhite)
{
    Board board = get_board();
    board[0].at(1) = 6;

    BoardState boardState{.board = board, .color = COLOR_WHITE};
    Boards boards = get_possible_boards(boardState);

    EXPECT_EQ(boards.size(), 2);

    EXPECT_EXISTS(boards, [](Board board)
                  { return board[0][2] == 6; });

    EXPECT_EXISTS(boards, [](Board board)
                  { return board[0][3] == 6; });
}

TEST(PossibleBoards, PawnOpeningBlack)
{
    Board board = get_board();
    board[0].at(6) = -6;

    BoardState boardState{.board = board, .color = COLOR_BLACK};
    Boards boards = get_possible_boards(boardState);

    EXPECT_EQ(boards.size(), 2);

    EXPECT_EXISTS(boards, [](Board board)
                  { return board[0][5] == -6; });

    EXPECT_EXISTS(boards, [](Board board)
                  { return board[0][4] == -6; });
}

TEST(PossibleBoards, BlockedRook)
{
    Board board = get_board();
    board[0].at(0) = 5;
    board[1].at(0) = 6;
    board[0].at(3) = 6;

    BoardState boardState{board, 1, NULL, NULL, 0, 0};
    Boards boards = get_possible_boards(boardState);

    EXPECT_EQ(boards.size(), 4);
}

TEST(PossibleBoards, KingSideCastleWhite)
{
    std::optional<Window> window = Window{4, 0, 8, 1};

    Board board = get_board();
    board[4].at(0) = 1;
    board[7].at(0) = 5;

    ColorCastlingRights kingSide{false, true};
    CastlingRights rights{.white = kingSide};

    BoardState boardState{.board = board, .color = 1, .castlingRights = rights, .window = window};
    Boards boards = get_possible_boards(boardState);

    EXPECT_EQ(boards.size(), 4);
    EXPECT_CASTLE_EXISTS(boards, KingSide, 1);
}

TEST(PossibleBoards, QueenSideCastleWhite)
{
    std::optional<Window> window = Window{0, 0, 5, 1};

    Board board = get_board();
    board[4].at(0) = 1;
    board[0].at(0) = 5;

    ColorCastlingRights queenSide{true, false};
    CastlingRights rights{.white = queenSide};

    BoardState boardState{.board = board, .color = 1, .castlingRights = rights, .window = window};
    Boards boards = get_possible_boards(boardState);

    EXPECT_EQ(boards.size(), 5);
    EXPECT_CASTLE_EXISTS(boards, QueenSide, 1);
}

TEST(PossibleBoards, KingSideCastleBlack)
{
    std::optional<Window> window = Window{4, 7, 8, 8};

    Board board = get_board();
    board[4].at(7) = -1;
    board[7].at(7) = -5;

    ColorCastlingRights kingSide{false, true};
    CastlingRights rights{.black = kingSide};

    BoardState boardState{.board = board, .color = COLOR_BLACK, .castlingRights = rights, .window = window};
    Boards boards = get_possible_boards(boardState);

    EXPECT_EQ(boards.size(), 4);
    EXPECT_CASTLE_EXISTS(boards, KingSide, COLOR_BLACK);
}

TEST(PossibleBoards, QueenSideCastleBlack)
{
    std::optional<Window> window = Window{0, 7, 5, 8};

    Board board = get_board();
    board[4].at(7) = -1;
    board[0].at(7) = -5;

    ColorCastlingRights queenSide{true, false};
    CastlingRights rights{.black = queenSide};

    BoardState boardState{.board = board, .color = COLOR_BLACK, .castlingRights = rights, .window = window};
    Boards boards = get_possible_boards(boardState);

    EXPECT_EQ(boards.size(), 5);
    EXPECT_CASTLE_EXISTS(boards, QueenSide, COLOR_BLACK);
}

TEST(PossibleBoards, PawnCaptures)
{
    Board board = get_board();
    board[4].at(1) = 6;

    board[3].at(2) = -6;
    board[5].at(2) = -6;

    BoardState boardState{.board = board, .color = COLOR_WHITE};
    Boards boards = get_possible_boards(boardState);

    EXPECT_EQ(boards.size(), 4);

    EXPECT_EXISTS(boards, [](Board board)
                  { return board[3][2] == 6 && board[4][1] == 0; });

    EXPECT_EXISTS(boards, [](Board board)
                  { return board[5][2] == 6 && board[4][1] == 0; });
}

TEST(PossibleBoards, QueenCapture)
{
    std::optional<Window> window = Window{0, 0, 1, 8};
    Board board = get_board();
    board[0].at(0) = 2;
    board[0].at(3) = -6;

    BoardState boardState{.board = board, .color = COLOR_WHITE, .window = window};
    Boards boards = get_possible_boards(boardState);

    EXPECT_EQ(boards.size(), 3);

    EXPECT_EXISTS(boards, [](Board board)
                  { return board[0][3] == 2; });
}

TEST(PossibleBoards, EnPassantWhite)
{
    Board board = get_board();
    board[0].at(0) = 6;
    board[1].at(0) = -6;

    std::optional<EnPassants> enPassants = EnPassants{ {1, 1} };
    BoardState boardState{.board = board, .enpassant = enPassants, .color = COLOR_WHITE};
    Boards boards = get_possible_boards(boardState);

    EXPECT_EQ(boards.size(), 2);

    EXPECT_EXISTS(boards, [](Board board)
                  { return board[1][1] == 6 && board[1][0] == 0; });
    EXPECT_EXISTS(boards, [](Board board)
                  { return board[0][1] == 6 && board[1][0] == -6; });
}

TEST(PossibleBoards, EnPassantBlack)
{
    Board board = get_board();
    board[0].at(3) = 6;
    board[1].at(3) = -6;

    std::optional<EnPassants> enPassants = EnPassants{ {0, 2} };
    BoardState boardState{.board = board, .enpassant = enPassants, .color = COLOR_BLACK};
    Boards boards = get_possible_boards(boardState);

    EXPECT_EQ(boards.size(), 2);

    EXPECT_EXISTS(boards, [](Board board)
                  { return board[0][2] == -6 && board[0][3] == 0; });
    EXPECT_EXISTS(boards, [](Board board)
                  { return board[1][2] == -6 && board[0][3] == 6; });
}

/**
 * @brief TODOS
 * Implement check-check
 * Document and Test everything
 */