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
    auto [value, _] = bestmove_benchmark(4, board, 1., {}, false, false, false, false, 0, 0);

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
    auto [value, _] = bestmove_benchmark(2, board, 1., {}, false, false, false, false, 0, 0);

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
    auto [value, _] = bestmove_benchmark(1, board, White, {}, false, false, false, false, 0, 0);

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
    auto [value, _] = bestmove_benchmark(2, board, White, {}, false, false, false, false, 0, 0);

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
    auto [value, _] = bestmove_benchmark(4, board, White, {}, false, false, false, false, 0, 0);

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
    auto [value, _] = bestmove_benchmark(6, board, Black, {}, false, false, false, false, 0, 0);

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
    auto [value, _] = bestmove_benchmark(4, board, 1., {}, false, false, false, false, 0, 0);

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
    auto [value, _] = bestmove_benchmark(4, board, -1., {}, false, false, false, false, 0, 0);

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
    auto [value, _] = bestmove_benchmark(4, board, -1., {}, false, false, false, false, 0, 0);

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
    auto [value, _] = bestmove_benchmark(4, board, 1., {}, false, false, false, false, 0, 0);

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
    auto [value, _] = bestmove_benchmark(6, board, White, {}, false, false, false, false, 0, 0);

    EXPECT_EQ(value, "f4f5");
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
    auto [value, _] = bestmove_benchmark(6, board, White, {}, false, false, false, false, 0, 0);

    EXPECT_EQ(value, "c3e5");
}

// broken because of new evaluation function
//  TEST(Bestmove, QuiesenceSearch1)
//  {
//      Board board = {{{0, 0, pRook, -pRook, 0, 0, pQueen, pKing},
//                      {0, 0, 0, 0, 0, 0, 0, 0},
//                      {0, 0, 0, 0, 0, 0, 0, 0},
//                      {0, 0, 0, 0, 0, 0, 0, 0},
//                      {pKnight, 0, 0, 0, 0, 0, 0, 0},
//                      {0, 0, 0, 0, 0, 0, 0, 0},
//                      {0, -pPawn, 0, 0, 0, 0, -pPawn, 0},
//                      {0, 0, -pBishop, 0, 0, 0, 0, -pKing}}};

//     C_Session session = construct_session(5);
//     marshall_board_state(session.board_state, board, Black, {}, false, false, false, false, 0, 0);
//     collect_legal_moves(session.board_state, session.move_list_stack[0]);
//     float score = quiescence_search(session, -infty, infty, 0);

//     EXPECT_EQ(score, -3.0);
// }

// TEST(Bestmove, QuiensenceSearch2)
// {
//     Board board = {{{0, 0, pRook, 0, 0, 0, pQueen, pKing},
//                     {0, 0, 0, 0, 0, 0, 0, 0},
//                     {0, 0, 0, 0, 0, 0, 0, 0},
//                     {0, 0, 0, 0, 0, 0, 0, 0},
//                     {pKnight, 0, 0, 0, 0, 0, 0, 0},
//                     {0, 0, 0, 0, 0, 0, 0, 0},
//                     {0, 0, 0, 0, 0, 0, -pPawn, 0},
//                     {0, 0, -pBishop, 0, 0, 0, 0, -pKing}}};

//     C_Session session = construct_session(5);
//     marshall_board_state(session.board_state, board, Black, {}, false, false, false, false, 0, 0);
//     collect_legal_moves(session.board_state, session.move_list_stack[0]);

//     float score = quiescence_search(session, -infty, infty, 0);

//     EXPECT_EQ(score, -13);
// }

// TEST(Bestmove, QuiensenceSearch3)
// {
//     Board board = {{{0, 0, pRook, -pRook, 0, 0, pQueen, pKing},
//                     {0, 0, 0, 0, 0, 0, 0, 0},
//                     {0, pKnight, 0, 0, 0, 0, 0, 0},
//                     {0, 0, 0, 0, 0, 0, 0, 0},
//                     {0, 0, 0, 0, 0, 0, 0, 0},
//                     {0, 0, 0, 0, 0, 0, 0, 0},
//                     {0, 0, 0, 0, 0, 0, -pPawn, 0},
//                     {0, 0, -pBishop, 0, 0, 0, 0, -pKing}}};

//     C_Session session = construct_session(5);
//     marshall_board_state(session.board_state, board, White, {}, false, false, false, false, 0, 0);
//     collect_legal_moves(session.board_state, session.move_list_stack[0]);
//     float score = quiescence_search(session, -infty, infty, 0);

//     EXPECT_EQ(score, 13);
// }

