#ifndef CONSTANTS
#define CONSTANTS
#include <map>
#include <array>
#include <vector>
#include <stdint.h>

const uint8_t pKing = 1;
const uint8_t pQueen = 2;
const uint8_t pBishop = 3;
const uint8_t pKnight = 4;
const uint8_t pRook = 5;
const uint8_t pPawn = 6;

const int Black = -1.;
const int White = 1.;

enum board
{
    b_black,
    b_kings,
    b_queens,
    b_bishops,
    b_knights,
    b_rooks,
    b_pawns,
    b_white,
};
enum castling
{
    w_kingside,
    w_queenside,
    b_kingside,
    b_queenside,
};

const std::map<float, int>
    color_to_BB_index = {{-1., 0}, {1., 7}};

enum knight_direction
{
    NNE,
    NEE,
    SEE,
    SSE,
    SSW,
    SWW,
    NWW,
    NNW,
};
const std::vector<knight_direction> all_knight_directions = {NNE, NEE, SEE, SSE, SSW, SWW, NWW, NNW};
const std::map<knight_direction, float> knight_idx_offsets =
    {{NNE, 10.}, {NEE, 17.}, {SEE, 15.}, {SSE, 6.}, {SSW, -10.}, {SWW, -17.}, {NWW, -15.}, {NNW, -6}};

const std::map<int, char>
    col_int_to_string_map = {{0, 'a'}, {1, 'b'}, {2, 'c'}, {3, 'd'}, {4, 'e'}, {5, 'f'}, {6, 'g'}, {7, 'h'}};

const uint64_t col_a = 0xFF00000000000000LL;
const uint64_t col_b = 0x00FF000000000000LL;
const uint64_t col_c = 0x0000FF0000000000LL;
const uint64_t col_d = 0x000000FF00000000LL;
const uint64_t col_e = 0x00000000FF000000LL;
const uint64_t col_f = 0x0000000000FF0000LL;
const uint64_t col_g = 0x000000000000FF00LL;
const uint64_t col_h = 0x00000000000000FFLL;

const uint64_t row_1 = 0x8080808080808080LL;
const uint64_t row_2 = 0x4040404040404040LL;
const uint64_t row_3 = 0x2020202020202020LL;
const uint64_t row_4 = 0x1010101010101010LL;
const uint64_t row_5 = 0x0808080808080808LL;
const uint64_t row_6 = 0x0404040404040404LL;
const uint64_t row_7 = 0x0202020202020202LL;
const uint64_t row_8 = 0x0101010101010101LL;

const uint64_t cols[] = {col_a, col_b, col_c, col_d, col_e, col_f, col_g, col_h};
const uint64_t rows[] = {row_1, row_2, row_3, row_4, row_5, row_6, row_7, row_8};

const uint64_t white_squares = 0x55AA55AA55AA55AALL;
const uint64_t black_squares = 0xAA55AA55AA55AA55LL;
const uint64_t whole_field = 0xFFFFFFFFFFFFFFFFLL;
const uint64_t white_half = 0xF0F0F0F0F0F0F0F0LL;
const uint64_t black_half = 0x0F0F0F0F0F0F0F0FLL;
const uint64_t center_half = 0x3C3C3C3C3C3C3C3CLL;
const uint64_t edge_half = 0xC3C3C3C3C3C3C3C3LL;
const uint64_t most_sig_bit = 0x8000000000000000LL;
const uint64_t empty_board = 0x0000000000000000LL;

const uint64_t w_ks_castling_mask = 0x0000000080000080LL;
const uint64_t w_qs_castling_mask = 0x8000000080000000LL;
const uint64_t b_ks_castling_mask = 0x0000000001000001LL;
const uint64_t b_qs_castling_mask = 0x0100000001000000LL;

