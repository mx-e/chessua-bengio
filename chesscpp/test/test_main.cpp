#include <gtest/gtest.h>
#include "test_utils.hpp"
#include "../include/main.hpp"

TEST(Main, UCICastlingKingCaptureInTransit)
{
    C_BoardState board_state = init_board_state_for_test(White);
    board_state.half_moves = 0.;
    board_state.moves = 1.;

    set_pieces(board_state, White, pRook, fill_bitboard({{0, 0}, {7, 0}}));
    set_pieces(board_state, White, pPawn, fill_bitboard({{0, 1}}));
    set_pieces(board_state, White, pBishop, fill_bitboard({{4, 1}}));
    set_pieces(board_state, White, pKnight, fill_bitboard({{5, 1}}));

    set_pieces(board_state, Black, pRook, fill_bitboard({{3, 6}, {0, 7}}));
    set_pieces(board_state, Black, pQueen, fill_bitboard({{4, 7}}));
    set_pieces(board_state, Black, pBishop, fill_bitboard({{7, 3}}));
    set_pieces(board_state, Black, pPawn, fill_bitboard({{0, 2}}));

    UCIStrings uci_strings = get_uci_moves(board_state);

    EXPECT_EQ(exists<std::string>(
                  uci_strings, [](std::string uci)
                  { return uci == "e1c1"; }),
              false);

    EXPECT_EQ(exists<std::string>(
                  uci_strings, [](std::string uci)
                  { return uci == "a1b1"; }),
              true);
}

TEST(Bestmove, Capture)
{
    Board board = {{{pKing, 0, 0, 0, 0, 0, 0, -pKing},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, pBishop, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, -pRook, 0, 0}}};
    std::string value = bestmove_benchmark(4, board, 1., {}, false, false, false, false, 0, 0);

    EXPECT_EQ("d2h6", value);
}

TEST(Bestmove, Checkmate)
{
    Board board = {{{0, pRook, 0, 0, 0, 0, 0, pKing},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {pRook, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pKing}}};
    std::string value = bestmove_benchmark(2, board, 1., {}, false, false, false, false, 0, 0);

    EXPECT_EQ(value, "a2h2");
}