// TEST(Bestmove, Evaluation1)
// {
//     Board board = {{{0, 0, pRook, 0, 0, 0, pQueen, pKing},
//                     {0, 0, 0, 0, 0, 0, 0, 0},
//                     {0, 0, 0, 0, 0, 0, 0, 0},
//                     {0, 0, 0, 0, 0, 0, 0, 0},
//                     {pKnight, 0, 0, 0, 0, 0, 0, 0},
//                     {0, 0, 0, 0, 0, 0, 0, 0},
//                     {0, 0, 0, 0, 0, 0, -pPawn, 0},
//                     {0, 0, -pBishop, 0, 0, 0, 0, -pKing}}};

//     C_Session session = construct_session(5);
//     marshall_board_state(session.board_state, board, Black, {}, false, false, false, false, 0, 0);
//     float score = evaluate(session.board_state);

//     EXPECT_EQ(score, -13);
// }

TEST(MoveGen, SpeedTest)
{
    Board board = {{{0, 0, 0, 0, 0, 0, 0, pRook},
                    {0, pPawn, 0, -pRook, 0, 0, -pRook, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, pQueen, 0, 0, -pPawn, 0, 0, 0},
                    {0, 0, 0, -pQueen, pPawn, -pPawn, 0, 0},
                    {0, 0, 0, pPawn, 0, 0, -pPawn, 0},
                    {0, 0, 0, pPawn, 0, -pKing, -pPawn, 0},
                    {0, pKing, pPawn, 0, 0, -pPawn, 0, pRook}}};
    C_Session session = construct_session(5);
    marshall_board_state(session.board_state, board, Black, {}, false, false, false, false, 0, 0);
    for (int i = 0; i < 10000000; ++i)
    {
        collect_legal_moves(session.board_state, session.move_list_stack[0]);
        session.move_list_stack[0].clear();
    }
    EXPECT_EQ(true, true);
}

TEST(MoveGen, SpeedTestPushPop)
{
    Board board = {{{0, 0, 0, 0, 0, 0, 0, pRook},
                    {0, pPawn, 0, -pRook, 0, 0, -pRook, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, pQueen, 0, 0, -pPawn, 0, 0, 0},
                    {0, 0, 0, -pQueen, pPawn, -pPawn, 0, 0},
                    {0, 0, 0, pPawn, 0, 0, -pPawn, 0},
                    {0, 0, 0, pPawn, 0, -pKing, -pPawn, 0},
                    {0, pKing, pPawn, 0, 0, -pPawn, 0, pRook}}};
    C_Session session = construct_session(5);
    marshall_board_state(session.board_state, board, Black, {}, false, false, false, false, 0, 0);
    collect_legal_moves(session.board_state, session.move_list_stack[0]);

    for (int i = 0; i < 10000000; ++i)
    {
        int move_no = i % 20;
        move m = session.move_list_stack[0][move_no];
        push_move(session.board_state, m, session.move_list_stack[1]);
        pop_move(session.board_state);
        session.move_list_stack[1].clear();
    }
    EXPECT_EQ(true, true);
}

TEST(MoveGen, SpeedTestEval)
{
    Board board = {{{0, 0, 0, 0, 0, 0, 0, pRook},
                    {0, pPawn, 0, -pRook, 0, 0, -pRook, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, pQueen, 0, 0, -pPawn, 0, 0, 0},
                    {0, 0, 0, -pQueen, pPawn, -pPawn, 0, 0},
                    {0, 0, 0, pPawn, 0, 0, -pPawn, 0},
                    {0, 0, 0, pPawn, 0, -pKing, -pPawn, 0},
                    {0, pKing, pPawn, 0, 0, -pPawn, 0, pRook}}};
    C_Session session = construct_session(5);
    marshall_board_state(session.board_state, board, Black, {}, false, false, false, false, 0, 0);
    collect_legal_moves(session.board_state, session.move_list_stack[0]);

    for (int i = 0; i < 10000000; ++i)
    {
        evaluate(session.board_state);
    }
    EXPECT_EQ(true, true);
}

TEST(Evaluations, Material)
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
    Scores score = {0., 0.};
    evaluate_material(session.board_state, score);

    EXPECT_EQ(score[OPENING],
              material_values[OPENING][pRook] + material_values[OPENING][pQueen] + material_values[OPENING][pKnight] + material_values[OPENING][pKing] - material_values[OPENING][pPawn] - material_values[OPENING][pBishop] - material_values[OPENING][pKing]);

    EXPECT_EQ(score[ENDGAME],
              material_values[ENDGAME][pRook] + material_values[ENDGAME][pQueen] + material_values[ENDGAME][pKnight] + material_values[ENDGAME][pKing] - material_values[ENDGAME][pPawn] - material_values[ENDGAME][pBishop] - material_values[ENDGAME][pKing]);
}