const uint64_t w_ks_castling_no_attack = 0x0000000080808000LL;
const uint64_t w_qs_castling_no_attack = 0x0000808080000000LL;
const uint64_t b_ks_castling_no_attack = 0x0000000001010100LL;
const uint64_t b_qs_castling_no_attack = 0x0000010101000000LL;

const uint64_t w_ks_castling_free = 0x0000000000808000LL;
const uint64_t w_qs_castling_free = 0x0080808000000000LL;
const uint64_t b_ks_castling_free = 0x0000000000010100LL;
const uint64_t b_qs_castling_free = 0x0001010100000000LL;

const uint64_t ep_zone = row_3 | row_6;

const std::map<float, std::array<uint64_t, 2>> color_to_castling_mask =
    {{1., {w_ks_castling_mask, w_qs_castling_mask}}, {-1., {b_ks_castling_mask, b_qs_castling_mask}}};

const std::map<float, std::array<uint64_t, 2>> color_to_castling_no_attack =
    {{1., {w_ks_castling_no_attack, w_qs_castling_no_attack}}, {-1., {b_ks_castling_no_attack, b_qs_castling_no_attack}}};

const std::map<float, uint64_t> castling_to_castling_free =
    {{w_kingside, w_ks_castling_free},
     {w_queenside, w_qs_castling_free},
     {b_kingside, b_ks_castling_free},
     {b_queenside, b_qs_castling_free}};

const uint8_t w_ks_castling_state_mask = UINT8_C(0x01);
const uint8_t w_qs_castling_state_mask = UINT8_C(0x02);
const uint8_t b_ks_castling_state_mask = UINT8_C(0x04);
const uint8_t b_qs_castling_state_mask = UINT8_C(0x08);

const std::map<castling, uint8_t> castling_to_castling_state_mask =
    {{w_kingside, w_ks_castling_state_mask},
     {w_queenside, w_qs_castling_state_mask},
     {b_kingside, b_ks_castling_state_mask},
     {b_queenside, b_qs_castling_state_mask}};

const std::map<float, std::array<castling, 2>> color_to_castling_indicator =
    {{1., {w_kingside, w_queenside}}, {-1., {b_kingside, b_queenside}}};

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

const uint64_t king_moves[64] = {
    ////A
    0x40C0000000000000LL,
    0xA0E0000000000000LL,
    0x5070000000000000LL,
    0x2838000000000000LL,
    0x141C000000000000LL,
    0x0A0E000000000000LL,
    0x0507000000000000LL,
    0x0203000000000000LL,

    ////B
    0xC040C00000000000LL,
    0xE0A0E00000000000LL,
    0x7050700000000000LL,
    0x3828380000000000LL,
    0x1C141C0000000000LL,
    0x0E0A0E0000000000LL,
    0x0705070000000000LL,
    0x0302030000000000LL,

    ////C
    0x00C040C000000000LL,
    0x00E0A0E000000000LL,
    0x0070507000000000LL,
    0x0038283800000000LL,
    0x001C141C00000000LL,
    0x000E0A0E00000000LL,
    0x0007050700000000LL,
    0x0003020300000000LL,

    ////D
    0x0000C040C0000000LL,
    0x0000E0A0E0000000LL,
    0x0000705070000000LL,
    0x0000382838000000LL,
    0x00001C141C000000LL,
    0x00000E0A0E000000LL,
    0x0000070507000000LL,
    0x0000030203000000LL,

    ////E
    0x000000C040C00000LL,
    0x000000E0A0E00000LL,
    0x0000007050700000LL,
    0x0000003828380000LL,
    0x0000001C141C0000LL,
    0x0000000E0A0E0000LL,
    0x0000000705070000LL,
    0x0000000302030000LL,

    ////F
    0x00000000C040C000LL,
    0x00000000E0A0E000LL,
    0x0000000070507000LL,
    0x0000000038283800LL,
    0x000000001C141C00LL,
    0x000000000E0A0E00LL,
    0x0000000007050700LL,
    0x0000000003020300LL,

    ////G
    0x0000000000C040C0LL,
    0x0000000000E0A0E0LL,
    0x0000000000705070LL,
    0x0000000000382838LL,
    0x00000000001C141CLL,
    0x00000000000E0A0ELL,
    0x0000000000070507LL,
    0x0000000000030203LL,

    ////H
    0x000000000000C040LL,
    0x000000000000E0A0LL,
    0x0000000000007050LL,
    0x0000000000003828LL,
    0x0000000000001C14LL,
    0x0000000000000E0ALL,
    0x0000000000000705LL,
    0x0000000000000302LL,
};

