#include <gtest/gtest.h>
#include "../include/collections.hpp"
#include "../include/transforms.hpp"
#include "test_utils.hpp"

void _print_bitboard(uint64_t bb)
{
    std::string bb_str = "";
    for (int row = 0; row < 8; row++)
    {
        for (int col = 7; col >= 0; col--)
        {
            uint64_t index = 1;
            std::string bit = bb & (index << (col * 8 + row)) ? "1" : "∙";
            bb_str += bit;
            if (col != 0)
            {
                bb_str += " ";
            }
        }
        bb_str += '\n';
    }
    bb_str += '\n';
    std::cout << bb_str;
}

TEST(BoardTransforms, CastlingPossible)
{
    // w_kingside
    C_Session session = construct_session();
    C_BoardState board = session.board_state;
    uint64_t blockers = fill_bitboard_max(empty_board, {56, 32});
    print_bitboard(blockers);
    board.pieces[b_black] = blockers;
    EXPECT_EQ(get_castling_possible(board, w_kingside), true);

    unset_castling_rights(board, w_kingside);
    EXPECT_EQ(get_castling_possible(board, w_kingside), false);
    set_castling_rights(board, w_kingside);
    EXPECT_EQ(get_castling_possible(board, w_kingside), true);

    blockers = fill_bitboard_max(empty_board, {56, 40, 48, 32});
    print_bitboard(blockers);
    board.pieces[b_black] = blockers;
    EXPECT_EQ(get_castling_possible(board, w_kingside), false);
    unset_castling_rights(board, w_kingside);

    // w_queenside
    blockers = fill_bitboard_max(empty_board, {0, 32});
    print_bitboard(blockers);
    board.pieces[b_white] = blockers;
    EXPECT_EQ(get_castling_possible(board, w_queenside), true);
    unset_castling_rights(board, w_queenside);
    EXPECT_EQ(get_castling_possible(board, w_queenside), false);
    set_castling_rights(board, w_queenside);

    blockers = fill_bitboard_max(empty_board, {16});
    board.pieces[b_white] = blockers;
    EXPECT_EQ(get_castling_possible(board, w_queenside), false);

    // b_kingside
    EXPECT_EQ(get_castling_possible(board, b_kingside), true);
    unset_castling_rights(board, b_kingside);
    EXPECT_EQ(get_castling_possible(board, b_kingside), false);
    set_castling_rights(board, b_kingside);
    blockers = fill_bitboard_max(empty_board, {55});
    board.pieces[b_white] = blockers;
    EXPECT_EQ(get_castling_possible(board, b_kingside), false);

    // b_queenside
    EXPECT_EQ(get_castling_possible(board, b_queenside), true);
    unset_castling_rights(board, b_queenside);
    EXPECT_EQ(get_castling_possible(board, b_queenside), false);
    set_castling_rights(board, b_queenside);
    blockers = fill_bitboard_max(empty_board, {15});
    board.pieces[b_black] = blockers;
    EXPECT_EQ(get_castling_possible(board, b_queenside), false);
}

TEST(Collections, PawnFreeMove)
{
    C_BoardState board_state = init_board_state_for_test(White);

    set_pieces(board_state, White, pPawn, fill_bitboard({{3, 3}, {5, 7}}));

    MoveList move_list;
    collect_pawn_moves_and_captures(board_state, move_list);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(3, 3) && move.dest == flat(3, 4); }),
              true);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(5, 7) && move.dest == flat(5, 8); }),
              false);
}

TEST(Collections, PawnBlocked)
{
    C_BoardState board_state = init_board_state_for_test(White);

    set_pieces(board_state, White, pPawn, fill_bitboard({{3, 3}}));
    set_pieces(board_state, Black, pPawn, fill_bitboard({{3, 4}}));

    for (auto turn : {White, Black})
    {
        board_state.turn = turn;

        MoveList move_list;
        collect_pawn_moves_and_captures(board_state, move_list);

        EXPECT_EQ(move_list.size(), 0);
    }
}

TEST(Collections, PawnDoubleMoveWhite)
{
    C_BoardState board_state = init_board_state_for_test(White);

    set_pieces(board_state, White, pPawn, fill_bitboard({{3, 1}}));

    MoveList move_list;
    collect_pawn_moves_and_captures(board_state, move_list);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(3, 1) && move.dest == flat(3, 3); }),
              true);
}