TEST(Evaluations, Mobility)
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
    marshall_board_state(session.board_state, board, White, {}, false, false, false, false, 0, 0);
    Scores score = {0., 0.};

    int n_rook_moves_w = get_rook_move_count(session.board_state, White);
    EXPECT_EQ(n_rook_moves_w, 1);

    int n_queen_moves_w = get_queen_move_count(session.board_state, White);
    EXPECT_EQ(n_queen_moves_w, 1);

    int n_pawn_moves_b = get_pawn_move_count(session.board_state, Black);
    EXPECT_EQ(n_pawn_moves_b, 2);

    int n_bishop_moves_black = get_bishop_move_count(session.board_state, Black);
    EXPECT_EQ(n_bishop_moves_black, 0);

    int n_king_moves_black = get_king_move_count(session.board_state, Black);
    EXPECT_EQ(n_king_moves_black, 0);

    int n_king_moves_white = get_king_move_count(session.board_state, White);
    EXPECT_EQ(n_king_moves_white, 0);

    evaluate_mobility(session.board_state, score);

    EXPECT_EQ(score[OPENING], mobility_bonus[OPENING][pRook] + mobility_bonus[OPENING][pQueen] - mobility_bonus[OPENING][pPawn] * 2);
    EXPECT_EQ(score[ENDGAME], mobility_bonus[ENDGAME][pRook] + mobility_bonus[ENDGAME][pQueen] - mobility_bonus[ENDGAME][pPawn] * 2);
}

TEST(Evaluations, DoubledPawns)
{
    Board board = {{{0, 0, pPawn, 0, 0, 0, pPawn, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, pQueen, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pPawn},
                    {pPawn, 0, 0, pPawn, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, pRook, 0, 0, 0, 0, -pPawn, 0},
                    {0, 0, -pPawn, 0, 0, 0, 0, -pPawn}}};

    C_Session session = construct_session(5);
    marshall_board_state(session.board_state, board, White, {}, false, false, false, false, 0, 0);
    int w_d_pawns = doubled_pawns(session.board_state, White);
    int b_d_pawns = doubled_pawns(session.board_state, Black);

    EXPECT_EQ(w_d_pawns, 2);
    EXPECT_EQ(b_d_pawns, 1);
}

TEST(Evaluations, PassedPawns)
{
    Board board = {{{0, 0, pPawn, 0, 0, 0, pPawn, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, pQueen, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pPawn},
                    {pPawn, 0, 0, pPawn, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, pRook, 0, 0, 0, 0, -pPawn, 0},
                    {0, 0, -pPawn, 0, 0, 0, 0, -pPawn}}};

    C_Session session = construct_session(5);
    marshall_board_state(session.board_state, board, White, {}, false, false, false, false, 0, 0);
    int w_p_pawns = passed_pawns(session.board_state, White);
    int b_p_pawns = passed_pawns(session.board_state, Black);

    EXPECT_EQ(w_p_pawns, 1);
    EXPECT_EQ(b_p_pawns, 2);
}

TEST(Evaluations, IsolatedPawns)
{
    Board board = {{{0, 0, pPawn, 0, 0, 0, pPawn, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, pQueen, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pPawn},
                    {pPawn, 0, 0, pPawn, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, pRook, 0, 0, 0, 0, -pPawn, 0},
                    {0, 0, -pPawn, 0, 0, 0, 0, -pPawn}}};

    C_Session session = construct_session(5);
    marshall_board_state(session.board_state, board, White, {}, false, false, false, false, 0, 0);
    int w_i_pawns = isolated_pawns(session.board_state, White);
    int b_i_pawns = isolated_pawns(session.board_state, Black);

    EXPECT_EQ(w_i_pawns, 2);
    EXPECT_EQ(b_i_pawns, 1);
}

TEST(Evaluations, CompletePawnStructure)
{
    Board board = {{{0, 0, pPawn, 0, 0, 0, pPawn, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, pQueen, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pPawn},
                    {pPawn, 0, 0, pPawn, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, pRook, 0, 0, 0, 0, -pPawn, 0},
                    {0, 0, -pPawn, 0, 0, 0, 0, -pPawn}}};

    C_Session session = construct_session(5);
    marshall_board_state(session.board_state, board, White, {}, false, false, false, false, 0, 0);
    Scores score = {0., 0.};
    evaluate_pawn_structure(session.board_state, score);
    float actual_pawn_structure_opening = 1 * doubled_pawn_penalty[OPENING] + 1 * isolated_pawn_penalty[OPENING] + -1 * passed_pawn_bonus[OPENING];
    float actual_pawn_structure_endgame = 1 * doubled_pawn_penalty[ENDGAME] + 1 * isolated_pawn_penalty[ENDGAME] + -1 * passed_pawn_bonus[ENDGAME];

    EXPECT_EQ(score[OPENING], actual_pawn_structure_opening);
    EXPECT_EQ(score[ENDGAME], actual_pawn_structure_endgame);
}

