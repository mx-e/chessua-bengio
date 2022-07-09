
#ifndef PAWN_STRUCT
#define PAWN_STRUCT

#include <stdint.h>
#include <map>

#include "constants.hpp"
#include "types.hpp"
#include "expressions.hpp"
#include "utils.hpp"

enum Phase
{
    OPENING,
    ENDGAME
};

// from https://github.com/GunshipPenguin/shallow-blue/blob/master/src/eval.h
const int passed_pawn_bonus[2] = {[OPENING] = 10, [ENDGAME] = 70};
const int doubled_pawn_penalty[2] = {[OPENING] = -20, [ENDGAME] = -30};
const int isolated_pawn_penalty[2] = {[OPENING] = -15, [ENDGAME] = -30};

const uint64_t isolated_pawn_mask[8] = {
    col_b,
    col_a | col_c,
    col_b | col_d,
    col_c | col_e,
    col_d | col_f,
    col_e | col_g,
    col_f | col_h,
    col_g};

// white, black
const std::map<float, int> color_to_pawn_mask_idx = {{1., 0}, {-1, 1}};
const uint64_t passed_pawn_mask[2][64] = {{
                                              0x7f7f000000000000LL,
                                              0x3f3f000000000000LL,
                                              0x1f1f000000000000LL,
                                              0xf0f000000000000LL,
                                              0x707000000000000LL,
                                              0x303000000000000LL,
                                              0x101000000000000LL,
                                              0x0LL,
                                              0x7f7f7f0000000000LL,
                                              0x3f3f3f0000000000LL,
                                              0x1f1f1f0000000000LL,
                                              0xf0f0f0000000000LL,
                                              0x707070000000000LL,
                                              0x303030000000000LL,
                                              0x101010000000000LL,
                                              0x0LL,
                                              0x7f7f7f00000000LL,
                                              0x3f3f3f00000000LL,
                                              0x1f1f1f00000000LL,
                                              0xf0f0f00000000LL,
                                              0x7070700000000LL,
                                              0x3030300000000LL,
                                              0x1010100000000LL,
                                              0x0LL,
                                              0x7f7f7f000000LL,
                                              0x3f3f3f000000LL,
                                              0x1f1f1f000000LL,
                                              0xf0f0f000000LL,
                                              0x70707000000LL,
                                              0x30303000000LL,
                                              0x10101000000LL,
                                              0x0LL,
                                              0x7f7f7f0000LL,
                                              0x3f3f3f0000LL,
                                              0x1f1f1f0000LL,
                                              0xf0f0f0000LL,
                                              0x707070000LL,
                                              0x303030000LL,
                                              0x101010000LL,
                                              0x0LL,
                                              0x7f7f7f00LL,
                                              0x3f3f3f00LL,
                                              0x1f1f1f00LL,
                                              0xf0f0f00LL,
                                              0x7070700LL,
                                              0x3030300LL,
                                              0x1010100LL,
                                              0x0LL,
                                              0x7f7f7fLL,
                                              0x3f3f3fLL,
                                              0x1f1f1fLL,
                                              0xf0f0fLL,
                                              0x70707LL,
                                              0x30303LL,
                                              0x10101LL,
                                              0x0LL,
                                              0x7f7fLL,
                                              0x3f3fLL,
                                              0x1f1fLL,
                                              0xf0fLL,
                                              0x707LL,
                                              0x303LL,
                                              0x101LL,
                                              0x0LL,
                                          },
                                          {
                                              0x0LL,
                                              0x8080000000000000LL,
                                              0xc0c0000000000000LL,
                                              0xe0e0000000000000LL,
                                              0xf0f0000000000000LL,
                                              0xf8f8000000000000LL,
                                              0xfcfc000000000000LL,
                                              0xfefe000000000000LL,
                                              0x0LL,
                                              0x8080800000000000LL,
                                              0xc0c0c00000000000LL,
                                              0xe0e0e00000000000LL,
                                              0xf0f0f00000000000LL,
                                              0xf8f8f80000000000LL,
                                              0xfcfcfc0000000000LL,
                                              0xfefefe0000000000LL,
                                              0x0LL,
                                              0x80808000000000LL,
                                              0xc0c0c000000000LL,
                                              0xe0e0e000000000LL,
                                              0xf0f0f000000000LL,
                                              0xf8f8f800000000LL,
                                              0xfcfcfc00000000LL,
                                              0xfefefe00000000LL,
                                              0x0LL,
                                              0x808080000000LL,
                                              0xc0c0c0000000LL,
                                              0xe0e0e0000000LL,
                                              0xf0f0f0000000LL,
                                              0xf8f8f8000000LL,
                                              0xfcfcfc000000LL,
                                              0xfefefe000000LL,
                                              0x0LL,
                                              0x8080800000LL,
                                              0xc0c0c00000LL,
                                              0xe0e0e00000LL,
                                              0xf0f0f00000LL,
                                              0xf8f8f80000LL,
                                              0xfcfcfc0000LL,
                                              0xfefefe0000LL,
                                              0x0LL,
                                              0x80808000LL,
                                              0xc0c0c000LL,
                                              0xe0e0e000LL,
                                              0xf0f0f000LL,
                                              0xf8f8f800LL,
                                              0xfcfcfc00LL,
                                              0xfefefe00LL,
                                              0x0LL,
                                              0x808080LL,
                                              0xc0c0c0LL,
                                              0xe0e0e0LL,
                                              0xf0f0f0LL,
                                              0xf8f8f8LL,
                                              0xfcfcfcLL,
                                              0xfefefeLL,
                                              0x0LL,
                                              0x8080LL,
                                              0xc0c0LL,
                                              0xe0e0LL,
                                              0xf0f0LL,
                                              0xf8f8LL,
                                              0xfcfcLL,
                                              0xfefeLL,
                                          }};