TEST(Collections, PawnDoubleMoveBlack)
{
    C_BoardState board_state = init_board_state_for_test(Black);

    set_pieces(board_state, Black, pPawn, fill_bitboard({{3, 6}}));

    MoveList move_list;
    collect_pawn_moves_and_captures(board_state, move_list);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(3, 6) && move.dest == flat(3, 4); }),
              true);
}

TEST(Collections, PawnPromotionWhite)
{
    C_BoardState board_state = init_board_state_for_test(White);

    set_pieces(board_state, White, pPawn, fill_bitboard({{3, 6}}));

    MoveList move_list;
    collect_pawn_moves_and_captures(board_state, move_list);

    for (auto promotion_piece : {pQueen, pKnight, pRook, pBishop})
    {
        EXPECT_EQ(exists<move>(
                      move_list, [&promotion_piece](move move)
                      { return move.src == flat(3, 6) && move.dest == flat(3, 7) && move.promotion == promotion_piece; }),
                  true);
    }
}

TEST(Collections, PawnPromotionBlack)
{
    C_BoardState board_state = init_board_state_for_test(Black);

    set_pieces(board_state, Black, pPawn, fill_bitboard({{3, 1}}));

    MoveList move_list;
    collect_pawn_moves_and_captures(board_state, move_list);

    for (auto promotion_piece : {pQueen, pKnight, pRook, pBishop})
    {
        EXPECT_EQ(exists<move>(
                      move_list, [&promotion_piece](move move)
                      { return move.src == flat(3, 1) && move.dest == flat(3, 0) && move.promotion == promotion_piece; }),
                  true);
    }
}

TEST(Collections, PawnCaptureWhite)
{
    C_BoardState board_state = init_board_state_for_test(White);

    set_pieces(board_state, White, pPawn, fill_bitboard({{4, 4}}));
    set_pieces(board_state, Black, pKnight, fill_bitboard({{5, 5}}));
    set_pieces(board_state, Black, pRook, fill_bitboard({{3, 5}}));

    MoveList move_list;
    collect_pawn_moves_and_captures(board_state, move_list);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(4, 4) && move.dest == flat(5, 5) && move.capture == pKnight; }),
              true);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(4, 4) && move.dest == flat(3, 5) && move.capture == pRook; }),
              true);
}

TEST(Collections, PawnCaptureBlack)
{
    C_BoardState board_state = init_board_state_for_test(Black);

    set_pieces(board_state, Black, pPawn, fill_bitboard({{4, 4}}));
    set_pieces(board_state, White, pKnight, fill_bitboard({{5, 3}}));
    set_pieces(board_state, White, pRook, fill_bitboard({{3, 3}}));

    MoveList move_list;
    collect_pawn_moves_and_captures(board_state, move_list);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(4, 4) && move.dest == flat(5, 3) && move.capture == pKnight; }),
              true);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(4, 4) && move.dest == flat(3, 3) && move.capture == pRook; }),
              true);
}

TEST(Collections, PawnEnPassantCaptureWhite)
{
    C_BoardState board_state = init_board_state_for_test(White);

    set_pieces(board_state, White, pPawn, fill_bitboard({{4, 4}}));
    set_pieces(board_state, Black, pPawn, fill_bitboard({{5, 4}}));
    set_pieces(board_state, Black, pPawn, fill_bitboard({{3, 4}}));

    for (auto en_passant : {flat(5, 5), flat(3, 5)})
    {
        board_state.en_passant = en_passant;

        MoveList move_list;
        collect_pawn_moves_and_captures(board_state, move_list);

        EXPECT_EQ(exists<move>(
                      move_list, [&en_passant](move move)
                      { return move.src == flat(4, 4) && move.dest == en_passant && move.capture == 0; }),
                  true);
    }
}

TEST(Collections, PawnEnPassantCaptureBlack)
{
    C_BoardState board_state = init_board_state_for_test(Black);

    set_pieces(board_state, Black, pPawn, fill_bitboard({{4, 3}}));
    set_pieces(board_state, White, pPawn, fill_bitboard({{5, 3}}));
    set_pieces(board_state, White, pPawn, fill_bitboard({{3, 3}}));

    for (auto en_passant : {flat(5, 2), flat(3, 2)})
    {
        board_state.en_passant = en_passant;

        MoveList move_list;
        collect_pawn_moves_and_captures(board_state, move_list);

        EXPECT_EQ(exists<move>(
                      move_list, [&en_passant](move move)
                      { return move.src == flat(4, 3) && move.dest == en_passant && move.capture == 0; }),
                  true);
    }
}