TEST(Evaluations, KingShield)
{
    Board board = {{{0, pPawn, 0, 0, 0, 0, 0, 0},
                    {0, pPawn, 0, 0, 0, 0, 0, 0},
                    {0, 0, pPawn, 0, 0, 0, 0, 0},
                    {pKing, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, -pPawn, 0, 0},
                    {0, 0, 0, 0, -pPawn, -pKing, 0, 0},
                    {0, 0, 0, 0, -pPawn, 0, 0, 0}}};

    C_Session session = construct_session(5);
    marshall_board_state(session.board_state, board, White, {}, false, false, false, false, 0, 0);
    Scores score = {0., 0.};
    int w_king_shield = n_pawns_shielding_king(session.board_state, White);
    int b_king_shield = n_pawns_shielding_king(session.board_state, Black);

    EXPECT_EQ(w_king_shield, 1);
    EXPECT_EQ(b_king_shield, 2);

    evaluate_king_shield(session.board_state, score);
    EXPECT_EQ(score[OPENING], -king_pawn_shield_bonus[OPENING]);
    EXPECT_EQ(score[ENDGAME], -king_pawn_shield_bonus[ENDGAME]);
}

TEST(Evaluations, RooksOnOpenFile)
{
    Board board = {{{0, pRook, 0, 0, 0, 0, -pPawn, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pRook},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, pRook, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, pRook, pRook, 0, 0, 0, 0, 0},
                    {0, pRook, 0, 0, 0, 0, -pRook, 0}}};

    C_Session session = construct_session(5);
    marshall_board_state(session.board_state, board, White, {}, false, false, false, false, 0, 0);
    Scores score = {0., 0.};

    float w_open_file_rooks = rooks_on_open_file(session.board_state, White);
    float b_open_file_rooks = rooks_on_open_file(session.board_state, Black);

    EXPECT_EQ(w_open_file_rooks, 2);
    EXPECT_EQ(b_open_file_rooks, 1);

    evaluate_rooks_on_open_file(session.board_state, score);
    EXPECT_EQ(score[OPENING], rook_open_file_bonus[OPENING]);
    EXPECT_EQ(score[ENDGAME], rook_open_file_bonus[ENDGAME]);
}

TEST(Evaluations, BishopPair)
{
    Board board = {{{0, 0, 0, 0, 0, -pBishop, 0, 0},
                    {0, 0, 0, 0, pBishop, 0, 0, 0},
                    {pBishop, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, -pBishop},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}};

    C_Session session = construct_session(5);
    marshall_board_state(session.board_state, board, White, {}, false, false, false, false, 0, 0);
    EXPECT_TRUE(has_bishop_pair(session.board_state, White));
    EXPECT_TRUE(has_bishop_pair(session.board_state, Black));

    board = {{{0, 0, 0, 0, 0, 0, 0, 0},
              {0, 0, 0, 0, pBishop, 0, 0, 0},
              {pBishop, 0, 0, 0, 0, 0, 0, 0},
              {0, 0, 0, 0, 0, 0, 0, 0},
              {0, 0, 0, 0, 0, 0, 0, 0},
              {0, 0, 0, 0, 0, 0, 0, -pBishop},
              {0, 0, 0, 0, 0, 0, 0, 0},
              {0, 0, 0, 0, 0, 0, 0, 0}}};

    session = construct_session(5);
    marshall_board_state(session.board_state, board, White, {}, false, false, false, false, 0, 0);
    EXPECT_TRUE(has_bishop_pair(session.board_state, White));
    EXPECT_FALSE(has_bishop_pair(session.board_state, Black));
}

TEST(Evaluations, GamePhaseInterpolationEndgame)
{
    Board board = {{{0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}};

    C_Session session = construct_session(5);
    marshall_board_state(session.board_state, board, White, {}, false, false, false, false, 0, 0);
    Scores score = {300., -201.};
    EXPECT_EQ(interpolate_scores(session.board_state, score), -201.);
}

TEST(Evaluations, GamePhaseInterpolationOpening)
{
    C_BoardState board = get_new_game_board();
    Scores score = {300., -201.};
    EXPECT_EQ(interpolate_scores(board, score), 300.);
}

TEST(Evaluations, GamePhaseInterpolationsIntermediate)
{
    Board board = {{{0, 0, 0, 0, 0, 0, pKing, 0},
                    {0, 0, 0, 0, pQueen, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, -pRook, 0, 0, 0},
                    {-pKing, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0}}};

    C_Session session = construct_session(5);
    marshall_board_state(session.board_state, board, White, {}, false, false, false, false, 0, 0);
    Scores score = {300., -201.};
    float phase = initial_phase_weights - 6;

    EXPECT_EQ(interpolate_scores(session.board_state, score), (300 * (initial_phase_weights - phase) + (-201. * phase)) / initial_phase_weights);
}