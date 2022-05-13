#ifndef BB_CONSTANTS
#define BB_CONSTANTS
#include <stdint.h>

const int King = 1;
const int Queen = 2;
const int Bishop = 3;
const int Knight = 4;
const int Rook = 5;
const int Pawn = 6;

const int Black = -1;
const int White = 1;

const uint64_t col_a = 0xFF00000000000000LL;
const uint64_t col_b = 0x00FF000000000000LL;
const uint64_t col_c = 0x0000FF0000000000LL;
const uint64_t col_d = 0x000000FF00000000LL;
const uint64_t col_e = 0x00000000FF000000LL;
const uint64_t col_f = 0x0000000000FF0000LL;
const uint64_t col_g = 0x000000000000FF00LL;
const uint64_t col_h = 0x00000000000000FFLL;

const uint64_t row_1 = 0x0101010101010101LL;
const uint64_t row_2 = 0x0202020202020202LL;
const uint64_t row_3 = 0x0404040404040404LL;
const uint64_t row_4 = 0x0808080808080808LL;
const uint64_t row_5 = 0x1010101010101010LL;
const uint64_t row_6 = 0x2020202020202020LL;
const uint64_t row_7 = 0x4040404040404040LL;
const uint64_t row_8 = 0x8080808080808080LL;

const uint64_t cols[] = {col_a, col_b, col_c, col_d, col_e, col_f, col_g, col_h};
const uint64_t rows[] = {row_1, row_2, row_3, row_4, row_5, row_6, row_7, row_8};

const uint64_t king_moves[] = {
    0x0203000000000000LL,
    0x0507000000000000LL,
    0x0A0E000000000000LL,
    0x141C000000000000LL,
    0x2838000000000000LL,
    0x5070000000000000LL,
    0xA0E0000000000000LL,
    0x40C0000000000000LL,
    ////A
    0x0302030000000000LL,
    0x0705070000000000LL,
    0x0E0A0E0000000000LL,
    0x1C141C0000000000LL,
    0x3828380000000000LL,
    0x7050700000000000LL,
    0xE0A0E00000000000LL,
    0xC040C00000000000LL,
    ////B
    0x0003020300000000LL,
    0x0007050700000000LL,
    0x000E0A0E00000000LL,
    0x001C141C00000000LL,
    0x0038283800000000LL,
    0x0070507000000000LL,
    0x00E0A0E000000000LL,
    0x00C040C000000000LL,
    ////C
    0x0000030203000000LL,
    0x0000070507000000LL,
    0x00000E0A0E000000LL,
    0x00001C141C000000LL,
    0x0000382838000000LL,
    0x0000705070000000LL,
    0x0000E0A0E0000000LL,
    0x0000C040C0000000LL,
    ////D
    0x0000000302030000LL,
    0x0000000705070000LL,
    0x0000000E0A0E0000LL,
    0x0000001C141C0000LL,
    0x0000003828380000LL,
    0x0000007050700000LL,
    0x000000E0A0E00000LL,
    0x000000C040C00000LL,
    ////E
    0x0000000003020300LL,
    0x0000000007050700LL,
    0x000000000E0A0E00LL,
    0x000000001C141C00LL,
    0x0000000038283800LL,
    0x0000000070507000LL,
    0x00000000E0A0E000LL,
    0x00000000C040C000LL,
    ////F
    0x0000000000030203LL,
    0x0000000000070507LL,
    0x00000000000E0A0ELL,
    0x00000000001C141CLL,
    0x0000000000382838LL,
    0x0000000000705070LL,
    0x0000000000E0A0E0LL,
    0x0000000000C040C0LL,
    ////G
    0x0000000000000302LL,
    0x0000000000000705LL,
    0x0000000000000E0ALL,
    0x0000000000001C14LL,
    0x0000000000003828LL,
    0x0000000000007050LL,
    0x000000000000E0A0LL,
    0x000000000000C040LL,
};

const uint64_t white_squares = 0x55AA55AA55AA55AALL;
const uint64_t black_squares = 0xAA55AA55AA55AA55LL;

const uint64_t white_pawns_starting_config = row_2;
const uint64_t black_pawns_starting_config = row_7;
const uint64_t white_rooks_starting_config = row_1 & (col_a | col_h);
const uint64_t black_rooks_starting_config = row_8 & (col_a | col_h);
const uint64_t white_knights_starting_config = row_1 & (col_b | col_g);
const uint64_t black_knights_starting_config = row_8 & (col_b | col_g);
const uint64_t white_bishops_starting_config = row_1 & (col_c | col_f);
const uint64_t black_bishops_starting_config = row_8 & (col_c | col_f);
const uint64_t white_queen_starting_config = row_1 & col_d;
const uint64_t black_queen_starting_config = row_8 & col_d;
const uint64_t white_king_starting_config = row_1 & col_e;
const uint64_t black_king_starting_config = row_8 & col_e;

#endif