TEST(Collections, KnightFreeMoves)
{
    for (auto color : {White, Black})
    {
        C_BoardState board_state = init_board_state_for_test(color);

        set_pieces(board_state, color, pKnight, fill_bitboard({{4, 4}}));

        MoveList move_list;
        collect_knight_moves_and_captures(board_state, move_list);

        EXPECT_EQ(move_list.size(), 8);

        EXPECT_EQ(exists<move>(
                      move_list, [](move move)
                      { return move.src == flat(4, 4) && move.dest == flat(5, 6); }),
                  true);
        EXPECT_EQ(exists<move>(
                      move_list, [](move move)
                      { return move.src == flat(4, 4) && move.dest == flat(6, 5); }),
                  true);
        EXPECT_EQ(exists<move>(
                      move_list, [](move move)
                      { return move.src == flat(4, 4) && move.dest == flat(3, 6); }),
                  true);
        EXPECT_EQ(exists<move>(
                      move_list, [](move move)
                      { return move.src == flat(4, 4) && move.dest == flat(6, 3); }),
                  true);
        EXPECT_EQ(exists<move>(
                      move_list, [](move move)
                      { return move.src == flat(4, 4) && move.dest == flat(3, 2); }),
                  true);
        EXPECT_EQ(exists<move>(
                      move_list, [](move move)
                      { return move.src == flat(4, 4) && move.dest == flat(2, 3); }),
                  true);
        EXPECT_EQ(exists<move>(
                      move_list, [](move move)
                      { return move.src == flat(4, 4) && move.dest == flat(5, 2); }),
                  true);
        EXPECT_EQ(exists<move>(
                      move_list, [](move move)
                      { return move.src == flat(4, 4) && move.dest == flat(2, 5); }),
                  true);
    }
}

TEST(Collections, KnightCaptureMoves)
{
    for (auto color : {White, Black})
    {
        C_BoardState board_state = init_board_state_for_test(color);

        set_pieces(board_state, color, pKnight, fill_bitboard({{4, 4}}));
        set_pieces(board_state, -color, pQueen, fill_bitboard({{3, 6}}));
        set_pieces(board_state, -color, pBishop, fill_bitboard({{2, 3}}));

        MoveList move_list;
        collect_knight_moves_and_captures(board_state, move_list);

        EXPECT_EQ(exists<move>(
                      move_list, [](move move)
                      { return move.src == flat(4, 4) && move.dest == flat(3, 6) && move.capture == pQueen; }),
                  true);
        EXPECT_EQ(exists<move>(
                      move_list, [](move move)
                      { return move.src == flat(4, 4) && move.dest == flat(2, 3) && move.capture == pBishop; }),
                  true);
    }
}

TEST(Collections, BishopFreeMoves)
{
    for (auto color : {White, Black})
    {
        C_BoardState board_state = init_board_state_for_test(color);

        set_pieces(board_state, color, pBishop, fill_bitboard({{4, 5}}));

        MoveList move_list;
        collect_bishop_moves_and_captures(board_state, move_list);

        EXPECT_EQ(move_list.size(), 11);
    }
}

TEST(Collections, BishopCaptures)
{
    for (auto color : {White, Black})
    {
        C_BoardState board_state = init_board_state_for_test(color);

        set_pieces(board_state, color, pBishop, fill_bitboard({{4, 5}}));
        set_pieces(board_state, -color, pPawn, fill_bitboard({{2, 3}}));

        MoveList move_list;
        collect_bishop_moves_and_captures(board_state, move_list);

        EXPECT_EQ(move_list.size(), 9);

        EXPECT_EQ(exists<move>(
                      move_list, [](move move)
                      { return move.src == flat(4, 5) && move.dest == flat(2, 3) && move.capture == pPawn; }),
                  true);
    }
}

TEST(Collections, RookFreeMoves)
{
    for (auto color : {White, Black})
    {
        C_BoardState board_state = init_board_state_for_test(color);

        set_pieces(board_state, color, pRook, fill_bitboard({{4, 5}}));

        MoveList move_list;
        collect_rook_moves_and_captures(board_state, move_list);

        EXPECT_EQ(move_list.size(), 13);
    }
}