TEST(Bestmove, CaptureSituationDepth1)
{
    // This test is intended for use WITHOUT quiesence search and ZW Search
    Board board = {{{0, 0, 0, 0, 0, 0, pKing, 0},
                    {0, 0, 0, pRook, 0, 0, 0, 0},
                    {0, pKnight, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {-pQueen, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, -pPawn, 0, 0, 0, 0, -pPawn, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pKing}}};
    std::string value = bestmove_benchmark(1, board, White, {}, false, false, false, false, 0, 0);

    EXPECT_EQ(value, "c2e1");
}

TEST(Bestmove, CaptureSituationDepth2)
{
    // This test is intended for use WITHOUT quiesence search and ZW Search
    Board board = {{{0, pRook, 0, 0, -pKnight, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, -pPawn, 0},
                    {0, 0, 0, 0, 0, 0, -pQueen, 0},
                    {0, pRook, 0, 0, -pPawn, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {pKing, 0, 0, 0, 0, 0, -pPawn, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pKing}}};
    std::string value = bestmove_benchmark(2, board, White, {}, false, false, false, false, 0, 0);

    EXPECT_EQ(value, "d2d5");
}

TEST(Bestmove, CaptureSituationDepth3)
{
    // This test is intended for use WITHOUT quiesence search and ZW Search
    Board board = {{{0, pRook, 0, 0, -pKnight, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, -pPawn, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, pRook, 0, 0, -pPawn, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {pKing, 0, 0, pBishop, 0, -pPawn, -pPawn, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pKing}}};
    std::string value = bestmove_benchmark(4, board, White, {}, false, false, false, false, 0, 0);

    EXPECT_EQ(value, "g4e6");
}

TEST(Bestmove, SituationBlackMatesIn3)
{
    // Source: https://www.chess.com/forum/view/more-puzzles/black-mates-in-3
    Board board = {{{0, pKing, 0, pPawn, -pPawn, 0, 0, 0},
                    {0, pPawn, pPawn, -pPawn, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {pKnight, 0, 0, 0, 0, 0, 0, 0},
                    {0, -pPawn, 0, -pKing, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, -pPawn, 0, 0, 0, -pBishop, -pBishop, 0}}};
    std::string value = bestmove_benchmark(5, board, Black, {}, false, false, false, false, 0, 0);

    EXPECT_EQ(value, "e2d1q");
}

TEST(BestMoveMateIn2, TaraschKurschner1893)
{
    // https://www.sparkchess.com/chess-puzzles/siegbert-tarrash-vs-max-kurschner.html
    Board board = {{{pRook, 0, 0, -pPawn, 0, 0, -pPawn, -pRook},
                    {pKnight, 0, 0, 0, 0, -pKnight, -pBishop, 0},
                    {0, pBishop, pPawn, -pPawn, pBishop, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pQueen},
                    {pRook, 0, 0, 0, 0, pPawn, 0, -pKing},
                    {0, pPawn, 0, 0, pQueen, -pBishop, 0, 0},
                    {pKing, pPawn, 0, 0, 0, 0, -pPawn, 0},
                    {0, pPawn, 0, 0, 0, 0, -pPawn, -pRook}}};
    std::string value = bestmove_benchmark(4, board, 1., {}, false, false, false, false, 0, 0);

    EXPECT_EQ("f5g6", value);
}


TEST(BestMoveMateIn2, MasonMarko1894)
{

    // https://www.sparkchess.com/chess-puzzles/james-mason-vs-georg-marco.html
    Board board = {{{0, 0, pPawn, 0, 0, 0, -pPawn, 0},
                    {0, pPawn, 0, 0, 0, 0, -pPawn, 0},
                    {pBishop, 0, 0, 0, -pBishop, -pPawn, 0, 0},
                    {pRook, 0, 0, 0, -pPawn, 0, 0, 0},
                    {pRook, 0, 0, 0, 0, 0, 0, 0},
                    {pKnight, -pRook, 0, 0, 0, 0, 0, 0},
                    {0, -pRook, pPawn, 0, 0, 0, -pPawn, -pKing},
                    {pKing, pPawn, -pBishop, 0, 0, 0, 0, 0}}};
    std::string value = bestmove_benchmark(4, board, -1., {}, false, false, false, false, 0, 0);

    EXPECT_EQ("g2g1", value);
}

TEST(BestMoveMateIn2, PaoliFoltys1949)
{

    // https://www.sparkchess.com/chess-puzzles/enrico-paoli-vs-jan-foltys.html
    Board board = {{{pRook, 0, 0, 0, pPawn, 0, 0, 0},
                    {0, 0, pPawn, -pPawn, 0, 0, 0, 0},
                    {0, 0, 0, 0, -pPawn, -pBishop, -pKing, 0},
                    {0, 0, pKing, 0, 0, 0, 0, 0},
                    {pRook, 0, 0, 0, pKnight, 0, 0, 0},
                    {0, -pRook, 0, -pBishop, -pPawn, 0, -pPawn, 0},
                    {0, 0, 0, 0, 0, -pPawn, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}};
    std::string value = bestmove_benchmark(4, board, -1., {}, false, false, false, false, 0, 0);

    EXPECT_EQ("c6b5", value);
}

TEST(BestMoveMateIn2, VidmarEuwe1929)
{

    // https://www.sparkchess.com/chess-puzzles/milan-vidmar-vs-max-euwe.html
    Board board = {{{0, pPawn, pQueen, 0, -pPawn, 0, 0, 0},
                    {0, pPawn, 0, 0, 0, -pPawn, 0, 0},
                    {0, -pRook, 0, 0, 0, 0, 0, 0},
                    {pRook, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, -pQueen, pKnight, 0, -pPawn, 0},
                    {0, 0, 0, 0, -pPawn, 0, 0, -pKing},
                    {pKing, 0, pPawn, 0, 0, -pPawn, 0, 0}}};
    std::string value = bestmove_benchmark(4, board, 1., {}, false, false, false, false, 0, 0);

    EXPECT_EQ("a3f8", value);
}

TEST(BestMoveMateIn3, BernsteinKotov1946)
{
    // Source: https://www.sparkchess.com/chess-puzzles/ossip-bernstein-vs-alexander-kotov.html
    Board board = {{{0, 0, 0, 0, 0, 0, 0, pRook},
                    {0, pPawn, 0, -pRook, 0, 0, -pRook, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, pQueen, 0, 0, -pPawn, 0, 0, 0},
                    {0, 0, 0, -pQueen, pPawn, -pPawn, 0, 0},
                    {0, 0, 0, pPawn, 0, 0, -pPawn, 0},
                    {0, 0, 0, pPawn, 0, -pKing, -pPawn, 0},
                    {0, pKing, pPawn, 0, 0, -pPawn, 0, pRook}}};
    std::string value = bestmove_benchmark(6, board, White, {}, false, false, false, false, 0, 0);

    EXPECT_EQ(value, "Wf4f5");
}

TEST(BestMoveMateIn3, ZukertortPotter1875)
{
    // https://www.sparkchess.com/chess-puzzles/johannes-zukertort-vs-william-norwood-potter.html
    Board board = {{{0, -pQueen, 0, 0, 0, 0, -pPawn, 0},
                    {0, pPawn, 0, 0, 0, 0, pRook, 0},
                    {0, 0, pBishop, 0, 0, -pPawn, 0, 0},
                    {0, 0, 0, 0, 0, -pKing, 0, -pRook},
                    {pRook, 0, 0, 0, 0, -pBishop, pKnight, 0},
                    {0, pPawn, 0, 0, 0, 0, 0, 0},
                    {pKing, pPawn, 0, 0, 0, 0, 0, 0},
                    {0, pPawn, 0, 0, 0, 0, 0, 0}}};
    std::string value = bestmove_benchmark(6, board, White, {}, false, false, false, false, 0, 0);

    EXPECT_EQ(value, "Wc3e5");
}

TEST(Bestmove, QuiesenceSearch1)
{
    Board board = {{{0, 0, pRook, -pRook, 0, 0, pQueen, pKing},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {pKnight, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, -pPawn, 0, 0, 0, 0, -pPawn, 0},
                    {0, 0, -pBishop, 0, 0, 0, 0, -pKing}}};

    C_Session session = construct_session(5);
    marshall_board_state(session.board_state, board, Black, {}, false, false, false, false, 0, 0);
    collect_legal_moves(session.board_state, session.move_list_stack[0]);
    float score = quiescence_search(session, -infty, infty, 0);

    EXPECT_EQ(score, -3.0);
}

TEST(Bestmove, QuiensenceSearch2)
{
    Board board = {{{0, 0, pRook, 0, 0, 0, pQueen, pKing},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {pKnight, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, -pPawn, 0},
                    {0, 0, -pBishop, 0, 0, 0, 0, -pKing}}};

    C_Session session = construct_session(5);
    marshall_board_state(session.board_state, board, Black, {}, false, false, false, false, 0, 0);
    collect_legal_moves(session.board_state, session.move_list_stack[0]);

    float score = quiescence_search(session, -infty, infty, 0);

    EXPECT_EQ(score, -13);
}

TEST(Bestmove, QuiensenceSearch3)
{
    Board board = {{{0, 0, pRook, -pRook, 0, 0, pQueen, pKing},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, pKnight, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, -pPawn, 0},
                    {0, 0, -pBishop, 0, 0, 0, 0, -pKing}}};

    C_Session session = construct_session(5);
    marshall_board_state(session.board_state, board, White, {}, false, false, false, false, 0, 0);
    collect_legal_moves(session.board_state, session.move_list_stack[0]);
    float score = quiescence_search(session, -infty, infty, 0);

    EXPECT_EQ(score, 13);
}

TEST(Bestmove, Evaluation1)
{
    Board board = {{{0, 0, pRook, 0, 0, 0, pQueen, pKing},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {pKnight, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, -pPawn, 0},
                    {0, 0, -pBishop, 0, 0, 0, 0, -pKing}}};

    C_Session session = construct_session(5);
    marshall_board_state(session.board_state, board, Black, {}, false, false, false, false, 0, 0);
    float score = evaluate(session.board_state);

    EXPECT_EQ(score, -13);
}