enum sliding_direction
{
    N,
    NE,
    E,
    SE,
    S,
    SW,
    W,
    NW
};

const std::vector<sliding_direction>
    all_sliding_directions = {N, NE, E, SE, S, SW, W, NW};
const std::map<sliding_direction, float> sliding_idx_offset =
    {{N, 1.}, {NE, 9.}, {E, 8.}, {SE, 7.}, {S, -1.}, {SW, -9}, {W, -8}, {NW, -7.}};

const uint64_t rays[8][64] = {
    ////N
    {
        ////A
        0x7F00000000000000LL,
        0x3F00000000000000LL,
        0x1F00000000000000LL,
        0x0F00000000000000LL,
        0x0700000000000000LL,
        0x0300000000000000LL,
        0x0100000000000000LL,
        0x0000000000000000LL,
        ////B
        0x007F000000000000LL,
        0x003F000000000000LL,
        0x001F000000000000LL,
        0x000F000000000000LL,
        0x0007000000000000LL,
        0x0003000000000000LL,
        0x0001000000000000LL,
        0x0000000000000000LL,
        ////C
        0x00007F0000000000LL,
        0x00003F0000000000LL,
        0x00001F0000000000LL,
        0x00000F0000000000LL,
        0x0000070000000000LL,
        0x0000030000000000LL,
        0x0000010000000000LL,
        0x0000000000000000LL,
        ////D
        0x0000007F00000000LL,
        0x0000003F00000000LL,
        0x0000001F00000000LL,
        0x0000000F00000000LL,
        0x0000000700000000LL,
        0x0000000300000000LL,
        0x0000000100000000LL,
        0x0000000000000000LL,
        ////E
        0x000000007F000000LL,
        0x000000003F000000LL,
        0x000000001F000000LL,
        0x000000000F000000LL,
        0x0000000007000000LL,
        0x0000000003000000LL,
        0x0000000001000000LL,
        0x0000000000000000LL,
        ////F
        0x00000000007F0000LL,
        0x00000000003F0000LL,
        0x00000000001F0000LL,
        0x00000000000F0000LL,
        0x0000000000070000LL,
        0x0000000000030000LL,
        0x0000000000010000LL,
        0x0000000000000000LL,
        ////G
        0x0000000000007F00LL,
        0x0000000000003F00LL,
        0x0000000000001F00LL,
        0x0000000000000F00LL,
        0x0000000000000700LL,
        0x0000000000000300LL,
        0x0000000000000100LL,
        0x0000000000000000LL,
        ////H
        0x000000000000007FLL,
        0x000000000000003FLL,
        0x000000000000001FLL,
        0x000000000000000FLL,
        0x0000000000000007LL,
        0x0000000000000003LL,
        0x0000000000000001LL,
        0x0000000000000000LL,
    },
    ////NE
    {
        ////A
        0x0040201008040201LL,
        0x0020100804020100LL,
        0x0010080402010000LL,
        0x0008040201000000LL,
        0x0004020100000000LL,
        0x0002010000000000LL,
        0x0001000000000000LL,
        0x0000000000000000LL,
        ////B
        0x0000402010080402LL,
        0x0000201008040201LL,
        0x0000100804020100LL,
        0x0000080402010000LL,
        0x0000040201000000LL,
        0x0000020100000000LL,
        0x0000010000000000LL,
        0x0000000000000000LL,
        ////C
        0x0000004020100804LL,
        0x0000002010080402LL,
        0x0000001008040201LL,
        0x0000000804020100LL,
        0x0000000402010000LL,
        0x0000000201000000LL,
        0x0000000100000000LL,
        0x0000000000000000LL,
        ////D
        0x0000000040201008LL,
        0x0000000020100804LL,
        0x0000000010080402LL,
        0x0000000008040201LL,
        0x0000000004020100LL,
        0x0000000002010000LL,
        0x0000000001000000LL,
        0x0000000000000000LL,
        ////E
        0x0000000000402010LL,
        0x0000000000201008LL,
        0x0000000000100804LL,
        0x0000000000080402LL,
        0x0000000000040201LL,
        0x0000000000020100LL,
        0x0000000000010000LL,
        0x0000000000000000LL,
        ////F
        0x0000000000004020LL,
        0x0000000000002010LL,
        0x0000000000001008LL,
        0x0000000000000804LL,
        0x0000000000000402LL,
        0x0000000000000201LL,
        0x0000000000000100LL,
        0x0000000000000000LL,
        ////G
        0x0000000000000040LL,
        0x0000000000000020LL,
        0x0000000000000010LL,
        0x0000000000000008LL,
        0x0000000000000004LL,
        0x0000000000000002LL,
        0x0000000000000001LL,
        0x0000000000000000LL,
        ////H
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
    },
    ////E
    {
        ////A
        0x0080808080808080LL,
        0x0040404040404040LL,
        0x0020202020202020LL,
        0x0010101010101010LL,
        0x0008080808080808LL,
        0x0004040404040404LL,
        0x0002020202020202LL,
        0x0001010101010101LL,
        ////B
        0x0000808080808080LL,
        0x0000404040404040LL,
        0x0000202020202020LL,
        0x0000101010101010LL,
        0x0000080808080808LL,
        0x0000040404040404LL,
        0x0000020202020202LL,
        0x0000010101010101LL,
        ////C
        0x0000008080808080LL,
        0x0000004040404040LL,
        0x0000002020202020LL,
        0x0000001010101010LL,
        0x0000000808080808LL,
        0x0000000404040404LL,
        0x0000000202020202LL,
        0x0000000101010101LL,
        ////D
        0x0000000080808080LL,
        0x0000000040404040LL,
        0x0000000020202020LL,
        0x0000000010101010LL,
        0x0000000008080808LL,
        0x0000000004040404LL,
        0x0000000002020202LL,
        0x0000000001010101LL,
        ////E
        0x0000000000808080LL,
        0x0000000000404040LL,
        0x0000000000202020LL,
        0x0000000000101010LL,
        0x0000000000080808LL,
        0x0000000000040404LL,
        0x0000000000020202LL,
        0x0000000000010101LL,
        ////F
        0x0000000000008080LL,
        0x0000000000004040LL,
        0x0000000000002020LL,
        0x0000000000001010LL,
        0x0000000000000808LL,
        0x0000000000000404LL,
        0x0000000000000202LL,
        0x0000000000000101LL,
        ////G
        0x0000000000000080LL,
        0x0000000000000040LL,
        0x0000000000000020LL,
        0x0000000000000010LL,
        0x0000000000000008LL,
        0x0000000000000004LL,
        0x0000000000000002LL,
        0x0000000000000001LL,
        ////H
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
    },
    ////SE
    {
        ////A
        0x0000000000000000LL,
        0x0080000000000000LL,
        0x0040800000000000LL,
        0x0020408000000000LL,
        0x0010204080000000LL,
        0x0008102040800000LL,
        0x0004081020408000LL,
        0x0002040810204080LL,
        ////B
        0x0000000000000000LL,
        0x0000800000000000LL,
        0x0000408000000000LL,
        0x0000204080000000LL,
        0x0000102040800000LL,
        0x0000081020408000LL,
        0x0000040810204080LL,
        0x0000020408102040LL,
        ////C
        0x0000000000000000LL,
        0x0000008000000000LL,
        0x0000004080000000LL,
        0x0000002040800000LL,
        0x0000001020408000LL,
        0x0000000810204080LL,
        0x0000000408102040LL,
        0x0000000204081020LL,
        ////D
        0x0000000000000000LL,
        0x0000000080000000LL,
        0x0000000040800000LL,
        0x0000000020408000LL,
        0x0000000010204080LL,
        0x0000000008102040LL,
        0x0000000004081020LL,
        0x0000000002040810LL,
        ////E
        0x0000000000000000LL,
        0x0000000000800000LL,
        0x0000000000408000LL,
        0x0000000000204080LL,
        0x0000000000102040LL,
        0x0000000000081020LL,
        0x0000000000040810LL,
        0x0000000000020408LL,
        ////F
        0x0000000000000000LL,
        0x0000000000008000LL,
        0x0000000000004080LL,
        0x0000000000002040LL,
        0x0000000000001020LL,
        0x0000000000000810LL,
        0x0000000000000408LL,
        0x0000000000000204LL,
        ////G
        0x0000000000000000LL,
        0x0000000000000080LL,
        0x0000000000000040LL,
        0x0000000000000020LL,
        0x0000000000000010LL,
        0x0000000000000008LL,
        0x0000000000000004LL,
        0x0000000000000002LL,
        ////H
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,

    },
    ////S
    {
        ////A
        0x0000000000000000LL,
        0x8000000000000000LL,
        0xC000000000000000LL,
        0xE000000000000000LL,
        0xF000000000000000LL,
        0xF800000000000000LL,
        0xFC00000000000000LL,
        0xFE00000000000000LL,
        ////B
        0x0000000000000000LL,
        0x0080000000000000LL,
        0x00C0000000000000LL,
        0x00E0000000000000LL,
        0x00F0000000000000LL,
        0x00F8000000000000LL,
        0x00FC000000000000LL,
        0x00FE000000000000LL,
        ////C
        0x0000000000000000LL,
        0x0000800000000000LL,
        0x0000C00000000000LL,
        0x0000E00000000000LL,
        0x0000F00000000000LL,
        0x0000F80000000000LL,
        0x0000FC0000000000LL,
        0x0000FE0000000000LL,
        ////D
        0x0000000000000000LL,
        0x0000008000000000LL,
        0x000000C000000000LL,
        0x000000E000000000LL,
        0x000000F000000000LL,
        0x000000F800000000LL,
        0x000000FC00000000LL,
        0x000000FE00000000LL,
        ////E
        0x0000000000000000LL,
        0x0000000080000000LL,
        0x00000000C0000000LL,
        0x00000000E0000000LL,
        0x00000000F0000000LL,
        0x00000000F8000000LL,
        0x00000000FC000000LL,
        0x00000000FE000000LL,
        ////F
        0x0000000000000000LL,
        0x0000000000800000LL,
        0x0000000000C00000LL,
        0x0000000000E00000LL,
        0x0000000000F00000LL,
        0x0000000000F80000LL,
        0x0000000000FC0000LL,
        0x0000000000FE0000LL,
        ////G
        0x0000000000000000LL,
        0x0000000000008000LL,
        0x000000000000C000LL,
        0x000000000000E000LL,
        0x000000000000F000LL,
        0x000000000000F800LL,
        0x000000000000FC00LL,
        0x000000000000FE00LL,
        ////H
        0x0000000000000000LL,
        0x0000000000000080LL,
        0x00000000000000C0LL,
        0x00000000000000E0LL,
        0x00000000000000F0LL,
        0x00000000000000F8LL,
        0x00000000000000FCLL,
        0x00000000000000FELL,

    },
    ////SW
    {
        ////A
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        ////B
        0x0000000000000000LL,
        0x8000000000000000LL,
        0x4000000000000000LL,
        0x2000000000000000LL,
        0x1000000000000000LL,
        0x0800000000000000LL,
        0x0400000000000000LL,
        0x0200000000000000LL,
        ////C
        0x0000000000000000LL,
        0x0080000000000000LL,
        0x8040000000000000LL,
        0x4020000000000000LL,
        0x2010000000000000LL,
        0x1008000000000000LL,
        0x0804000000000000LL,
        0x0402000000000000LL,
        ////D
        0x0000000000000000LL,
        0x0000800000000000LL,
        0x0080400000000000LL,
        0x8040200000000000LL,
        0x4020100000000000LL,
        0x2010080000000000LL,
        0x1008040000000000LL,
        0x0804020000000000LL,
        ////E
        0x0000000000000000LL,
        0x0000008000000000LL,
        0x0000804000000000LL,
        0x0080402000000000LL,
        0x8040201000000000LL,
        0x4020100800000000LL,
        0x2010080400000000LL,
        0x1008040200000000LL,
        ////F
        0x0000000000000000LL,
        0x0000000080000000LL,
        0x0000008040000000LL,
        0x0000804020000000LL,
        0x0080402010000000LL,
        0x8040201008000000LL,
        0x4020100804000000LL,
        0x2010080402000000LL,
        ////G
        0x0000000000000000LL,
        0x0000000000800000LL,
        0x0000000080400000LL,
        0x0000008040200000LL,
        0x0000804020100000LL,
        0x0080402010080000LL,
        0x8040201008040000LL,
        0x4020100804020000LL,
        ////H
        0x0000000000000000LL,
        0x0000000000008000LL,
        0x0000000000804000LL,
        0x0000000080402000LL,
        0x0000008040201000LL,
        0x0000804020100800LL,
        0x0080402010080400LL,
        0x8040201008040200LL,

    },
    ////W
    {
        ////A
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        ////B
        0x8000000000000000LL,
        0x4000000000000000LL,
        0x2000000000000000LL,
        0x1000000000000000LL,
        0x0800000000000000LL,
        0x0400000000000000LL,
        0x0200000000000000LL,
        0x0100000000000000LL,
        ////C
        0x8080000000000000LL,
        0x4040000000000000LL,
        0x2020000000000000LL,
        0x1010000000000000LL,
        0x0808000000000000LL,
        0x0404000000000000LL,
        0x0202000000000000LL,
        0x0101000000000000LL,
        ////D
        0x8080800000000000LL,
        0x4040400000000000LL,
        0x2020200000000000LL,
        0x1010100000000000LL,
        0x0808080000000000LL,
        0x0404040000000000LL,
        0x0202020000000000LL,
        0x0101010000000000LL,
        ////E
        0x8080808000000000LL,
        0x4040404000000000LL,
        0x2020202000000000LL,
        0x1010101000000000LL,
        0x0808080800000000LL,
        0x0404040400000000LL,
        0x0202020200000000LL,
        0x0101010100000000LL,
        ////F
        0x8080808080000000LL,
        0x4040404040000000LL,
        0x2020202020000000LL,
        0x1010101010000000LL,
        0x0808080808000000LL,
        0x0404040404000000LL,
        0x0202020202000000LL,
        0x0101010101000000LL,
        ////G
        0x8080808080800000LL,
        0x4040404040400000LL,
        0x2020202020200000LL,
        0x1010101010100000LL,
        0x0808080808080000LL,
        0x0404040404040000LL,
        0x0202020202020000LL,
        0x0101010101010000LL,
        ////H
        0x8080808080808000LL,
        0x4040404040404000LL,
        0x2020202020202000LL,
        0x1010101010101000LL,
        0x0808080808080800LL,
        0x0404040404040400LL,
        0x0202020202020200LL,
        0x0101010101010100LL,
    },
    ////NW
    {
        ////E
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        0x0000000000000000LL,
        ////E
        0x4000000000000000LL,
        0x2000000000000000LL,
        0x1000000000000000LL,
        0x0800000000000000LL,
        0x0400000000000000LL,
        0x0200000000000000LL,
        0x0100000000000000LL,
        0x0000000000000000LL,
        ////E
        0x2040000000000000LL,
        0x1020000000000000LL,
        0x0810000000000000LL,
        0x0408000000000000LL,
        0x0204000000000000LL,
        0x0102000000000000LL,
        0x0001000000000000LL,
        0x0000000000000000LL,
        ////E
        0x1020400000000000LL,
        0x0810200000000000LL,
        0x0408100000000000LL,
        0x0204080000000000LL,
        0x0102040000000000LL,
        0x0001020000000000LL,
        0x0000010000000000LL,
        0x0000000000000000LL,
        ////E
        0x0810204000000000LL,
        0x0408102000000000LL,
        0x0204081000000000LL,
        0x0102040800000000LL,
        0x0001020400000000LL,
        0x0000010200000000LL,
        0x0000000100000000LL,
        0x0000000000000000LL,
        ////F
        0x0408102040000000LL,
        0x0204081020000000LL,
        0x0102040810000000LL,
        0x0001020408000000LL,
        0x0000010204000000LL,
        0x0000000102000000LL,
        0x0000000001000000LL,
        0x0000000000000000LL,
        ////G
        0x0204081020400000LL,
        0x0102040810200000LL,
        0x0001020408100000LL,
        0x0000010204080000LL,
        0x0000000102040000LL,
        0x0000000001020000LL,
        0x0000000000010000LL,
        0x0000000000000000LL,
        ////H
        0x0102040810204000LL,
        0x0001020408102000LL,
        0x0000010204081000LL,
        0x0000000102040800LL,
        0x0000000001020400LL,
        0x0000000000010200LL,
        0x0000000000000100LL,
        0x0000000000000000LL,
    }};