int passed_pawns(const C_BoardState &board, float color)
{
    int passed = 0;
    uint64_t pawns = get_pawns(board, color);

    while (pawns != empty_board)
    {
        int field = forward_scan(pawns);
        if ((get_pawns(board, -color) & passed_pawn_mask[color_to_pawn_mask_idx.at(color)][field]) == 0)
            passed++;
        pawns &= ~cols[(int)field / 8];
    }
    return passed;
}

int doubled_pawns(const C_BoardState &board, float color)
{
    int doubled = 0;
    for (uint64_t col : cols)
    {
        int pawns_on_file = bb_pop_count(get_pawns(board, color) & col);
        if (pawns_on_file > 1)
            doubled += (pawns_on_file - 1);
    }
    return doubled;
}

int isolated_pawns(const C_BoardState &board, float color)
{
    int isolated = 0;
    uint64_t pawns = get_pawns(board, color);
    for (int col_idx = 0; col_idx < 8; col_idx++)
    {
        uint64_t mask = cols[col_idx];
        uint64_t adj_mask = isolated_pawn_mask[col_idx];
        if ((pawns & mask) && !(pawns & adj_mask))
            isolated++;
    }
    return isolated;
}

void evaluate_pawn_structure(const C_BoardState &board, Scores &scores)
{
    int passed_pawn_diff = passed_pawns(board, White) - passed_pawns(board, Black);
    scores[OPENING] += passed_pawn_bonus[OPENING] * passed_pawn_diff;
    scores[ENDGAME] += passed_pawn_bonus[ENDGAME] * passed_pawn_diff;

    int doubled_pawns_diff = doubled_pawns(board, White) - doubled_pawns(board, Black);
    scores[OPENING] += doubled_pawn_penalty[OPENING] * doubled_pawns_diff;
    scores[ENDGAME] += doubled_pawn_penalty[ENDGAME] * doubled_pawns_diff;

    int isolated_pawn_diff = isolated_pawns(board, White) - isolated_pawns(board, Black);
    scores[OPENING] += isolated_pawn_penalty[OPENING] * isolated_pawn_diff;
    scores[ENDGAME] += isolated_pawn_penalty[ENDGAME] * isolated_pawn_diff;
}

#endif