TEST(Collections, RookCaptures)
{
    for (auto color : {White, Black})
    {
        C_BoardState board_state = init_board_state_for_test(color);

        set_pieces(board_state, color, pRook, fill_bitboard({{4, 5}}));
        set_pieces(board_state, -color, pPawn, fill_bitboard({{2, 5}}));

        MoveList move_list;
        collect_rook_moves_and_captures(board_state, move_list);

        EXPECT_EQ(move_list.size(), 11);

        EXPECT_EQ(exists<move>(
                      move_list, [](move move)
                      { return move.src == flat(4, 5) && move.dest == flat(2, 5) && move.capture == pPawn; }),
                  true);
    }
}

TEST(Collections, QueenFreeMoves)
{
    for (auto color : {White, Black})
    {
        C_BoardState board_state = init_board_state_for_test(color);

        set_pieces(board_state, color, pQueen, fill_bitboard({{4, 5}}));

        MoveList move_list;
        collect_queen_moves_and_captures(board_state, move_list);

        EXPECT_EQ(move_list.size(), 24);
    }
}

TEST(Collections, QueenCaptures)
{
    for (auto color : {White, Black})
    {
        C_BoardState board_state = init_board_state_for_test(color);

        set_pieces(board_state, color, pQueen, fill_bitboard({{4, 5}}));
        set_pieces(board_state, -color, pPawn, fill_bitboard({{2, 3}}));

        MoveList move_list;
        collect_queen_moves_and_captures(board_state, move_list);

        EXPECT_EQ(move_list.size(), 22);

        EXPECT_EQ(exists<move>(
                      move_list, [](move move)
                      { return move.src == flat(4, 5) && move.dest == flat(2, 3) && move.capture == pPawn; }),
                  true);
    }
}

TEST(Collections, KingFreeMoves)
{
    C_BoardState board_state;
    set_pieces(board_state, White, pKing, fill_bitboard({{2, 2}}));
    board_state.castling_rights = 0;
    // set_pieces(board_state, Black, pPawn, fill_bitboard({{2, 3}}));

    MoveList move_list;
    collect_king_moves_and_captures(board_state, move_list);

    EXPECT_EQ(move_list.size(), 8);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(2, 2) && move.dest == flat(1, 1); }),
              true);
    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(2, 2) && move.dest == flat(1, 2); }),
              true);
    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(2, 2) && move.dest == flat(2, 1); }),
              true);
    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(2, 2) && move.dest == flat(2, 3); }),
              true);
    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(2, 2) && move.dest == flat(3, 2); }),
              true);
    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(2, 2) && move.dest == flat(3, 3); }),
              true);
    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(2, 2) && move.dest == flat(3, 1); }),
              true);
    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(2, 2) && move.dest == flat(1, 3); }),
              true);
}

TEST(Collections, KingCaptureMoves)
{
    C_BoardState board_state;
    set_pieces(board_state, White, pKing, fill_bitboard({{2, 2}}));
    set_pieces(board_state, Black, pRook, fill_bitboard({{2, 3}}));
    set_pieces(board_state, Black, pKnight, fill_bitboard({{1, 1}}));

    board_state.castling_rights = 0;

    MoveList move_list;
    collect_king_moves_and_captures(board_state, move_list);

    EXPECT_EQ(move_list.size(), 8);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(2, 2) && move.dest == flat(1, 1) && move.capture == pKnight; }),
              true);
    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(2, 2) && move.dest == flat(2, 3) && move.capture == pRook; }),
              true);
}

TEST(Collections, CastlingWhite)
{
    C_BoardState board_state = init_board_state_for_test(White);
    set_castling_rights(board_state, w_kingside);
    set_castling_rights(board_state, w_queenside);

    set_pieces(board_state, White, pRook, fill_bitboard({{7, 0}}));
    set_pieces(board_state, White, pRook, fill_bitboard({{0, 0}}));

    MoveList move_list;
    collect_king_moves_and_captures(board_state, move_list);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(4, 0) && move.dest == flat(6, 0); }),
              true);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(4, 0) && move.dest == flat(2, 0); }),
              true);
}

TEST(Collections, CastlingBlack)
{
    C_BoardState board_state = init_board_state_for_test(Black);
    set_castling_rights(board_state, b_kingside);
    set_castling_rights(board_state, b_queenside);

    set_pieces(board_state, Black, pRook, fill_bitboard({{7, 7}}));
    set_pieces(board_state, Black, pRook, fill_bitboard({{0, 7}}));

    MoveList move_list;
    collect_king_moves_and_captures(board_state, move_list);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(4, 7) && move.dest == flat(6, 7); }),
              true);

    EXPECT_EQ(exists<move>(
                  move_list, [](move move)
                  { return move.src == flat(4, 7) && move.dest == flat(2, 7); }),
              true);
}