const uint64_t knight_moves[64] = {
    0x20400000000000LL,
    0x10a00000000000LL,
    0x88500000000000LL,
    0x44280000000000LL,
    0x22140000000000LL,
    0x110a0000000000LL,
    0x8050000000000LL,
    0x4020000000000LL,
    0x2000204000000000LL,
    0x100010a000000000LL,
    0x8800885000000000LL,
    0x4400442800000000LL,
    0x2200221400000000LL,
    0x1100110a00000000LL,
    0x800080500000000LL,
    0x400040200000000LL,
    0x4020002040000000LL,
    0xa0100010a0000000LL,
    0x5088008850000000LL,
    0x2844004428000000LL,
    0x1422002214000000LL,
    0xa1100110a000000LL,
    0x508000805000000LL,
    0x204000402000000LL,
    0x40200020400000LL,
    0xa0100010a00000LL,
    0x50880088500000LL,
    0x28440044280000LL,
    0x14220022140000LL,
    0xa1100110a0000LL,
    0x5080008050000LL,
    0x2040004020000LL,
    0x402000204000LL,
    0xa0100010a000LL,
    0x508800885000LL,
    0x284400442800LL,
    0x142200221400LL,
    0xa1100110a00LL,
    0x50800080500LL,
    0x20400040200LL,
    0x4020002040LL,
    0xa0100010a0LL,
    0x5088008850LL,
    0x2844004428LL,
    0x1422002214LL,
    0xa1100110aLL,
    0x508000805LL,
    0x204000402LL,
    0x40200020LL,
    0xa0100010LL,
    0x50880088LL,
    0x28440044LL,
    0x14220022LL,
    0xa110011LL,
    0x5080008LL,
    0x2040004LL,
    0x402000LL,
    0xa01000LL,
    0x508800LL,
    0x284400LL,
    0x142200LL,
    0xa1100LL,
    0x50800LL,
    0x20400LL,
};

#endif