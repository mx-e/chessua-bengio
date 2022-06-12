//#include "custom.hpp"
#include <gtest/gtest.h>

<<<<<<< HEAD:chesscpp/archive/possible_boards.cpp
/*TEST(PossibleBoards, PawnOpeningWhite)
=======
// TEST(BestMove, Rooks)
// {
//     Board board{{{0, 0, 0, 0, 0, 0, 0, -King::ID},
//                  {Rook::ID, 0, 0, 0, 0, 0, 0, 0},
//                  {0, Rook::ID, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0}}};

//     BoardState boardState{board, COLOR_WHITE};
//     std::string move = bestmove(boardState, 1, true);

//     EXPECT_EQ(move, "c2a2");
// }

// TEST(BestMove, QueensLikeRooks)
// {
//     Board board{{{0, 0, 0, 0, 0, 0, 0, -King::ID},
//                  {Queen::ID, 0, 0, 0, 0, 0, 0, 0},
//                  {0, Queen::ID, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0}}};

//     BoardState boardState{board, COLOR_WHITE};
//     std::string move = bestmove(boardState, 1, true);

//     EXPECT_EQ(move, "c2a2");
// }

// TEST(BestMove, Bishops)
// {
//     Board board{{{0, Rook::ID, 0, 0, 0, Bishop::ID, Bishop::ID, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {-King::ID, 0, 0, 0, 0, 0, 0, 0}}};

//     BoardState boardState{board, COLOR_WHITE};
//     std::string move = bestmove(boardState, 1, true);

//     EXPECT_EQ(move, "a6b7");
// }
// TEST(BestMove, KingOfTheHill)
// {
//     Board board{{{0, Rook::ID, 0, 0, 0, Bishop::ID, Bishop::ID, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, -King::ID, 0, Rook::ID},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0},
//                  {0, 0, 0, 0, 0, 0, 0, 0}}};

//     BoardState boardState{board, COLOR_BLACK};
//     std::string move = bestmove(boardState, 1, true);

//     EXPECT_EQ(move, "e6d5");
// }

TEST(Evaluation, Classic)
{
    Board board{{{Queen::ID, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, King::ID, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0}}};

    int score = evaluate(board);
    EXPECT_EQ(19, score);
}

TEST(PossibleBoards, PawnOpeningWhite)
>>>>>>> edd2cfb (Add bestmove and evaluate function):chesscpp/test/main.cpp
{
    Board board{};
    board[0].at(1) = 6;

    BoardState boardState{board, COLOR_WHITE};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 2);

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][2] == 6; });

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][3] == 6; });
}

TEST(PossibleBoards, PawnOpeningWhiteBlocked)
{
    Board board{};
    board[0].at(1) = 6;
    board[0].at(2) = -6;

    BoardState boardState{board, COLOR_WHITE};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 0);
}

TEST(PossibleBoards, PawnOpeningBlack)
{
    Board board{};
    board[0].at(6) = -6;

    BoardState boardState{board, COLOR_BLACK};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 2);

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][5] == -6; });

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][4] == -6; });
}

TEST(PossibleBoards, PawnOpeningBlackBlocked)
{
    Board board{};
    board[0].at(6) = -6;
    board[0].at(5) = 6;

    BoardState boardState{board, COLOR_BLACK};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 0);
}

TEST(PossibleBoards, BlockedRook)
{
    Board board{};
    board[0].at(0) = 5;
    board[1].at(0) = 6;
    board[0].at(3) = 6;

    BoardState boardState{board, COLOR_WHITE};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 4);
}

BoardStates get_castling_scenario_black(Board board, ColorCastlingRights rights)
{
    BoardState boardState{board, COLOR_BLACK};
    boardState.castlingRights = CastlingRights{.black = rights};
    boardState.window = rights.queenSide ? Window{0, 7, 5, 8} : Window{4, 7, 8, 8};
    return get_possible_boards(boardState);
}

BoardStates get_castling_scenario_white(Board board, ColorCastlingRights rights)
{
    BoardState boardState{board, COLOR_WHITE};
    boardState.castlingRights = CastlingRights{.white = rights};
    boardState.window = rights.queenSide ? Window{0, 0, 5, 1} : Window{4, 0, 8, 1};
    return get_possible_boards(boardState);
}

TEST(PossibleBoards, KingSideCastleWhite)
{
    Board board{};
    board[4].at(0) = 1;
    board[7].at(0) = 5;

    BoardStates boardStates = get_castling_scenario_white(board, ColorCastlingRights{false, true});

    EXPECT_EQ(boardStates.size(), 4);
    EXPECT_CASTLE_EXISTS(boardStates, KingSide, 1);

    EXPECT_FORALL(boardStates, [](BoardState boardState)
                  { return !boardState.castlingRights.white.kingSide; });
}

TEST(PossibleBoards, QueenSideCastleWhite)
{
    Board board{};
    board[4].at(0) = 1;
    board[0].at(0) = 5;

    BoardStates boardStates = get_castling_scenario_white(board, ColorCastlingRights{true, false});

    EXPECT_EQ(boardStates.size(), 5);
    EXPECT_CASTLE_EXISTS(boardStates, QueenSide, 1);

    EXPECT_FORALL(boardStates, [](BoardState boardState)
                  { return !boardState.castlingRights.white.queenSide; });
}

TEST(PossibleBoards, KingSideCastleBlack)
{
    Board board{};
    board[4].at(7) = -1;
    board[7].at(7) = -5;

    BoardStates boardStates = get_castling_scenario_black(board, ColorCastlingRights{false, true});

    EXPECT_EQ(boardStates.size(), 4);
    EXPECT_CASTLE_EXISTS(boardStates, KingSide, COLOR_BLACK);

    EXPECT_FORALL(boardStates, [](BoardState boardState)
                  { return !boardState.castlingRights.black.kingSide; });
}

TEST(PossibleBoards, QueenSideCastleBlack)
{
    Board board{};
    board[4].at(7) = -1;
    board[0].at(7) = -5;

    BoardStates boardStates = get_castling_scenario_black(board, ColorCastlingRights{true, false});

    EXPECT_EQ(boardStates.size(), 5);
    EXPECT_CASTLE_EXISTS(boardStates, QueenSide, COLOR_BLACK);

    EXPECT_FORALL(boardStates, [](BoardState boardState)
                  { return !boardState.castlingRights.black.queenSide; });
}

TEST(PossibleBoards, KingSideCastleWhiteBlocked)
{
    Board board{};
    board[4].at(0) = 1;
    board[7].at(0) = 5;
    board[6].at(0) = -4;

    BoardStates boardStates = get_castling_scenario_white(board, ColorCastlingRights{false, true});

    EXPECT_EQ(boardStates.size(), 2);
}

TEST(PossibleBoards, QueenSideCastleWhiteBlocked)
{
    Board board{};
    board[4].at(0) = 1;
    board[0].at(0) = 5;
    board[3].at(0) = -4;

    BoardStates boardStates = get_castling_scenario_white(board, ColorCastlingRights{true, false});

    EXPECT_EQ(boardStates.size(), 4);
}

TEST(PossibleBoards, KingSideCastleBlackBlocked)
{
    Board board{};
    board[4].at(7) = -1;
    board[7].at(7) = -5;
    board[5].at(7) = 4;

    BoardStates boardStates = get_castling_scenario_black(board, ColorCastlingRights{false, true});

    EXPECT_EQ(boardStates.size(), 3);
}

TEST(PossibleBoards, QueenSideCastleBlackBlocked)
{
    Board board{};
    board[4].at(7) = -1;
    board[0].at(7) = -5;
    board[3].at(7) = 4;

    BoardStates boardStates = get_castling_scenario_black(board, ColorCastlingRights{true, false});
    EXPECT_EQ(boardStates.size(), 4);
}

TEST(PossibleBoards, PawnCaptures)
{
    Board board{};
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
    Board board{};
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
    Board board{};
    board[0].at(1) = 6;
    board[1].at(1) = 6;

    board[0].at(2) = -6;
    board[1].at(3) = -6;

    BoardState boardState{board, COLOR_WHITE};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 2);
}

TEST(PossibleBoards, PawnNoStraightCaptureBlack)
{
    Board board{};
    board[0].at(6) = -6;
    board[1].at(6) = -6;

    board[0].at(5) = 6;
    board[1].at(4) = 6;

    BoardState boardState{board, COLOR_BLACK};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 2);
}

TEST(PossibleBoards, EnPassantWhite)
{
    Board board{};
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
    Board board{};
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
    Board board{};
    board[0].at(1) = 6;

    BoardState boardState{board, COLOR_WHITE};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 2);

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.enpassant.size() > 0 && boardState.enpassant[0].first == 0 && boardState.enpassant[0].second == 2; });
}

TEST(PossibleBoards, EnPassantSetCorrectlyBlack)
{
    Board board{};
    board[0].at(6) = -6;

    BoardState boardState{board, COLOR_BLACK};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 2);

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.enpassant.size() > 0 && boardState.enpassant[0].first == 0 && boardState.enpassant[0].second == 5; });
}

TEST(PossibleBoards, PlayerChange)
{
    Board board{};
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
    Board board{};
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
    Board board{};
    board[0].at(0) = 1;
    board[0].at(2) = -2;

    BoardState boardState{board, COLOR_BLACK};

    EXPECT_THROW(get_possible_boards(boardState), BoardInCheckException);
}

TEST(PossibleBoards, CheckWhite)
{
    Board board{};
    board[0].at(0) = -1;
    board[0].at(2) = 2;

    BoardState boardState{board, COLOR_WHITE};

    EXPECT_THROW(get_possible_boards(boardState), BoardInCheckException);
}

TEST(PossibleBoards, PawnSwapWhite)
{
    Board board{};
    board[0].at(6) = 6;

    BoardState boardState{board, COLOR_WHITE};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 4);

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][7] == 2 && boardState.board[0][6] == 0; });
    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][7] == 3 && boardState.board[0][6] == 0; });
    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][7] == 4 && boardState.board[0][6] == 0; });
    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][7] == 5 && boardState.board[0][6] == 0; });
}

TEST(PossibleBoards, PawnSwapBlack)
{
    Board board{};
    board[0].at(1) = -6;

    BoardState boardState{board, COLOR_BLACK};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EQ(boardStates.size(), 4);

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][0] == -2 && boardState.board[0][1] == 0; });
    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][0] == -3 && boardState.board[0][1] == 0; });
    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][0] == -4 && boardState.board[0][1] == 0; });
    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.board[0][0] == -5 && boardState.board[0][1] == 0; });
}

TEST(PossibleBoards, UCIStringPawnMove)
{
    Board board{};
    board[3].at(2) = 6;

    BoardState boardState{board, COLOR_WHITE};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.uci == "d3d4"; });
}

TEST(PossibleBoards, UCIStringQueenMove)
{
    Board board{};
    board[3].at(2) = 2;

    BoardState boardState{board, COLOR_WHITE};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.uci == "d3g6"; });
}

TEST(PossibleBoards, UCIStringSwap)
{
    Board board{};
    board[3].at(6) = 6;

    BoardState boardState{board, COLOR_WHITE};
    BoardStates boardStates = get_possible_boards(boardState);

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.uci == "d7d8q"; });

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.uci == "d7d8b"; });

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.uci == "d7d8n"; });

    EXPECT_EXISTS(boardStates, [](BoardState boardState)
                  { return boardState.uci == "d7d8r"; });
}
*/